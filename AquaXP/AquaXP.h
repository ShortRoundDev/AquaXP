#pragma once

#include <stack>
#include <optional>
#include <vector>
#include <functional>
#include <set>
#include <variant>
#include <array>
#include <memory>

#include "framework.h"

#include "d3d11.h"
#include "wrl.h"
#include <wrl/client.h>
#include "DirectXMath.h"

#include "Types.h"
#include "Slots.h"
#include "Concepts.h"


#include "ICamera.h"
#include "NoclipCameraController.h"
#include "Application.h"
#include "Graphics.h"
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"
#include "CBuffer.h"
#include "Sampler.h"
#include "AABB.h"
#include "OctreeNode.h"

#include "RegisterAction.h"
#include "CollisionHelpers.h"
#include "ContextHelpers.h"