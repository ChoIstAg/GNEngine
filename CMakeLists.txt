cmake_minimum_required(VERSION 3.25)
project(GNEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++23")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# SDL 경로 설정
set(SDL3_DIR "${CMAKE_SOURCE_DIR}/include/lib/SDL3-3.2.18/x86_64-w64-mingw32/lib/cmake/SDL3")
set(SDL3_DLL_DIR "${CMAKE_SOURCE_DIR}/include/lib/SDL3-3.2.18/x86_64-w64-mingw32/bin")
set(SDL3_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/lib/SDL3-3.2.18/x86_64-w64-mingw32/include")
set(SDL3_image_DIR "${CMAKE_SOURCE_DIR}/include/lib/SDL3_image-3.2.4/x86_64-w64-mingw32/lib/cmake/SDL3_image")
set(SDL3_image_DLL_DIR "${CMAKE_SOURCE_DIR}/include/lib/SDL3_image-3.2.4/x86_64-w64-mingw32/bin")
set(SDL3_image_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/lib/SDL3_image-3.2.4/x86_64-w64-mingw32/include")
set(SDL3_ttf_DIR "${CMAKE_SOURCE_DIR}/include/lib/SDL3_ttf-3.2.2/x86_64-w64-mingw32/lib/cmake/SDL3_ttf")
set(SDL3_ttf_DLL_DIR "${CMAKE_SOURCE_DIR}/include/lib/SDL3_ttf-3.2.2/x86_64-w64-mingw32/bin")
set(SDL3_ttf_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/lib/SDL3_ttf-3.2.2/x86_64-w64-mingw32/include")


# config.h 생성 #프로젝트 경로 지정용. (소스에서 사용) 
#configure_file(${CMAKE_SOURCE_DIR}/include/config.h ${CMAKE_BINARY_DIR}/config.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/include/config.h.in ${CMAKE_BINARY_DIR}/config.h @ONLY)
include_directories(${CMAKE_BINARY_DIR})

# SDL 패키지 찾기
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

add_subdirectory(
    src
    include
)

# target_include_directories
target_include_directories(GNEngine
PUBLIC
    #includ engine lib
    "${CMAKE_SOURCE_DIR}/include"

    "${SDL3_INCLUDE_DIR}"
    "${SDL3_image_INCLUDE_DIR}"
    "${SDL3_ttf_INCLUDE_DIR}"
)

# target_link_libraries
target_link_libraries(GNEngine 
PUBLIC 
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

# target_compile_definitions
target_compile_definitions(GNEngine
PUBLIC
    PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}"
)