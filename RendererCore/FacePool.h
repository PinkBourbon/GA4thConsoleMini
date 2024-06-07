
#include "Type.h"

namespace renderer
{
	struct RenderFace;
}

namespace renderer
{
	class FacePool
	{
	public:
		FacePool(unsigned __int32 poolNum = 100000);
		RenderFace* newFace();

	private:
		const uint32 _capacity;
		uint32 _size = 0;
		RenderFace* _head;
		friend class Renderer;
	};
}

