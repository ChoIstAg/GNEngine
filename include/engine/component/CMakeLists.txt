add_library(engine_components
    EventListenerComponent.h
    TransformComponent.h
)

target_include_directories(engine_components PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})