cmake_minimum_required(VERSION 3.25)

# 애플리케이션 소스 파일
set(APP_SOURCES
    Application.cpp
    main.cpp
    scene/MainMenuScene.cpp
    scene/TestScene.cpp
    scene/LogoScene.cpp
)

# 애플리케이션 실행 파일 정의
add_executable(GNEngineApp ${APP_SOURCES})

# 애플리케이션 인클루드 디렉터리 설정
target_include_directories(GNEngineApp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/app
)

# 애플리케이션 링크 라이브러리 설정
target_link_libraries(GNEngineApp PRIVATE GNEngine)