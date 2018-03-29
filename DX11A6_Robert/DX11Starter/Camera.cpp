#include "Camera.h"



Camera::Camera()
{
	camPosition = XMFLOAT3 (0.0, 0.0, 0.0);
	camDirection = XMFLOAT3(0.0, 0.0, 1.0);
	camRotateX = 0;
	camRotateY = 0;
	speed = .25;
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

	// Strafing up and down
	if (GetAsyncKeyState('W') & 0x8000) {
		curPos = XMVectorAdd(up * (deltaTime), curPos);
		// Old code for moving forward
		//curPos = XMVectorAdd(curDir * (speed *deltaTime), curPos);
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		curPos = XMVectorAdd(up * -(deltaTime), curPos);
		// Old code for moving backwards
		//curPos = XMVectorAdd(curDir * -(speed *deltaTime), curPos);
	}
	// Rotating left and right
	if (GetAsyncKeyState('Q') & 0x8000) {
		camRotateY -= deltaTime;
	}
	if (GetAsyncKeyState('E') & 0x8000) {
		camRotateY += deltaTime;
	}
	// Strafing left and right
	if (GetAsyncKeyState('A') & 0x8000) {
		curPos = XMVectorAdd(XMVector3Cross(curDir, up) * (deltaTime), curPos);
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		curPos = XMVectorAdd(XMVector3Cross(curDir, up) * -(deltaTime), curPos);
	}
	if (GetAsyncKeyState(VK_LSHIFT) || GetAsyncKeyState(VK_RSHIFT) & 0x8000) {
		speed += 0.001f;
		if (speed > 3) speed = 3.0f;
	}
	if (GetAsyncKeyState(VK_LCONTROL) || GetAsyncKeyState(VK_RCONTROL) & 0x8000) {
		speed -= 0.001f;
		if (speed < 0) speed = 0;
	}

	// Move the camera forward -> movement speed values modified in 
	curPos = XMVectorAdd(curDir * (speed *deltaTime), curPos);

	XMVECTOR quaternionResult = XMQuaternionRotationRollPitchYaw(camRotateX, camRotateY, 0);

	XMVECTOR resultantDirection = XMVector3Rotate(forward, quaternionResult);

	XMMATRIX resultant = XMMatrixLookToLH(curPos, resultantDirection, up);

	XMStoreFloat3(&camPosition, curPos);
	//printf("Camera position on input: %d %d %d \n", curPos.x, camPosition.y, camPosition.z);
	XMStoreFloat3(&camDirection, resultantDirection);
	XMStoreFloat4x4(&camMatrix, XMMatrixTranspose(resultant));


}

XMFLOAT4X4 Camera::GetCamMatrix()
{
	return camMatrix;
}

void Camera::MouseRotateX(float mouseDelta)
{
	//camRotateY += mouseDelta * mouseSensitivity; //* 2;
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

XMFLOAT3 Camera::GetPosition() 
{
	//printf("in get pos %d\n", camPosition.x);
	return camPosition;
}
