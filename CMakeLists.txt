cmake_minimum_required(VERSION 3.30.5)
project(img_parsing_server_ex LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_CURRENT_BINARY_DIR}/build>")

add_subdirectory(src)

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
  message("!!!Found opencv!!")
  include_directories(${OpenCV_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
else()
  message("!!!!NOT Found opencv!!!")
endif()