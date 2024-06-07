#pragma once
#include "Type.h"
#include "RendererData.h"

namespace renderer
{
	class Light
	{
	public:
		Light();
	public:
		Transform _transform;
		RGB _color;

		friend class Renderer;
	};

	class DirectLight : public Light
	{
		Vector3 _direction;
		double _lux;
		friend class Renderer;
	};

	class PointLight : public Light
	{
		double _lumen;
		friend class Renderer;
	};
}