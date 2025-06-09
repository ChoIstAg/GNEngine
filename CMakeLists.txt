cmake_minimum_required(VERSION 3.30.0)
project(sdl3_test VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++23")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")


#set(CMAKE_SOURCE_DIR "c:/Devlopment/code/project/sdl3_test") #cause error in adding sdl3

set(SDL3_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3/x86_64-w64-mingw32/lib/cmake/SDL3")
set(SDL3_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3/x86_64-w64-mingw32/include")
set(SDL3_image_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_image/x86_64-w64-mingw32/lib/cmake/SDL3_image")
set(SDL3_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_image/x86_64-w64-mingw32/include/SDL3_image")
set(SDL3_ttf_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_ttf/x86_64-w64-mingw32/lib/cmake/SDL3_ttf")
set(SDL3_TTF_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_ttf/x86_64-w64-mingw32/include/SDL3_ttf")

# 프로젝트 루트 경로를 PROJECT_ROOT로 정의
#add_definitions(-DPROJECT_ROOT_PATH=\"${CMAKE_SOURCE_DIR}\")

add_subdirectory(
    src
)