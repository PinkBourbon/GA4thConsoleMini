#pragma once

#include "pch.h"
#include <iostream>
#include <windows.h>

#include "Timer.h"
#include "RendererMath.h"
#include "Type.h"
#include "RendererCore.h"
#include "RendererData.h"
#include "ConsoleOutput.h"
#include "MakeSimpleMesh.h"
#include "Controller.h"
#include "Light.h"

struct ScreenFontSize
{
	int width;
	int height;
	int fontSize;
};

ScreenFontSize g_screenFontSize[4] = 
{ 
	{96 * 8, 54 * 8, 2}, 
	{96 * 4, 54 * 4, 4},
	{96 * 2, 54 * 2, 8},
	{96 * 1, 54 * 1, 16}
};

HANDLE g_hConsole;



//int g_screenWidth = 96 * 9;
//int g_screenHeight = 54 * 9;
constexpr int OBJECT_SIZE = 10;

const char* objectName[OBJECT_SIZE];
renderer::RenderableObject* renderObject[OBJECT_SIZE];
renderer::Object object[OBJECT_SIZE];

int main()
{
	int dotSize = 2;
	printf("도트 사이즈를 입력해주세요 (0 ~ 3) : ");
	std::cin >> dotSize;
	if (dotSize > 3)
	{
		dotSize = 3;
	}
	else if (dotSize < 0)
	{
		dotSize = 0;
	}

	ConsoleOUT console(g_screenFontSize[dotSize].width, g_screenFontSize[dotSize].height, g_screenFontSize[dotSize].fontSize);
	renderer::Renderer Myrenderer(g_screenFontSize[dotSize].width, g_screenFontSize[dotSize].height);

	objectName[0] = "cube.obj";
	objectName[1] = "sphere.obj";
	objectName[2] = "dounut.obj";
	objectName[3] = "cat.obj";
	objectName[4] = "monkey.obj";
	objectName[5] = "car.obj";
	objectName[6] = "spider.obj";
	objectName[7] = "pikachu.obj";
	objectName[8] = "gummy.obj";
	objectName[9] = "wolf.obj";

	for (int i = 0; i < OBJECT_SIZE; ++i)
	{
		renderer::Mesh* testMesh = new renderer::Mesh;
		testMesh->ImportObjFile(objectName[i]);
		object[i]._mesh = testMesh;
		object[i]._transform._position.Set(0, 0, 0);
		object[i]._transform._rotation.Set(0, 1, 0);
		
		renderObject[i] = new renderer::RenderableObject(object[i]);
		renderObject[i]->_scheme = renderer::outputScheme::mesh;
	}

	Myrenderer.AddObject(renderObject[0]);
	object[0]._transform._scale.Set(5, 5, 5);
	object[1]._transform._scale.Set(5, 5, 5);
	object[2]._transform._scale.Set(5, 5, 5);
	object[3]._transform._scale.Set(2, 2, 2);
	object[4]._transform._scale.Set(5, 5, 5);
	object[5]._transform._scale.Set(5, 5, 5);
	object[6]._transform._scale.Set(30, 30, 30);
	object[7]._transform._scale.Set(3, 3, 3);
	object[8]._transform._scale.Set(2, 2, 2);
	object[9]._transform._scale.Set(20, 20, 20);

	renderer::Camera camera((double)(g_screenFontSize[dotSize].width) / (double)(g_screenFontSize[dotSize].height));
	camera._transform._position.Set(0, 0, 30);
	camera._transform._rotation.Set(0, 0, 0);

	renderer::Light light;

	bool isEnd = false;
	bool isDrawAnswer = false;
	int index = 0;
	while (!isEnd)
	{
		double rotationSpeed = 1.5;
		if ((GetAsyncKeyState(0x57/*W*/) & 0x8001) || (GetAsyncKeyState(VK_UP) & 0x8001))
		{
			renderObject[index]->_object->_transform._rotation.DeltaSet(-rotationSpeed, 0, 0);
		}
		if ((GetAsyncKeyState(0x53/*S*/) & 0x8001) || (GetAsyncKeyState(VK_DOWN) & 0x8001))
		{
			renderObject[index]->_object->_transform._rotation.DeltaSet(rotationSpeed, 0, 0);
		}
		if ((GetAsyncKeyState(0x44/*D*/) & 0x8001) || (GetAsyncKeyState(VK_RIGHT) & 0x8001))
		{
			renderObject[index]->_object->_transform._rotation.DeltaSet(0, rotationSpeed, 0);
		}
		if ((GetAsyncKeyState(0x41/*A*/) & 0x8001) || (GetAsyncKeyState(VK_LEFT) & 0x8001))
		{
			renderObject[index]->_object->_transform._rotation.DeltaSet(0, -rotationSpeed, 0);
		}
		if (GetAsyncKeyState(0x45/*E*/) & 0x8001)
		{
			renderObject[index]->_object->_transform._rotation.DeltaSet(0, 0, rotationSpeed);
		}
		if (GetAsyncKeyState(0x51/*Q*/) & 0x8001)
		{
			renderObject[index]->_object->_transform._rotation.DeltaSet(0, 0, -rotationSpeed);
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x0001)
		{
			if (isDrawAnswer == false)
			{

				renderObject[index]->_scheme = renderer::outputScheme::line;
			}
			else
			{
				//다음 메쉬로 넘겨주기
				renderObject[index]->_scheme = renderer::outputScheme::mesh;
				renderObject[index]->_object->_transform._rotation.Set(0, 0, 0);
				Myrenderer.DeleteObejct(*renderObject[index]);
				index = (index + 1) % OBJECT_SIZE;
				Myrenderer.AddObject(renderObject[index]);
			}
			
			isDrawAnswer = !isDrawAnswer;
		}
		
		Myrenderer.AffineTransformation();
		Myrenderer.ViewTransformation(camera);
		Myrenderer.ProjectionTransformation(camera);
		Myrenderer.Rasterisation();

		//Myrenderer.Shader(light);

		
		if (isDrawAnswer)
		{
			console.DrawBackBuffer(Myrenderer._screenBuffer, Myrenderer.GetScreenW(), Myrenderer.GetScreenH());
		}
		else
		{
			console.DrawBackBufferWithNoLight(Myrenderer._screenBuffer, Myrenderer.GetScreenW(), Myrenderer.GetScreenH());
		}
		
		console.SwapBackBuffer();
	}
	

	return 0;
}