cmake_minimum_required(VERSION 3.10)

project(ds_course_design)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-Werror -Wall -lpthread -O3)

include_directories(./include)

file(GLOB_RECURSE SRC src/*.cc)

find_package(OpenCV REQUIRED)

set(PROJECT_SRC ${SRC})

add_executable(ds_course_design ${PROJECT_SRC})
target_link_libraries(ds_course_design ${OpenCV_LIBS})
