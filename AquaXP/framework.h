#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#ifdef AQUAXP_EXPORTS
#define AQUAXP_API __declspec(dllexport)
#else
#define AQUAXP_API __declspec(dllimport)
#endif