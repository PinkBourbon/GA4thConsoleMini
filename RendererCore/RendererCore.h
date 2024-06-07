#pragma once
#ifndef RENDERERCORE_H
#define RENDERERCORE_H

#include "Type.h"
#include "DoubleLinkedList.h"
#include "RendererData.h"
#include "FacePool.h"


struct RenderableObject;
struct Camera;
class Light;

namespace renderer
{
	class Renderer
	{
	public:
		Renderer(uint32 screenW = 198, uint32 screenH = 108);
		~Renderer();
		bool AddObject(RenderableObject* object);
		bool DeleteObejct(RenderableObject& object);

		void AffineTransformation();						// 로컬 -> 월드 좌표로 변환
		void ViewTransformation(const Camera& camera);		// 뷰 좌표로 변환
		void ProjectionTransformation(const Camera& camera);	// 투영 변환, 원근 나눗셈
		void Rasterisation();								// 실제 화면에 어느 위치에 찍는지 좌표가 나옴
		void Fragment();									// 텍스쳐가 있을경우 어떤 색을 찍는지 결정함
		void Shader(const Light& light);										// 빛 등에 따라서 최종 색상이 결정됨.

		int GetScreenW();
		int GetScreenH();
		void ChangeScreenSize(int32 w, int32 h);

		void DrawToDot(RenderFace& face);
		void DrawToLine(RenderFace& face);
		void DrawToMesh(RenderFace& face);

	private:
		void NewScreenBuffer();
		void DeleteScreenBuffer();

		void NearFrustumClipping(RenderableObject& item);

		uint32 GetClipCount(const Face& face);
		void MakeClipFace(const Face& face);

		void PutScreenBuffer(int x, int y, Pixel& pixel);
		void DrawLine(RenderFace face, int index0, int index1);

	private:
		DoubleLinkedList<RenderableObject*> _objectList;
		FacePool _facePool;
		double _screenW;
		double _screenH;

	public:
		Pixel** _screenBuffer;
	};
}
#endif // RENDERERCORE_H