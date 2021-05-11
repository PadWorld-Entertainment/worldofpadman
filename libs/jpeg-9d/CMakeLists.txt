file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS *.c)
add_library(jpeg STATIC ${SRCS})
target_include_directories(jpeg PUBLIC .)
set_property(TARGET jpeg PROPERTY POSITION_INDEPENDENT_CODE ON)
