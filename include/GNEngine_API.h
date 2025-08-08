#pragma once

#ifdef _WIN32
    #ifdef GNEngine_EXPORTS
        #define GNEngine_API __declspec(dllexport)
    #else
        #define GNEngine_API __declspec(dllimport)
    #endif
#else
    #define GNEngine_API
#endif