cmake_minimum_required(VERSION 3.25)
project(GNEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++23")

# Set output directories for libraries and executables to keep the build directory clean.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)

# SDL 경로 설정 
set(SDL3_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3-3.2.18/x86_64-w64-mingw32/lib/cmake/SDL3")
set(SDL3_DLL_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3-3.2.18/x86_64-w64-mingw32/bin")
set(SDL3_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3-3.2.18/x86_64-w64-mingw32/include")
set(SDL3_image_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_image-3.2.4/x86_64-w64-mingw32/lib/cmake/SDL3_image")
set(SDL3_image_DLL_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_image-3.2.4/x86_64-w64-mingw32/bin")
set(SDL3_image_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_image-3.2.4/x86_64-w64-mingw32/include")
set(SDL3_ttf_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_ttf-3.2.2/x86_64-w64-mingw32/lib/cmake/SDL3_ttf")
set(SDL3_ttf_DLL_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_ttf-3.2.2/x86_64-w64-mingw32/bin")
set(SDL3_ttf_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_ttf-3.2.2/x86_64-w64-mingw32/include")

# OpenAL Soft 경로 설정
set(OPENAL_DIR "${CMAKE_SOURCE_DIR}/lib/openal-soft-1.24.3-bin")

# MinGW 런타임 경로 설정
set(MINGW_BIN_DIR "C:/Development_Environment/code/MSYS2/ucrt64/bin")

# SDL 패키지 찾기
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

# Box2D
add_subdirectory(lib/box2d-3.1.1)

# Create a directory for generated files to keep the build directory clean.
set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)

# Generate GNEngineRootPath.h from the .in template for use in the source code.
configure_file(
    "${CMAKE_SOURCE_DIR}/include/GNEngine/GNEngineRootPath.h.in"
    "${GENERATED_DIR}/GNEngineRootPath.h"
    @ONLY
)

# GNEngine 라이브러리 정의
add_library(GNEngine SHARED
    # Include All source files.
    src/GNEngine/core/ComponentArray.cpp
    src/GNEngine/core/Animation.cpp
    src/GNEngine/core/Sound.cpp
    src/GNEngine/core/Text.cpp

    src/GNEngine/manager/FileManager.cpp
    src/GNEngine/manager/EntityManager.cpp
    src/GNEngine/manager/SystemManager.cpp
    src/GNEngine/manager/RenderManager.cpp
    src/GNEngine/manager/InputManager.cpp
    src/GNEngine/manager/SceneManager.cpp
    src/GNEngine/manager/SoundManager.cpp
    src/GNEngine/manager/AnimationManager.cpp
    src/GNEngine/manager/TextManager.cpp
    src/GNEngine/manager/TextureManager.cpp
    src/GNEngine/manager/FadeManager.cpp
    src/GNEngine/GNManager.cpp
    
    src/GNEngine/system/RenderSystem.cpp
    src/GNEngine/system/SoundSystem.cpp
    src/GNEngine/system/InputSystem.cpp
    src/GNEngine/system/CameraSystem.cpp
    src/GNEngine/system/FadeSystem.cpp
    src/GNEngine/system/AnimationSystem.cpp
    src/GNEngine/system/MovementSystem.cpp
    src/GNEngine/system/InputToAccelerationSystem.cpp
    src/GNEngine/system/PlayerAnimationControlSystem.cpp
    src/GNEngine/system/AccelerationResetSystem.cpp

    src/GNEngine/component/AnimationComponent.cpp
    src/GNEngine/component/SoundComponent.cpp
)

# GNEngine 라이브러리 인클루드 디렉터리 설정
target_include_directories(GNEngine PUBLIC
    "${GENERATED_DIR}"
    "${CMAKE_SOURCE_DIR}/include"
    "${SDL3_INCLUDE_DIR}"
    "${SDL3_image_INCLUDE_DIR}"
    "${SDL3_ttf_INCLUDE_DIR}"
    "${OPENAL_DIR}/include"
    "${CMAKE_SOURCE_DIR}/lib"
    "${CMAKE_SOURCE_DIR}/lib/dr_libs"
    "${CMAKE_SOURCE_DIR}/lib/nlohmann"
    "${CMAKE_SOURCE_DIR}/lib/box2d-3.1.1/include"
    
    "${CMAKE_SOURCE_DIR}/include/GNEngine/flatbuffers_generated"
    "${CMAKE_SOURCE_DIR}/src/GNEngine/component"
    "${CMAKE_SOURCE_DIR}/src/GNEngine/core"
    "${CMAKE_SOURCE_DIR}/src/GNEngine/manager"
    "${CMAKE_SOURCE_DIR}/src/GNEngine/prefab"
    "${CMAKE_SOURCE_DIR}/src/GNEngine/resource"
    "${CMAKE_SOURCE_DIR}/src/GNEngine/system"
)

# GNEngine 라이브러리 링크 설정
target_link_libraries(GNEngine PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    "${OPENAL_DIR}/libs/Win64/OpenAL32.lib"
    stdc++fs
    box2d
)

# GNEngine_EXPORTS 정의 (DLL 빌드 시 필요)
target_compile_definitions(GNEngine PRIVATE GNEngine_EXPORTS)

# Force include <cstring> for compatibility with some libraries
target_compile_options(GNEngine PRIVATE -include cstring)

# DLL 복사
add_custom_command(TARGET GNEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL3_DLL_DIR}/SDL3.dll"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL3_image_DLL_DIR}/SDL3_image.dll"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL3_ttf_DLL_DIR}/SDL3_ttf.dll"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${OPENAL_DIR}/bin/Win64/soft_oal.dll"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/OpenAL32.dll"
)


# 애플리케이션 디렉터리 추가
add_subdirectory(example)

# Remove target_link_directories as it might not be needed with direct target linking
# target_link_directories(GNEngine PUBLIC ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})


