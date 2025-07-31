add_library(engine_components
    EventListenerComponent.h
    TransformComponent.h
    SoundComponent.h
    SoundComponent.cpp
    RenderComponent.h
    RenderComponent.cpp
    TextComponent.h
    TextComponent.cpp
)

target_include_directories(engine_components PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})