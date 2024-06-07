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

		void AffineTransformation();						// ���� -> ���� ��ǥ�� ��ȯ
		void ViewTransformation(const Camera& camera);		// �� ��ǥ�� ��ȯ
		void ProjectionTransformation(const Camera& camera);	// ���� ��ȯ, ���� ������
		void Rasterisation();								// ���� ȭ�鿡 ��� ��ġ�� ����� ��ǥ�� ����
		void Fragment();									// �ؽ��İ� ������� � ���� ����� ������
		void Shader(const Light& light);										// �� � ���� ���� ������ ������.

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