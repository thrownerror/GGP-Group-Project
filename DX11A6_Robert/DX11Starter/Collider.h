#pragma once
#include "Vertex.h"
#include "DirectXMath.h"
#include <d3d11.h>
#include <malloc.h>
using namespace DirectX;

class Collider
{
public:
	Collider();
	Collider(XMFLOAT3 pos);
	Collider(float xWidthRadius, float yHeightRadius, float zDepthRadius, XMFLOAT3 pos);
	~Collider();

	float GetColliderXWidth();
	float GetColliderYHeight();
	float GetColliderZDepth();

	void SetColliderXWidth(float xWidthRadius);
	void SetColliderYHeight(float yHeightRadius);
	void SetColliderZDepth(float zDepthRadius);

	void SetColliderPosition(XMFLOAT3 collPos);
	XMFLOAT3** GetColliderCorners();

	XMFLOAT3 GetColliderPos();

	void ManipulateCollisionBox(XMFLOAT4X4* matrix);
private:
	float xWidth;
	float yHeight;
	float zDepth;

	XMFLOAT3 position;
	/*
		Effectively, two sqaures of vertices on a cube, Front and Back
	*/
	//Front Square
	XMFLOAT3* topLeftFront;
	XMFLOAT3* topRightFront;
	XMFLOAT3* bottomLeftFront;
	XMFLOAT3* bottomRightFront;
	//Back Square
	XMFLOAT3* topLeftBack;
	XMFLOAT3* topRightBack;
	XMFLOAT3* bottomLeftBack;
	XMFLOAT3* bottomRightBack;

	XMFLOAT3** cornerArray;

	void EstablishBoundingVectors();
};

