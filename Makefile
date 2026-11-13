# Allow to override settings
CONFIG         ?= Makefile.local
-include $(CONFIG)

Q              ?= @
UPDATEDIR      ?= /tmp
BUILDTYPE      ?= Debug
BUILDDIR       ?= ./build/$(BUILDTYPE)
INSTALL_DIR    ?= $(BUILDDIR)
GENERATOR      ?= Ninja
ALLTARGET      ?= all
CMAKE          ?= cmake
CMAKE_OPTIONS  ?= -DCMAKE_BUILD_TYPE=$(BUILDTYPE) -G$(GENERATOR) --graphviz=$(BUILDDIR)/deps.dot
$(ALLTARGET): $(BUILDDIR)/CMakeCache.txt
	$(Q)$(CMAKE) --build $(BUILDDIR) --target $@
	$(Q)$(CMAKE) -E create_symlink $(BUILDDIR)/compile_commands.json compile_commands.json

$(BUILDDIR)/CMakeCache.txt:
	$(CMAKE) -H$(CURDIR) -B$(BUILDDIR) $(CMAKE_OPTIONS)

release:
	$(Q)$(MAKE) BUILDTYPE=Release

clean:
	$(Q)rm -rf $(BUILDDIR)

test:
	$(Q)$(CMAKE) --build $(BUILDDIR) --target test

distclean:
	$(Q)git clean -fdx

.PHONY: cmake
cmake:
	$(Q)$(CMAKE) -H$(CURDIR) -B$(BUILDDIR) $(CMAKE_OPTIONS)

package:
	$(Q)cd $(BUILDDIR); cpack

.PHONY: ccmake
ccmake:
	$(Q)ccmake -B$(BUILDDIR) -S.

%:
	$(Q)if [ ! -f $(BUILDDIR)/CMakeCache.txt ]; then $(CMAKE) -H$(CURDIR) -B$(BUILDDIR) $(CMAKE_OPTIONS); fi
	$(Q)$(CMAKE) --build $(BUILDDIR) --target $@
	$(Q)$(CMAKE) --install $(BUILDDIR) --component $@ --prefix $(INSTALL_DIR)/install-$@
	$(Q)$(CMAKE) -E create_symlink $(BUILDDIR)/compile_commands.json compile_commands.json

# Extract into a fresh directory; a failed download must not touch vendored files.
define UPDATE_ARCHIVE
	$(Q)set -eu; \
	stage=$$(mktemp -d "$(UPDATEDIR)/wop-update.XXXXXX"); \
	trap 'rm -rf "$$stage"' EXIT HUP INT TERM; \
	curl --fail --location --show-error "$(2)" -o "$$stage/source.tar.gz"; \
	tar -xzf "$$stage/source.tar.gz" -C "$$stage"; \
	src="$$stage/$(1)"; \
	test -d "$$src";
endef

.PHONY: update-sdl2 update-openal update-libogg update-jpeg update-opusfile update-libtheora update-libvorbis update-opus update-zlib

