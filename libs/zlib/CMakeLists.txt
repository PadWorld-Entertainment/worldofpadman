set(SRCS
	adler32.c
	compress.c
	crc32.c
	deflate.c
	inffast.c
	inflate.c
	inftrees.c
	trees.c
	zutil.c
)
add_library(zlib STATIC ${SRCS})
target_include_directories(zlib PUBLIC .)
target_compile_definitions(zlib PRIVATE NO_GZIP)
