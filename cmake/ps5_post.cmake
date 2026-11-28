if(NOT PS5)
  return()
endif()
target_include_directories(${PROJECT_NAME} PRIVATE ${CODE_DIR} ${CODE_DIR}/ps5 ${CODE_DIR}/sys ${CODE_DIR}/renderer_vulkan)
target_compile_definitions(${PROJECT_NAME} PRIVATE PS5_TITLE_NAME="World of Padman")
target_link_options(${PROJECT_NAME} PRIVATE -Wl,--unresolved-symbols=ignore-all)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND bash "${CODE_DIR}/ps5/link-title.sh" "${CMAKE_BINARY_DIR}" "${PS5_VULKAN_DIR}" "${PS5_PAYLOAD_SDK}" "$<TARGET_FILE:${PROJECT_NAME}>"
  COMMENT "Linking RADV & packaging PS5 title PPSA99517"
)
