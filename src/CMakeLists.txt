add_executable(Noma-s_home 
    main.cpp
    ../include/TextureObj.cpp
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