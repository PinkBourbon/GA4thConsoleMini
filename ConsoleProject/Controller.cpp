#include "Controller.h"
#include <windows.h>
#include "RendererData.h"

void ProcessInput(renderer::Camera& camera)
{
	double moveSpeed = 0.1;
	if (GetAsyncKeyState(0x57/*W*/) & 0x8001)
	{
		camera._transform._position.DeltaSet(0, 0, -moveSpeed);
	}
	if (GetAsyncKeyState(0x53/*S*/) & 0x8001)
	{
		camera._transform._position.DeltaSet(0, 0, moveSpeed);
	}
	if (GetAsyncKeyState(0x44/*D*/) & 0x8001)
	{
		camera._transform._position.DeltaSet(moveSpeed, 0, 0);
	}
	if (GetAsyncKeyState(0x41/*A*/) & 0x8001)
	{
		camera._transform._position.DeltaSet(-moveSpeed, 0, 0);
	}
	if (GetAsyncKeyState(VK_SHIFT) & 0x8001)
	{
		camera._transform._position.DeltaSet(0, -moveSpeed, 0);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8001)
	{
		camera._transform._position.DeltaSet(0, moveSpeed, 0);
	}
}
