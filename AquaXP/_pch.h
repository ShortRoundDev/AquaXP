// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

//STL
#include <vector>
#include <map>
#include <optional>
#include <string>
#include <memory>
#include <stack>

//DirectX
#include "d3d11.h"
#include "DirectXMath.h"
#include <DirectXTK/WICTextureLoader.h>
//Windows
#include "wrl.h"
#include <wrl/client.h>

//Custom
#include "Types.h"
#include "Slots.h"

#endif //PCH_H
