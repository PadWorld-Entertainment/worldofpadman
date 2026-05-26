if(NOT DEFINED BSPC_EXECUTABLE)
    message(FATAL_ERROR "BSPC_EXECUTABLE is required")
endif()

if(NOT DEFINED BSP_INPUT)
    message(FATAL_ERROR "BSP_INPUT is required")
endif()

if(NOT DEFINED WORK_DIR)
    message(FATAL_ERROR "WORK_DIR is required")
endif()

if(NOT DEFINED LOG_FILE)
    message(FATAL_ERROR "LOG_FILE is required")
endif()

if(NOT DEFINED BSPC_OPTIONS)
    set(BSPC_OPTIONS "")
endif()

set(BSPC_INTERNAL_LOG "${WORK_DIR}/bspc.log")

file(MAKE_DIRECTORY "${WORK_DIR}")
get_filename_component(LOG_DIR "${LOG_FILE}" DIRECTORY)
file(MAKE_DIRECTORY "${LOG_DIR}")
file(REMOVE "${LOG_FILE}" "${BSPC_INTERNAL_LOG}")

separate_arguments(BSPC_OPTIONS_LIST NATIVE_COMMAND "${BSPC_OPTIONS}")

execute_process(
    COMMAND "${BSPC_EXECUTABLE}" ${BSPC_OPTIONS_LIST} -bsp2aas "${BSP_INPUT}"
    WORKING_DIRECTORY "${WORK_DIR}"
    RESULT_VARIABLE BSPC_RESULT
    OUTPUT_FILE "${LOG_FILE}"
    ERROR_FILE "${LOG_FILE}"
)

file(APPEND "${LOG_FILE}" "\n\n=== wrapper status ===\nexit code: ${BSPC_RESULT}\n")

if(EXISTS "${BSPC_INTERNAL_LOG}")
    file(APPEND "${LOG_FILE}" "\n\n=== bspc internal log ===\n")
    file(READ "${BSPC_INTERNAL_LOG}" BSPC_INTERNAL_LOG_CONTENT)
    file(APPEND "${LOG_FILE}" "${BSPC_INTERNAL_LOG_CONTENT}")
    file(REMOVE "${BSPC_INTERNAL_LOG}")
endif()

if(NOT BSPC_RESULT EQUAL 0)
    message(FATAL_ERROR "wopbspc failed with exit code ${BSPC_RESULT} for ${BSP_INPUT}. See ${LOG_FILE}")
endif()
