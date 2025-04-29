file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/ *.h *.hpp *.cc *.cpp)
add_executable(${PROJECT_NAME} ${SRC})

set(FILES_TO_COPY ${CMAKE_CURRENT_SOURCE_DIR}/haarcascade_frontalcatface.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/haarcascade_frontalface_alt.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/haarcascade_frontalface_alt2.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/haarcascade_frontalface_alt_tree.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/haarcascade_frontalface_default.xml)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${FILES_TO_COPY}
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>)          