file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS *.c)
add_library(zlib STATIC ${SRCS})
target_include_directories(zlib PUBLIC .)
