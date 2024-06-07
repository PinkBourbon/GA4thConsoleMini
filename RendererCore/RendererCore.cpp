// RendererCore.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "RendererCore.h"
#include "RendererMath.h"
#include "RendererData.h"

// TODO: 라이브러리 함수의 예제입니다.
namespace renderer
{

	Renderer::Renderer(uint32 screenW /*= 1980*/, uint32 screenH /*= 1080*/)
		:_screenW(screenW), _screenH(screenH)
	{
		NewScreenBuffer();
		RenderFace* face = (RenderFace*)malloc(sizeof(RenderFace) * 10000);
	}

	Renderer::~Renderer()
	{
		auto iter = _objectList.begin();
		while (iter != _objectList.end())
		{
			iter = _objectList.Erase(iter);
		}

		DeleteScreenBuffer();
	}

	bool Renderer::AddObject(RenderableObject* object)
	{
		_objectList.PushBack(object);
		return true;
	}

	bool Renderer::DeleteObejct(RenderableObject& object)
	{
		auto node = _objectList.Find(&object);
		_objectList.Erase(node);
		return true;
	}

	void Renderer::AffineTransformation()
	{
		for (auto& item : _objectList)
		{
			Transform* tr = &((*item)._object->_transform);
			tr->_transformationMatrix = (tr->_position._data * (tr->_rotation._metrixR * tr->_scale._data));
		}
	}

	void Renderer::ViewTransformation(const Camera& camera)
	{
		Vector3 u = camera.GetU();
		Vector3 v = camera.GetV();
		Vector3 n = camera.GetN();
		Vector3 eye = camera.GetEye();
		Metrix44 t
		(
			u._x, u._y, u._z, -(Dot(eye, u)),
			v._x, v._y, v._z, -(Dot(eye, v)),
			n._x, n._y, n._z, -(Dot(eye, n)),
			0, 0, 0, 1
		);

		for (auto& item : _objectList)
		{
			Metrix44* transformationMetrix = &((*item)._object->_transform._transformationMatrix);
			*transformationMetrix = t * (*transformationMetrix);
		}
	}

	void Renderer::ProjectionTransformation(const Camera& camera)
	{
		double d = Cot(camera._fovY / 2);
		double a = camera._aspect;
		double divide = camera._f / (camera._f - camera._n);
		double n = camera._n;
		Metrix44 projectionMetrix
		(
			d / a, 0, 0, 0,
			0, d, 0, 0,
			0, 0, divide, n * divide,
			0, 0, -1, 0
		);

		_facePool._size = 0;
		for (auto& item : _objectList)
		{

			if (!item->_object->_mesh->isRender)
			{
				continue;
			}
			Metrix44* transformationMetrix = &((*item)._object->_transform._transformationMatrix);
			*transformationMetrix = projectionMetrix * (*transformationMetrix);

			uint32 faceBufferSize = item->_object->_mesh->_faceBufferSize;
			Face* faceBuffer = item->_object->_mesh->_faceBuffer;
			Vertex* vertexBuffer = item->_object->_mesh->_vertexBuffer;

			NearFrustumClipping(*item);
		}
	}

	void Renderer::Rasterisation()
	{
		Metrix44 viewPortMetrix
		(
			_screenW / 2, 0, 0, _screenW / 2,
			0, -(_screenH / 2), 0, _screenH / 2,
			0, 0, 1, 0,
			0, 0, 0, 1
		);

		//버퍼에 그리기전에 버퍼 초기화
		for (int y = 0; y < _screenH; ++y)
		{
			for (int x = 0; x < _screenW; ++x)
			{
				(_screenBuffer[y] + x)->_z = -2;
				(_screenBuffer[y] + x)->_lightIntensity = -1;
			}
		}

		RenderFace* face = _facePool._head;
		for (uint32 i = 0; i < _facePool._size; ++i)
		{
			RenderFace* face = _facePool._head + i;

			for (int j = 0; j < 3; ++j)
			{
				face->_vertex[j]._pos = viewPortMetrix * face->_vertex[j]._pos;
			}
			
			Vector3 thisNomal = Cross
			(
				(face->_vertex[2]._pos - face->_vertex[0]._pos),
				(face->_vertex[1]._pos - face->_vertex[0]._pos)
			).Normalization();

			Vector3 linkVector = { 0, 0, 1 };

			if (Dot(thisNomal, linkVector) < 0)
			{
				face->_isDraw = false;
				continue;
			}
			face->_isDraw = true;

			switch (face->_scheme)
			{
				case outputScheme::dot:
					DrawToDot(*face);
					break;
				case outputScheme::line:
					DrawToLine(*face);
					break;
				case outputScheme::mesh:
					DrawToMesh(*face);
					break;
				default:
					break;
			}
		}
	}

