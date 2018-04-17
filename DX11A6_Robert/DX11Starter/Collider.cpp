#include "Collider.h"



Collider::Collider()
{
	position = XMFLOAT3(0, 0, 0);
	xWidth = 1;
	yHeight = 1;
	zDepth = 1;
	
	EstablishBoundingVectors();
}

Collider::Collider(XMFLOAT3 pos)
{
	position = pos;
	xWidth = 1;
	yHeight = 1;
	zDepth = 1;

	EstablishBoundingVectors();

}

Collider::Collider(float xWidthRadius, float yHeightRadius, float zDepthRadius, XMFLOAT3 pos)
{
	position = pos;
	xWidth = xWidthRadius;
	yHeight = yHeightRadius;
	zDepth = zDepthRadius;

	EstablishBoundingVectors();
}

Collider::~Collider()
{
	delete topLeftFront;
	delete topRightFront;
	delete bottomLeftFront;
	delete bottomRightFront;
	delete topLeftBack;
	delete topRightBack;
	delete bottomLeftBack;
	delete bottomRightBack;
}

float Collider::GetColliderXWidth()
{
	return xWidth;
}

float Collider::GetColliderYHeight() 
{
	return yHeight;
}

float Collider::GetColliderZDepth() 
{
	return zDepth;
}

XMFLOAT3 Collider::GetColliderPos() 
{
	return position;
}

void Collider::SetColliderXWidth(float xWidthRadius) 
{
	xWidth = xWidthRadius;
}

void Collider::SetColliderYHeight(float yHeightRadius) 
{
	yHeight = yHeightRadius;
}

void Collider::SetColliderZDepth(float zDepthRadius) 
{
	zDepth = zDepthRadius;
}

void Collider::SetColliderPosition(XMFLOAT3 collPos)
{
	position = collPos;
	EstablishBoundingVectors();
}

XMFLOAT3** Collider::GetColliderCorners()
{
	XMFLOAT3** cornerArray = (XMFLOAT3**) malloc(sizeof(XMFLOAT3*) * 8); // XMFLOAT3[8];//= [topLeftFront, topRightfront, bottomLeftFront, bottomRightFront, topLeftBack, topRightBack, bottomLeftBack, bottomRightBack];
	cornerArray[0] = topLeftFront;
	cornerArray[1] = topRightFront;
	cornerArray[2] = bottomLeftFront;
	cornerArray[3] = bottomRightFront;
	cornerArray[4] = topLeftBack;
	cornerArray[5] = topRightBack;
	cornerArray[6] = bottomLeftBack;
	cornerArray[7] = bottomRightBack;
	//cornerArray[2]

	return cornerArray;//XMFLOAT3* = [topLeftFront, topRightfront, bottomLeftFront, bottomRightFront, topLeftBack, topRightBack, bottomLeftBack, bottomRightBack];
}

void Collider::EstablishBoundingVectors()
{
	topLeftFront = new XMFLOAT3(position.x - xWidth, position.y + yHeight, position.z + zDepth);
	topRightFront = new XMFLOAT3(position.x + xWidth, position.y + yHeight, position.z + zDepth);
	bottomLeftFront = new XMFLOAT3(position.x - xWidth, position.y - yHeight, position.z + zDepth);
	bottomRightFront = new XMFLOAT3(position.x + xWidth, position.y - yHeight, position.z + zDepth);

	topLeftBack = new XMFLOAT3(position.x - xWidth, position.y + yHeight, position.z - zDepth);
	topRightBack = new XMFLOAT3(position.x + xWidth, position.y + yHeight, position.z - zDepth);
	bottomLeftBack = new XMFLOAT3(position.x - xWidth, position.y - yHeight, position.z - zDepth);
	bottomRightBack = new XMFLOAT3(position.x + xWidth, position.y - yHeight, position.z - zDepth);
}

void Collider::ManipulateCollisionBox(XMFLOAT4X4* matrix)
{
//	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR tLF = XMLoadFloat3(topLeftFront);
	XMVECTOR tRF = XMLoadFloat3(topRightFront);
	XMVECTOR bLF = XMLoadFloat3(bottomLeftFront);
	XMVECTOR bRF = XMLoadFloat3(bottomRightFront);

	XMVECTOR tLB = XMLoadFloat3(topLeftBack);
	XMVECTOR tRB = XMLoadFloat3(topRightBack);
	XMVECTOR bLB = XMLoadFloat3(bottomLeftBack);
	XMVECTOR bRB = XMLoadFloat3(bottomRightBack);

	XMMATRIX loadedMatrix = XMLoadFloat4x4(matrix);

	tLF = XMVector4Transform(tLF, loadedMatrix);
	tRF = XMVector4Transform(tRF, loadedMatrix);
	bLF = XMVector4Transform(bLF, loadedMatrix);
	bRF = XMVector4Transform(bRF, loadedMatrix);
	tLB = XMVector4Transform(tLB, loadedMatrix);
	tRB = XMVector4Transform(tRB, loadedMatrix);
	bLB = XMVector4Transform(bLB, loadedMatrix);
	bRB = XMVector4Transform(bRB, loadedMatrix);

	//topLeftFront = XMStoreFloat3()
	XMStoreFloat3(topLeftFront,		tLF);
	XMStoreFloat3(topRightFront,	tRF);
	XMStoreFloat3(bottomLeftFront,	bLF);
	XMStoreFloat3(bottomRightFront, bRF);
	XMStoreFloat3(topLeftBack,		tRB);
	XMStoreFloat3(topRightBack,		tRB);
	XMStoreFloat3(bottomLeftBack,	bLB);
	XMStoreFloat3(bottomRightBack,	bRB);



}
