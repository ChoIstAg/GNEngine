file(GLOB ENGINE_SOURCES "../include/engine/*.cpp")

add_executable(Noma-s_home 
    main.cpp

    MainCallback.cpp

    # ${ENGINE_SOURCES} <- 주석처리 사유: 이것을 활성화시키면 engine의 모든 것이 다 링크되므로 모든 파일에서 에러가 발생하지 않아야만 컴파일 오류가 발생 안함.
    ../include/engine/Application.cpp
    ../include/engine/Texture.cpp
    ../include/engine/TextureManager.cpp
    ../include/engine/InputManager.cpp
)

# Copy SDL3.dll to the output directory after building the target
# This is necessary for the executable to run, as it depends on SDL3.dll
add_custom_command(TARGET Noma-s_home POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different

    "${CMAKE_SOURCE_DIR}/include/lib/SDL3/x86_64-w64-mingw32/bin/SDL3.dll"
    "${CMAKE_SOURCE_DIR}/include/lib/SDL3_image/x86_64-w64-mingw32/bin/SDL3_image.dll"
    "${CMAKE_SOURCE_DIR}/include/lib/SDL3_ttf/x86_64-w64-mingw32/bin/SDL3_ttf.dll"

    $<TARGET_FILE_DIR:Noma-s_home>
)