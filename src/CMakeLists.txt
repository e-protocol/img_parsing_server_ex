file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/ *.h *.hpp *.cc *.cpp)
add_executable(${PROJECT_NAME} ${SRC})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${CMAKE_CURRENT_SOURCE_DIR}/haarcascade_frontalface_alt.xml
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>)          