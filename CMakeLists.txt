cmake_minimum_required(VERSION 3.30.5)
project(img_parsing_server_ex LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_CURRENT_BINARY_DIR}/build>")

add_subdirectory(src)

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
else()
  message("!!!!NOT Found opencv!!!")
endif()

set(BOOST_ROOT /usr/include/boost)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.83 REQUIRED)


if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIR})
else()
  message("!!!NOT Found boost!!!")
endif()