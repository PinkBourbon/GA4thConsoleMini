#include "pch.h"
#include "Light.h"

namespace renderer
{
	Light::Light()
	{
		_transform._position.Set(0, 20, 0);
		_color._a = 0;
		_color._r = 255;
		_color._g = 255;
		_color._b = 255;
	}
}