# Keep the existing vendored paths: libs/CMakeLists.txt and local build files use them.
SDL2_VERSION=2.32.10
update-sdl2:
	$(call UPDATE_ARCHIVE,SDL2-$(SDL2_VERSION),https://github.com/libsdl-org/SDL/releases/download/release-$(SDL2_VERSION)/SDL2-$(SDL2_VERSION).tar.gz) \
	rm -rf libs/SDL2/src libs/SDL2/include libs/SDL2/cmake; \
	cp -r "$$src/." libs/SDL2

OPENAL_VERSION=1.25.2
update-openal:
	$(call UPDATE_ARCHIVE,openal-soft-$(OPENAL_VERSION),https://github.com/kcat/openal-soft/archive/refs/tags/$(OPENAL_VERSION).tar.gz) \
	curl --fail --location --show-error https://github.com/kcat/openal-soft/releases/download/$(OPENAL_VERSION)/openal-soft-$(OPENAL_VERSION)-bin.zip -o "$$stage/bin.zip"; \
	unzip -q "$$stage/bin.zip" -d "$$stage"; \
	cp "$$stage/openal-soft-$(OPENAL_VERSION)-bin/bin/Win64/soft_oal.dll" libs/libs/win64/OpenAL32.dll; \
	chmod -x libs/libs/win64/OpenAL32.dll; \
	cp "$$src"/include/AL/*.h libs/AL

LIBOGG_VERSION=1.3.6
update-libogg:
	$(call UPDATE_ARCHIVE,libogg-$(LIBOGG_VERSION),https://downloads.xiph.org/releases/ogg/libogg-$(LIBOGG_VERSION).tar.gz) \
	cp libs/libogg-1.3.5/include/ogg/config_types.h "$$src/include/ogg/config_types.h"; \
	rm -f libs/libogg-1.3.5/src/*.[ch] libs/libogg-1.3.5/include/ogg/*.h; \
	cp "$$src"/src/*.[ch] libs/libogg-1.3.5/src; \
	cp "$$src"/include/ogg/*.h libs/libogg-1.3.5/include/ogg

JPEG_VERSION=10
update-jpeg:
	$(call UPDATE_ARCHIVE,jpeg-$(JPEG_VERSION),https://ijg.org/files/jpegsrc.v$(JPEG_VERSION).tar.gz) \
	cp libs/jpeg-9e/jconfig.h "$$src/jconfig.h"; \
	rm -f libs/jpeg-9e/*.[ch]; \
	cp "$$src"/*.[ch] libs/jpeg-9e; \
	cp "$$src/README" libs/jpeg-9e/README

OPUSFILE_VERSION=0.12
update-opusfile:
	$(call UPDATE_ARCHIVE,opusfile-$(OPUSFILE_VERSION),https://downloads.xiph.org/releases/opus/opusfile-$(OPUSFILE_VERSION).tar.gz) \
	rm -f libs/opusfile-0.12/src/*.[ch] libs/opusfile-0.12/include/*.h; \
	cp "$$src"/src/*.[ch] libs/opusfile-0.12/src; \
	cp "$$src"/include/*.h libs/opusfile-0.12/include

LIBTHEORA_VERSION=1.2.0
update-libtheora:
	$(call UPDATE_ARCHIVE,libtheora-$(LIBTHEORA_VERSION),https://downloads.xiph.org/releases/theora/libtheora-$(LIBTHEORA_VERSION).tar.gz) \
	rm -rf libs/libtheora-1.1.1/lib libs/libtheora-1.1.1/include; \
	cp -r "$$src/lib" "$$src/include" libs/libtheora-1.1.1

LIBVORBIS_VERSION=1.3.7
update-libvorbis:
	$(call UPDATE_ARCHIVE,libvorbis-$(LIBVORBIS_VERSION),https://downloads.xiph.org/releases/vorbis/libvorbis-$(LIBVORBIS_VERSION).tar.gz) \
	rm -rf libs/libvorbis-1.3.7/lib libs/libvorbis-1.3.7/include; \
	cp -r "$$src/lib" "$$src/include" libs/libvorbis-1.3.7

OPUS_VERSION=1.6.1
update-opus:
	$(call UPDATE_ARCHIVE,opus-$(OPUS_VERSION),https://downloads.xiph.org/releases/opus/opus-$(OPUS_VERSION).tar.gz) \
	rm -rf libs/opus-1.3.1/src libs/opus-1.3.1/celt libs/opus-1.3.1/silk libs/opus-1.3.1/include libs/opus-1.3.1/dnn libs/opus-1.3.1/cmake; \
	cp -r "$$src/." libs/opus-1.3.1

ZLIB_VERSION=1.4
update-zlib:
	$(call UPDATE_ARCHIVE,zlib-$(ZLIB_VERSION),https://github.com/madler/zlib/releases/download/v$(ZLIB_VERSION)/zlib-$(ZLIB_VERSION).tar.gz) \
	rm -f libs/zlib/*.[ch]; \
	cp "$$src"/*.[ch] libs/zlib
