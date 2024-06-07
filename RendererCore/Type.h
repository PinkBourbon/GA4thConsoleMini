#pragma once
#ifndef TYPE_H
#define TYPE_H

#include <memory>
#include "Const.h"
#include "RendererMath.h"


namespace renderer
{
	using BYTE = unsigned char;
	using int8 = __int8;
	using int16 = __int16;
	using int32 = __int32;
	using int64 = __int64;
	using uint8 = unsigned __int8;
	using uint16 = unsigned __int16;
	using uint32 = unsigned __int32;
	using uint64 = unsigned __int64;

	struct RGB
	{
		unsigned char _r;
		unsigned char _g;
		unsigned char _b;
		unsigned char _a;
	};

	struct Vector3;
	struct Vector4;
	struct Metrix44;

}
#endif // TYPE_H