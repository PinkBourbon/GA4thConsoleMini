#include "pch.h"
#include "FacePool.h"
#include "RendererData.h"
#include <malloc.h>

namespace renderer
{
	FacePool::FacePool(unsigned __int32 poolNum/*= 100000*/)
		: _capacity(poolNum)
	{
		_head = (RenderFace*)malloc(sizeof(RenderFace) * _capacity);
	}

	RenderFace* FacePool::newFace()
	{
		RenderFace* ret = _head + _size;
		++_size;
		return ret;
	}
}