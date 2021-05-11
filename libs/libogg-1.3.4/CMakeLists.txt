file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS *.c)
add_library(ogg STATIC ${SRCS})
target_include_directories(ogg PUBLIC include)
