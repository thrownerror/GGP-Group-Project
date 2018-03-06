#include "Camera.h"



Camera::Camera()
{
	camPosition = XMFLOAT3 (0.0, 0.0, -2.0);
	camDirection = XMFLOAT3(0.0, 0.0, 1.0);
	camRotateX = 0;
	camRotateY = 0;
	speed = 1;
	mouseSensitivity = .01f;
	fieldOfView = 0.25f;

}


Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{



	XMVECTOR curPos = XMLoadFloat3(&camPosition);
	XMVECTOR curDir = XMLoadFloat3(&camDirection);
	XMVECTOR forward = XMLoadFloat3(&forwardZ);
	XMVECTOR up = XMLoadFloat3(&upY);
	XMVECTOR right = XMVector3Cross(curDir, up);

	

	float radianRotateX = (camRotateX * XM_PI) / 180;
	float radianRotateY = (camRotateY * XM_PI) / 180;


	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		curPos = XMVectorAdd(up * (speed*deltaTime), curPos);
	}
	if (GetAsyncKeyState('X') & 0x8000) {
		curPos = XMVectorAdd(up * -(speed*deltaTime), curPos);
	}
	if (GetAsyncKeyState('W') & 0x8000) {
		curPos = XMVectorAdd(curDir * (speed *deltaTime), curPos);
	//	camDirection.x, camDirection.y, camDirection.z *= speed * deltaTime;
	//	camPosition.x += camDirection.x;
	//	camPosition.y += camDirection.y;
	//	camPosition.z += camDirection.z;
		//camDirection.y *= speed;
		//camDirection.z *= speed;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		curPos = XMVectorAdd(curDir * -(speed *deltaTime), curPos);
		//camDirection
		//XMVECTOR cross = XMVector3Cross(curDir, up);
		//XMFLOAT3 = 
		//camPosition.x += -camDirection.x;
		//camPosition.y += -camDirection.y;
		//camPosition.z += -camDirection.z;
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		curPos = XMVectorAdd(XMVector3Cross(curDir, up) * (speed*deltaTime), curPos);
		//camDirection.x, camDirection.y, camDirection.z *= speed * deltaTime;
		//camPosition.x += -camDirection.x;
		//camPosition.y += -camDirection.y;
		//camPosition.z += -camDirection.z;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		curPos = XMVectorAdd(XMVector3Cross(curDir, up) * -(speed*deltaTime), curPos);
	}

	XMVECTOR quaternionResult = XMQuaternionRotationRollPitchYaw(camRotateX, camRotateY, 0);

	XMVECTOR resultantDirection = XMVector3Rotate(forward, quaternionResult);

	XMMATRIX resultant = XMMatrixLookToLH(curPos, resultantDirection, up);

	XMStoreFloat3(&camPosition, curPos);
	XMStoreFloat3(&camDirection, resultantDirection);
	XMStoreFloat4x4(&camMatrix, XMMatrixTranspose(resultant));




}

XMFLOAT4X4 Camera::GetCamMatrix()
{
	return camMatrix;
}

void Camera::MouseRotateX(float mouseDelta)
{
	camRotateY += mouseDelta * mouseSensitivity; //* 2;
	//printf("Rotate y: %f \n", camRotateY);
}

void Camera::MouseRotateY(float mouseDelta)
{
	camRotateX -= mouseDelta * mouseSensitivity; // *2;
	if (camRotateX > -2) {
		camRotateX = -2;
	}
	if (camRotateX < -4) {
		camRotateX = -4;
	}
	//printf("Rotate x: %f \n", camRotateX);
}

void Camera::UpdateProjectionMatrix(float width, float height)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		fieldOfView * XM_PI,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}
