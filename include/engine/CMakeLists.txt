add_executable(GNEngine
    Texture.cpp
    TextureManager.cpp
    Animation.cpp
    AnimationManager.cpp
    Text.cpp
    TextManager.cpp

    Application.cpp
    #RenderManager.cpp
    InputManager.cpp

    #Scene.cpp
    #SceneManager.cpp
)

add_library(GNEngine
    Texture.cpp
    TextureManager.cpp
    Animation.cpp
    AnimationManager.cpp

    TextObj.cpp
    TextManager.cpp

    SceneManager.cpp
)