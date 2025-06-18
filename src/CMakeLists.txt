add_executable(sdl3_test 
#main:
        
#sub:
    TextureObj.cpp
    TextureManager.cpp
    TextManager.cpp
)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

target_include_directories(sdl3_test
    PRIVATE
        "${CMAKE_SOURCE_DIR}/lib/SDL3/x86_64-w64-mingw32/include"
        "${CMAKE_SOURCE_DIR}/lib/SDL3_image/x86_64-w64-mingw32/include"
        "${CMAKE_SOURCE_DIR}/lib/SDL3_ttf/x86_64-w64-mingw32/include"
        
        "${CMAKE_SOURCE_DIR}/src/include/texture.hpp"
)

target_link_libraries(sdl3_test 
    PUBLIC 
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
)


# Copy SDL3.dll to the output directory after building the target
# This is necessary for the executable to run, as it depends on SDL3.dll
add_custom_command(TARGET sdl3_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different

    "${CMAKE_SOURCE_DIR}/lib/SDL3/x86_64-w64-mingw32/bin/SDL3.dll"
    "${CMAKE_SOURCE_DIR}/lib/SDL3_image/x86_64-w64-mingw32/bin/SDL3_image.dll"
    "${CMAKE_SOURCE_DIR}/lib/SDL3_ttf/x86_64-w64-mingw32/bin/SDL3_ttf.dll"
    
    #"${CMAKE_SOURCE_DIR}/asset/example_bmp.bmp"
    #"${CMAKE_SOURCE_DIR}/asset/example_png.png"

    $<TARGET_FILE_DIR:sdl3_test>)

#target_compile_definitions(god_maga21-43 PRIVATE PROJECT_ROOT_PATH=\"${CMAKE_SOURCE_DIR}\")