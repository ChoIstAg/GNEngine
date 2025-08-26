#pragma once

/* 
 * 硫?고뵆?쏇뤌 吏?먯쓣 ?꾪븳 ?뚮젅洹몃뱾
*/

#ifdef _WIN32
    #ifdef GNEngine_EXPORTS
        #define GNEngine_API __declspec(dllexport)
    #else
        #define GNEngine_API __declspec(dllimport)
    #endif
#else
    #define GNEngine_API
#endif

