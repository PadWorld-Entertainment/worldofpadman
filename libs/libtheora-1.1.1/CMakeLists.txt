set(SRCS
	lib/apiwrapper.c
	lib/bitpack.c
	lib/decapiwrapper.c
	lib/decinfo.c
	lib/decode.c
	lib/dequant.c
	lib/fragment.c
	lib/huffdec.c
	lib/idct.c
	lib/info.c
	lib/internal.c
	lib/quant.c
	lib/state.c
	lib/analyze.c
	lib/fdct.c
	lib/encfrag.c
	lib/encapiwrapper.c
	lib/encinfo.c
	lib/encode.c
	lib/huffenc.c
	lib/mathops.c
	lib/mcenc.c
	lib/rate.c
	lib/tokenize.c
	lib/enquant.c
)

add_library(theora STATIC ${SRCS})
target_link_libraries(theora PUBLIC vorbis)
target_include_directories(theora PUBLIC include lib)
target_compile_definitions(theora PUBLIC USE_CIN_THEORA)

include(CheckCSourceCompiles)
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang" AND NOT MSVC)
	check_c_source_compiles("
		#if !defined(__i386__) && !defined(__x86_64__)
		#error Not an x86 target
		#endif
		int main(void) { __asm__ volatile (\"emms\"); return 0; }
	" THEORA_GNU_X86_ASM)
	if (THEORA_GNU_X86_ASM)
		set(THEORA_ASM_DIR x86)
	endif()
elseif (MSVC)
	check_c_source_compiles("
		#ifndef _M_IX86
		#error MSVC inline assembly requires 32-bit x86
		#endif
		int main(void) {
			__asm { emms }
			return 0;
		}
	" THEORA_MSVC_X86_ASM)
	if (THEORA_MSVC_X86_ASM)
		set(THEORA_ASM_DIR x86_vc)
	endif()
endif()

if (THEORA_ASM_DIR)
	target_compile_definitions(theora PRIVATE OC_X86_ASM)
	foreach (SRC mmxstate mmxidct mmxfrag x86state mmxencfrag x86enc mmxfdct)
		target_sources(theora PRIVATE lib/${THEORA_ASM_DIR}/${SRC}.c)
	endforeach()
	if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/${THEORA_ASM_DIR}/x86cpu.c")
		target_sources(theora PRIVATE lib/${THEORA_ASM_DIR}/x86cpu.c)
		if (THEORA_ASM_DIR STREQUAL "x86")
			target_sources(theora PRIVATE lib/x86/sse2idct.c lib/x86/sse2encfrag.c lib/x86/x86enquant.c)
		endif()
	else()
		target_sources(theora PRIVATE lib/cpu.c)
	endif()
	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		target_compile_definitions(theora PRIVATE OC_X86_64_ASM)
		target_sources(theora PRIVATE lib/x86/sse2fdct.c)
	endif()
	message(STATUS "Theora: ${THEORA_ASM_DIR} assembly enabled")
else()
	message(STATUS "Theora: portable C backend (no supported assembly for this compiler/target)")
endif()
