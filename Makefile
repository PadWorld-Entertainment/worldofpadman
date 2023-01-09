# Allow to override settings
CONFIG         ?= Makefile.local
-include $(CONFIG)

Q              ?= @
UPDATEDIR      := /tmp
BUILDTYPE      ?= Debug
BUILDDIR       ?= ./build/$(BUILDTYPE)
INSTALL_DIR    ?= $(BUILDDIR)
GENERATOR      := Ninja
CMAKE          ?= cmake
CMAKE_OPTIONS  ?= -DCMAKE_BUILD_TYPE=$(BUILDTYPE) -G$(GENERATOR) --graphviz=$(BUILDDIR)/deps.dot

all:
	$(Q)if [ ! -f $(BUILDDIR)/CMakeCache.txt ]; then $(CMAKE) -H$(CURDIR) -B$(BUILDDIR) $(CMAKE_OPTIONS); fi
	$(Q)$(CMAKE) --build $(BUILDDIR) --target $@
	$(Q)$(CMAKE) -E create_symlink $(BUILDDIR)/compile_commands.json compile_commands.json

release:
	$(Q)$(MAKE) BUILDTYPE=Release

clean:
	$(Q)rm -rf $(BUILDDIR)

distclean:
	$(Q)git clean -fdx

.PHONY: cmake
cmake:
	$(Q)$(CMAKE) -H$(CURDIR) -B$(BUILDDIR) $(CMAKE_OPTIONS)

.PHONY: ccmake
ccmake:
	$(Q)ccmake -B$(BUILDDIR) -S.

%:
	$(Q)if [ ! -f $(BUILDDIR)/CMakeCache.txt ]; then $(CMAKE) -H$(CURDIR) -B$(BUILDDIR) $(CMAKE_OPTIONS); fi
	$(Q)$(CMAKE) --build $(BUILDDIR) --target $@
	$(Q)$(CMAKE) --install $(BUILDDIR) --component $@ --prefix $(INSTALL_DIR)/install-$@
	$(Q)$(CMAKE) -E create_symlink $(BUILDDIR)/compile_commands.json compile_commands.json

define UPDATE_GIT
	$(Q)if [ ! -d $(UPDATEDIR)/$(1).sync ]; then \
		git clone --depth=1 $(2) $(UPDATEDIR)/$(1).sync; \
	else \
		cd $(UPDATEDIR)/$(1).sync && git pull --depth=1 --rebase; \
	fi;
endef

update-sdl2:
	$(call UPDATE_GIT,sdl2,https://github.com/libsdl-org/SDL.git -b SDL2)
	rm -rf libs/SDL2/src/* libs/SDL2/include/* libs/SDL2/cmake/*
	cp -r $(UPDATEDIR)/sdl2.sync/* libs/SDL2

OPENAL_VERSION=1.21.1
update-openal:
	$(call UPDATE_GIT,openal,https://github.com/kcat/openal-soft.git -b $(OPENAL_VERSION))
	curl https://www.openal-soft.org/openal-binaries/openal-soft-$(OPENAL_VERSION)-bin.zip -o $(UPDATEDIR)/openal-soft-bin.zip
	unzip -o $(UPDATEDIR)/openal-soft-bin.zip -d $(UPDATEDIR)
	cp $(UPDATEDIR)/openal-soft-$(OPENAL_VERSION)-bin/bin/Win64/soft_oal.dll libs/libs/win64/OpenAL32.dll
	chmod -x libs/libs/win64/OpenAL32.dll
	cp -r $(UPDATEDIR)/openal.sync/include/AL/* libs/AL

LIBOGG_VERSION=1.3.5
update-libogg:
	curl -L https://github.com/xiph/ogg/releases/download/v$(LIBOGG_VERSION)/libogg-$(LIBOGG_VERSION).zip -o $(UPDATEDIR)/libogg.zip
	unzip -o $(UPDATEDIR)/libogg.zip -d $(UPDATEDIR)
	rm -f libs/libogg-$(LIBOGG_VERSION)/src/*.c
	rm -f libs/libogg-$(LIBOGG_VERSION)/include/ogg/*.h
	cp $(UPDATEDIR)/libogg-$(LIBOGG_VERSION)/src/*.c libs/libogg-$(LIBOGG_VERSION)/src
	cp $(UPDATEDIR)/libogg-$(LIBOGG_VERSION)/include/ogg/*.h libs/libogg-$(LIBOGG_VERSION)/include/ogg

JPEG_VERSION=9e
update-jpeg:
	curl -L https://ijg.org/files/jpegsr$(JPEG_VERSION).zip -o $(UPDATEDIR)/jpeg.zip
	unzip -o $(UPDATEDIR)/jpeg.zip -d $(UPDATEDIR)
	rm -f libs/jpeg-*/*.[ch]
	cp $(UPDATEDIR)/jpeg-$(JPEG_VERSION)/*.[ch] libs/jpeg-$(JPEG_VERSION)
	find libs/jpeg-$(JPEG_VERSION) -type f -exec sed -i 's/\r//g' {} \;

OPUSFILE_VERSION=0.12
update-opusfile:
	curl -L https://downloads.xiph.org/releases/opus/opusfile-$(OPUSFILE_VERSION).zip -o $(UPDATEDIR)/opusfile.zip
	unzip -o $(UPDATEDIR)/opusfile.zip -d $(UPDATEDIR)
	rm -f libs/opusfile-$(OPUSFILE_VERSION)/src/*.c
	rm -f libs/opusfile-$(OPUSFILE_VERSION)/include/*.h
	cp $(UPDATEDIR)/opusfile-$(OPUSFILE_VERSION)/src/*.c libs/opusfile-$(OPUSFILE_VERSION)/src
	cp $(UPDATEDIR)/opusfile-$(OPUSFILE_VERSION)/include/*.h libs/opusfile-$(OPUSFILE_VERSION)/include

LIBTHEORA_VERSION=1.1.1
update-libtheora:
	curl -L https://downloads.xiph.org/releases/theora/libtheora-$(LIBTHEORA_VERSION).zip -o $(UPDATEDIR)/libtheora.zip
	unzip -o $(UPDATEDIR)/libtheora.zip -d $(UPDATEDIR)
	echo Todo

LIBVORBIS_VERSION=1.3.7
update-libvorbis:
	curl -L https://downloads.xiph.org/releases/vorbis/libvorbis-$(LIBVORBIS_VERSION).zip -o $(UPDATEDIR)/libvorbis.zip
	unzip -o $(UPDATEDIR)/libvorbis.zip -d $(UPDATEDIR)
	echo Todo

OPUS_VERSION=1.3.1
update-opus:
	curl -L https://archive.mozilla.org/pub/opus/opus-$(OPUS_VERSION).tar.gz -o $(UPDATEDIR)/opus.tar.gz
	tar -xzf $(UPDATEDIR)/opus.tar.gz -C $(UPDATEDIR)
	echo Todo
