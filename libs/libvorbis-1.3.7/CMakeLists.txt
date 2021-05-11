file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS *.c)
add_library(vorbis STATIC ${SRCS})
target_link_libraries(vorbis PUBLIC ogg)
target_include_directories(vorbis PUBLIC include lib)
