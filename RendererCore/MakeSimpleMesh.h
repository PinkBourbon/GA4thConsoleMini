#pragma once

#include "RendererMath.h"

struct Mesh;

namespace renderer
{
	Mesh* MakeCubeMesh(Vector3 rightTop = Vector3{1, 1, 1}, Vector3 leftDown = Vector3{-1, -1, -1});
	Mesh* MakeTriangleMesh();
}