	void Renderer::Fragment()
	{

	}

	void Renderer::Shader(const Light& light)
	{
		for (int y = 0; y < _screenH; ++y)
		{
			for (int x = 0; x < _screenW; ++x)
			{
				if ((_screenBuffer[y] + x)->_z == -2)
				{
					continue;
				}
				//double intensity = Dot(light._direction, (_screenBuffer[y] + x)->_nomal);
				//(_screenBuffer[y] + x)->_lightIntensity = intensity;
			}
		}
	}

	int Renderer::GetScreenW()
	{
		return (int)_screenW;
	}

	int Renderer::GetScreenH()
	{
		return (int)_screenH;
	}

	void Renderer::ChangeScreenSize(int32 w, int32 h)
	{
		DeleteScreenBuffer();
		_screenW = w;
		_screenH = h;
		NewScreenBuffer();
	}

	void Renderer::DrawToDot(RenderFace& face)
	{
		Pixel pixel;
		for (int i = 0; i < 3; ++i)
		{
			int x = Round(face._vertex[i]._pos._x);
			int y = Round(face._vertex[i]._pos._y);
			pixel._z = face._vertex[i]._pos._z;

			PutScreenBuffer(x, y, pixel);
		}
	}

	void Renderer::DrawToLine(RenderFace& face)
	{
		if (face._isDraw == false)
		{
			return;
		}

		DrawLine(face, 0, 1);

		DrawLine(face, 1, 2);

		DrawLine(face, 2, 0);
	}

	void Renderer::DrawToMesh(RenderFace& face)
	{
		if (face._isDraw == false)
		{
			return;
		}

		int minX = _screenW;
		int maxX = 0;
		int minY = _screenH;
		int maxY = 0;

		for (int i = 0; i < 3; ++i)
		{
			int x = Round(face._vertex[i]._pos._x);
			int y = Round(face._vertex[i]._pos._y);
			minX = x < minX ? x : minX;
			maxX = x > maxX ? x : maxX;
			minY = y < minY ? y : minY;
			maxY = y > maxY ? y : maxY;
		}
		maxY += 1;
		maxX += 1;

		minX = minX < 0 ? 0 : minX;
		maxX = maxX > _screenW ? _screenW : maxX;
		minY = minY < 0 ? 0 : minY;
		maxY = maxY > _screenH ? _screenH : maxY;

		double uX = face._vertex[1]._pos._x - face._vertex[0]._pos._x;
		double uY = face._vertex[1]._pos._y - face._vertex[0]._pos._y;
		double vX = face._vertex[2]._pos._x - face._vertex[0]._pos._x;
		double vY = face._vertex[2]._pos._y - face._vertex[0]._pos._y;
		double uDotv = uX * vX + uY * vY;
		double vDotv = vX * vX + vY * vY;
		double uDotu = uX * uX + uY * uY;
		float denominator = uDotv * uDotv - vDotv * uDotu;

		if (denominator == 0.0f)
		{
			return;
		}

		double invZ0 = 1.0 / face._vertex[0]._pos._z;
		double invZ1 = 1.0 / face._vertex[1]._pos._z;
		double invZ2 = 1.0 / face._vertex[2]._pos._z;

		Vector3 nomal = Cross((face._vertex[2]._pos - face._vertex[0]._pos), 
			(face._vertex[1]._pos - face._vertex[0]._pos));
		nomal.Normalization();

		for (int y = minY; y < maxY; ++y)
		{
			for (int x = minX; x < maxX; ++x)
			{
				//원근 보간 해야함.
				double wX = x - face._vertex[0]._pos._x;
				double wY = y - face._vertex[0]._pos._y;
				double wdotu = wX * uX + wY * uY;
				double wdotv = wX * vX + wY * vY;

				double s = (wdotv * uDotv - wdotu * vDotv) / denominator;
				double t = (wdotu * uDotv - wdotv * uDotu) / denominator;

				if (((s >= 0.0) && (s <= 1.0)) 
					&& ((t >= 0.0) && (t <= 1.0)) 
					&& (((1-s-t) >= 0.0) && ((1-s-t) <= 1.0)))
				{
					Pixel pixel;
					pixel._z = -0.5;
					pixel._nomal = nomal;

					PutScreenBuffer(x, y, pixel);
				}

			}
		}
	}

