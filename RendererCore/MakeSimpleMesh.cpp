#include "pch.h"
#include "RendererData.h"
#include "RendererMath.h"

namespace renderer
{

	Mesh* MakeCubeMesh(Vector3 rightTop /*= Vector3(1, 1, 1)*/, Vector3 leftDown /*= Vector3(-1, -1, -1)*/)
	{
		Mesh* mesh = new Mesh;
		mesh->_vertexBufferSize = 8;
		mesh->_vertexBuffer = new Vertex[mesh->_vertexBufferSize]
		{
			{rightTop._x, rightTop._y, rightTop._z},
			{rightTop._x, rightTop._y, leftDown._z},
			{rightTop._x, leftDown._y, rightTop._z},
			{rightTop._x, leftDown._y, leftDown._z},
			{leftDown._x, rightTop._y, rightTop._z},
			{leftDown._x, rightTop._y, leftDown._z},
			{leftDown._x, leftDown._y, rightTop._z},
			{leftDown._x, leftDown._y, leftDown._z}
		};
		mesh->_faceBufferSize = 12;
		mesh->_faceBuffer = new Face[mesh->_faceBufferSize]
		{
			{0, 2, 1},
			{1, 2, 3},
			{0, 1, 4},
			{1, 5, 4},
			{0, 4, 2},
			{2, 4, 6},
			{4, 5, 6},
			{5, 7, 6},
			{2, 6, 3},
			{7, 3, 6},
			{1, 3, 5},
			{3, 7, 5}
		};

		return mesh;
	}

	Mesh* MakeTriangleMesh()
	{
		Mesh* mesh = new Mesh;
		mesh->_vertexBufferSize = 3;
		mesh->_vertexBuffer = new Vertex[mesh->_vertexBufferSize]
		{
			{0, 0, 0},
			{1, 0, 0},
			{0, 1, 0}
		};
		mesh->_faceBufferSize = 1;
		mesh->_faceBuffer = new Face[mesh->_faceBufferSize]
		{
			{0, 1, 2}
		};

		renderer::Vector4 v1 = mesh->_vertexBuffer[1]._pos - mesh->_vertexBuffer[0]._pos;
		renderer::Vector4 v2 = mesh->_vertexBuffer[2]._pos - mesh->_vertexBuffer[0]._pos;

		return mesh;
	}

}