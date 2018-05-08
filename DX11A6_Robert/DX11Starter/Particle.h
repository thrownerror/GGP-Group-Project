#pragma once

#include <DirectXMath.h>

struct Particle {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 StartVel;
	float Size;
	float Age;
};

struct ParticleVertex {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT4 Color;
	float Size;
};