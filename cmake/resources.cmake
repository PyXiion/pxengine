function(copy_resource_folder TARGET)
  add_custom_command(TARGET ${TARGET} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data/ $<TARGET_FILE_DIR:${TARGET}>/data/)
endfunction()