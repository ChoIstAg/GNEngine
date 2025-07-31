add_library(engine_objects
    Object.h
    TestObject.h
    TestObject.cpp
    TextObject.h
    TextObject.cpp
)

target_include_directories(engine_objects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