	void Renderer::NewScreenBuffer()
	{
		_screenBuffer = (Pixel**)malloc(sizeof(Pixel*) * _screenH);
		for (int i = 0; i < _screenH; ++i)
		{
			_screenBuffer[i] = (Pixel*)malloc(sizeof(Pixel) * _screenW);
		}
	}

	void Renderer::DeleteScreenBuffer()
	{
		for (int i = 0; i < _screenH; ++i)
		{
			free(_screenBuffer[i]) ;
		}
		free(_screenBuffer);
	}

	void Renderer::NearFrustumClipping(RenderableObject& item)
	{
		uint32 faceBufferSize = item._object->_mesh->_faceBufferSize;
		Face* faceBuffer = item._object->_mesh->_faceBuffer;
		Vertex* vertexBuffer = item._object->_mesh->_vertexBuffer;
		Metrix44* transformationMetrix = &(item._object->_transform._transformationMatrix);

		for (uint32 i = 0; i < faceBufferSize; ++i)
		{
			RenderFace* face = _facePool.newFace();
			face->_scheme = item._scheme;

			bool isIn[3] = { false, false, false };
			int outVertexCount = 0;
			//좌표 계산 및 삼각형 컬링.
			for (int j = 0; j < 3; ++j)
			{
				face->_vertex[j]._pos = *transformationMetrix * vertexBuffer[faceBuffer[i]._vertexIndex[j]]._pos;
				face->_w[j] = face->_vertex->_pos._w;

				Vector4& pos = face->_vertex[j]._pos;
				double w = pos._w;
				if (pos._z > 0)
				{
					isIn[j] = false;
					++outVertexCount;
					continue;
				}
				isIn[j] = true;
			}

			switch (outVertexCount)
			{
				case 0:
				{
					break;
				}
				case 1:
				{
					int outPosIndex = 0;
					if (isIn[0] == false)
					{
						outPosIndex = 0;
					}
					else if (isIn[1] == false)
					{
						outPosIndex = 1;
					}
					else
					{
						outPosIndex = 2;
					}

					Vector4& out = face->_vertex[outPosIndex]._pos;
					Vector4& in0 = face->_vertex[(outPosIndex + 1) % 3]._pos;
					Vector4& in1 = face->_vertex[(outPosIndex + 2) % 3]._pos;


					RenderFace* newFace = _facePool.newFace();
					*newFace = *face;

					double t = out._z / (out._z - in0._z);
					newFace->_vertex[outPosIndex]._pos = out * (1 - t) + in0 * t;
					newFace->_uv[outPosIndex] = newFace->_uv[outPosIndex] * (1 - t);
					newFace->_uv[outPosIndex] += face->_uv[(outPosIndex + 1) % 3] * t;

					face->_vertex[(outPosIndex + 1) % 3] = newFace->_vertex[outPosIndex];
					face->_uv[(outPosIndex + 1) % 3] = newFace->_uv[outPosIndex];
					//만들어진 face 원근나눗셈
					for (int j = 0; j < 3; ++j)
					{
						newFace->_vertex[j]._pos /= newFace->_vertex[j]._pos._w;
					}

					t = out._z / (out._z - in1._z);
					out = out * (1 - t) + in1 * t;
					face->_uv[outPosIndex] = face->_uv[outPosIndex] * (1 - t);
					face->_uv[outPosIndex] += face->_uv[(outPosIndex + 2) % 3] * t;
					break;
				}
				case 2:
				{
					int inPosIndex = 0;
					if (isIn[0] == true)
					{
						inPosIndex = 0;
					}
					else if (isIn[1] == true)
					{
						inPosIndex = 1;
					}
					else
					{
						inPosIndex = 2;
					}
					Vector4& in = face->_vertex[inPosIndex]._pos;
					Vector4& out0 = face->_vertex[(inPosIndex + 1) % 3]._pos;
					Vector4& out1 = face->_vertex[(inPosIndex + 2) % 3]._pos;

					double t = out0._z / (out0._z - in._z);
					out0 = out0 * (1 - t) + in * t;
					face->_uv[(inPosIndex + 1) % 3] = face->_uv[(inPosIndex + 1) % 3] * (1 - t);
					face->_uv[(inPosIndex + 1) % 3] += face->_uv[inPosIndex] * t;

					t = out1._z / (out1._z - in._z);
					out1 = out1 * (1 - t) + in * t;
					face->_uv[(inPosIndex + 2) % 3] = face->_uv[(inPosIndex + 2) % 3] * (1 - t);
					face->_uv[(inPosIndex + 2) % 3] += face->_uv[inPosIndex] * t;
					break;
				}
				default:
					continue;
			}

			for (int j = 0; j < 3; ++j)
			{
				face->_vertex[j]._pos /= face->_vertex[j]._pos._w;
			}
		}
	}

