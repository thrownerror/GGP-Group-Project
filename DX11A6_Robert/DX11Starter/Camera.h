#pragma once
#include <d3d11.h>
#include "DXCore.h"
#include "Vertex.h"
#include "DirectXMath.h"
#include "Mesh.h"
using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();
	void Update(float deltaTime);
	XMFLOAT4X4 GetCamMatrix();
	void MouseRotateX(float mouseDelta);
	void MouseRotateY(float mouseDelta);
	void UpdateProjectionMatrix(float width, float height);
	XMFLOAT4X4 GetProjectionMatrix();
	XMFLOAT3 GetPosition();
private:
	
	XMFLOAT3 camPosition;
	XMFLOAT3 camDirection;
	XMFLOAT4X4 camMatrix;
	XMFLOAT4X4 projectionMatrix;
	float fieldOfView;
	float mouseSensitivity;
	float speed;
	float camRotateX;
	float camRotateY;

	XMFLOAT3 forwardZ =	 XMFLOAT3(0.0, 0.0, 1.0);
	XMFLOAT3 upY	  =	 XMFLOAT3(0.0, 1.0, 0.0);
};

