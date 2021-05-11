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
	lib/cpu.c
	lib/enquant.c
	lib/x86/mmxstate.c
	lib/x86/mmxidct.c
	lib/x86/mmxfrag.c
	lib/x86/x86state.c
	lib/x86/mmxencfrag.c
	lib/x86/x86enc.c
	lib/x86/mmxfdct.c
)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	list(APPEND SRCS lib/x86/sse2fdct.c)
endif()

add_library(theora STATIC ${SRCS})
target_link_libraries(theora PUBLIC ogg)
target_include_directories(theora PUBLIC include lib)