	void Renderer::PutScreenBuffer(int x, int y, Pixel& pixel)
	{
		if ((x < 0 || x >= _screenW) || (y < 0 || y >= _screenH))
		{
			return;
		}

		if (pixel._z > 1.0e-15)
		{
			return;
		}

		if ((_screenBuffer[y]+x)->_z < pixel._z)
		{
			*(_screenBuffer[y] + x) = pixel;
		}
	}

	void Renderer::DrawLine(RenderFace face, int index0, int index1)
	{
		Pixel pixel0;
		int x0 = Round(face._vertex[index0]._pos._x);
		int y0 = Round(face._vertex[index0]._pos._y);
		pixel0._z = face._vertex[index0]._pos._z;

		Pixel pixel1;
		int x1 = Round(face._vertex[index1]._pos._x);
		int y1 = Round(face._vertex[index1]._pos._y);
		pixel1._z = face._vertex[index1]._pos._z;

		if (x0 > x1)
		{
			return DrawLine(face, index1, index0);
		}

		int x = x0;
		int y = y0;
		int dx = x1 - x0;
		int dy = y1 - y0;
		int dirY = dy > 0 ? 1 : -1;
		dy *= dirY;
		int e = 0;
		Pixel pixel = pixel0;
		if (dy > dx)
		{
			
			while (y * dirY < y1 * dirY)
			{
				pixel._z =
					1.0 /
					(
						((double)(y1 - y) / (double)(y1 - y0)) / pixel0._z
						+ (1.0 - ((double)(y1 - y) / (double)(y1 - y0))) / pixel1._z
					);
				PutScreenBuffer(x, y, pixel);
				y += dirY;
				e += dx;
				if (e >= dx)
				{
					e -= dy;
					++x;
				}
			}
		}
		else
		{
			
			while (x < x1)
			{
				pixel._z =
					1.0 /
					(
						((double)(x1 - x) / (double)dx) / pixel0._z
						+ (1.0 - ((double)(x1 - x) / (double)dx)) / pixel1._z
					);
				PutScreenBuffer(x, y, pixel);
				++x;
				e += dy;
				if (e >= dx)
				{
					e -= dx;
					y += dirY;
				}
			}
		}

		PutScreenBuffer(x, y, pixel);
	}
}