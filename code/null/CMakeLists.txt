set(SRCS
    null_client.c
    null_input.c
    null_snddma.c
)
add_library(null ${SRCS})
target_link_libraries(null qcommon)
