#!/usr/bin/env bash
set -euo pipefail
work=$1
vulkan=$2
sdk=$3
exe_in=$4

ps5=$(cd -- "$(dirname -- "${BASH_SOURCE}")" && pwd)
archive=${RADV_ARCHIVE:-$vulkan/.deps/native/radv-release/lib/libvulkan_radeon.ps5.a}
export PS5_CLANG=${PS5_CLANG:-$(command -v clang-22 || command -v clang || true)}
tool="$vulkan/build/host/ps5-native-tool"
native="$vulkan/tooling/native"
param="$ps5/sce_sys/param.json"

for file in "$archive" "$tool" "$param" "$sdk/bin/prospero-lld" "$vulkan/tools/radv-link.sh" "$vulkan/runtime/libc.prx" "$exe_in"; do
    [[ -e $file ]] || { echo "missing: $file" >&2; exit 2; }
done

mkdir -p "$work/link/obj" "$work/link/stubs"
cc() { PS5_PAYLOAD_SDK="$sdk" sh "$vulkan/tooling/prospero-clang18" "$@"; }

cc -std=c++20 -O2 -fno-exceptions -fno-rtti -c "$native/app_crt.cpp" -o "$work/link/obj/app_crt.o"
stub() {
    local library=$1 source=$2
    cc -std=c11 -O2 -fPIC -c "$source" -o "$work/link/obj/${library}_stub.o"
    "$sdk/bin/prospero-lld" --shared -soname "${library}.prx" -o "$work/link/stubs/${library}.so" "$work/link/obj/${library}_stub.o"
}
stub libSceAgc "$vulkan/vendor/ps5/sdk/stubs/agc_canary_link_stub.c"
stub libSceAgcDriver "$vulkan/vendor/ps5/sdk/stubs/agc_driver_canary_link_stub.c"
stub libSceMouse "$ps5/stubs/libSceMouse_stub.c"

source "$vulkan/tools/radv-link.sh"
radv_link_recipe "$vulkan" "$sdk" "$archive" || exit 2
printf "{\n    local:\n        gethostbyname;\n        mkstemp;\n        readlink;\n};\n" > "$work/link/extra-local.map"
radv_link_flags+=(--defsym=gethostbyname=ps5q3_gethostbyname --defsym=readlink=ps5q3_readlink --defsym=mkstemp=ps5q3_mkstemp --version-script "$work/link/extra-local.map")

platform_has_localeconv=false
grep -q " T ps5_localeconv$" <<<"$("$sdk/bin/llvm-nm" --defined-only "$sdk/target/lib/libps5platform.a" 2>/dev/null)" && platform_has_localeconv=true
if $platform_has_localeconv && [[ " ${radv_link_flags[*]} " != *" --defsym=localeconv=ps5_localeconv "* ]]; then
    printf '{\n    local:\n        localeconv;\n};\n' > "$work/link/localeconv-local.map"
    radv_link_flags+=(--defsym=gethostbyname=ps5q3_gethostbyname --defsym=readlink=ps5q3_readlink --defsym=mkstemp=ps5q3_mkstemp --defsym=localeconv=ps5_localeconv --version-script "$work/link/localeconv-local.map")
fi

# Extract all object files from CMake's linked executable/archive or link directly against its .o files
mapfile -t cmake_objs < <(find "$work" -name '*.o' ! -path "$work/link/*" ! -path "$work/libs/zlib/*" ! -path "$work/CMakeFiles/*CompilerId*")

"$sdk/bin/prospero-lld" "${radv_linker_script[@]}" --eh-frame-hdr "${radv_link_flags[@]}" \
    --version-script "$native/app-symbols.map" --exclude-libs=ALL \
    -e _start -o "$work/link/llvm-pie.elf" \
    "$work/link/obj/app_crt.o" "${cmake_objs[@]}" \
    "$work/link/stubs/libSceAgc.so" "$work/link/stubs/libSceAgcDriver.so" \
    "$work/link/stubs/libSceMouse.so" \
    "${radv_link_inputs[@]}" \
    --as-needed "$sdk"/target/lib/*.so

null_imports=$(comm -23 \
    <("$sdk/bin/llvm-nm" -D --undefined-only "$work/link/llvm-pie.elf" | awk '$1 == "U" { sub(/@.*/, "", $2); print $2 }' | sort -u) \
    <(for library in "$sdk"/target/lib/*.so "$work/link/stubs/libSceAgc.so" "$work/link/stubs/libSceAgcDriver.so" "$work/link/stubs/libSceMouse.so"; do
        case ${library##*/} in libkernel_sys.so | libScePosixForWebKit.so) continue ;; esac
        "$sdk/bin/llvm-nm" -D --defined-only "$library" 2>/dev/null | awk '{ print $NF }'
    done | sort -u))
if [[ -n $null_imports ]]; then
    echo "link-title.sh: null imports: ${null_imports//$'
'/ }" >&2
    exit 1
fi

"$tool" link --in "$work/link/llvm-pie.elf" --out "$work/eboot.elf.new" \
    --stub-dir "$sdk/target/lib" --stub "$work/link/stubs/libSceAgc.so" \
    --stub "$work/link/stubs/libSceAgcDriver.so" --stub "$work/link/stubs/libSceMouse.so" --module-sdk 0x02000009 \
    --companion-sdk 0x08050001 --file-name eboot.elf

title_id=$(python3 -c 'import json,sys; print(json.load(open(sys.argv[1]))["titleId"])' "$param")
app="$work/$title_id"
mkdir -p "$app/sce_sys" "$app/sce_module" "$app/wop"
"$tool" self --sign --in "$work/eboot.elf.new" --out "$app/eboot.bin" --magic 0x1D3D154F
"$tool" self --inspect --file "$app/eboot.bin" > /dev/null
cp "$param" "$app/sce_sys/param.json"
(cd "$vulkan/runtime" && sha256sum --check --strict --quiet libc.prx.sha256)
cp "$vulkan/runtime/libc.prx" "$app/sce_module/libc.prx"
mv "$work/eboot.elf.new" "$work/eboot.elf"
printf '==> %s: %s (eboot.bin %s bytes)\n' "$title_id" "$app" "$(stat -c %s "$app/eboot.bin")"
