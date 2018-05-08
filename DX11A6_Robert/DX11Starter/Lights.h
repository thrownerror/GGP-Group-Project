#pragma once
#include "GameEntity.h"
struct DirectionalLight
{
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
	XMFLOAT3 Direction;
};

struct PointLight
{
	XMFLOAT3 Position;
	XMFLOAT3 Color;
};