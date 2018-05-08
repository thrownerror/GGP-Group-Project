#pragma once

#include <d3d11.h>

#include "SimpleShader.h"
#include "Particle.h"
#include "Camera.h"

// Heavy parts of this borrow from the
// CPU Particle Demo. Credit to Chris
// where it's due.
class Emitter
{
	Particle* particles;
	ParticleVertex* localParticleVertices;

	int particlesPerSecond;
	float secondsPerParticle;
	float timeSinceEmit;

	int particleCount;
	float lifetime;

	DirectX::XMFLOAT3 accel;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 velocity;

	DirectX::XMFLOAT4 startColor;
	DirectX::XMFLOAT4 endColor;

	float startSize;
	float endSize;

	int maxParticles;
	int firstDeadIndex;
	int firstAliveIndex;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	ID3D11ShaderResourceView* texture;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

public:
	Emitter(
		int max,
		int pps,
		float lifetime,
		float start,
		float end,
		DirectX::XMFLOAT4 startCol,
		DirectX::XMFLOAT4 endCol,
		DirectX::XMFLOAT3 vel,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3 accel,
		ID3D11Device* device,
		SimpleVertexShader* vs,
		SimplePixelShader* ps,
		ID3D11ShaderResourceView* texture);
	Emitter();
	Emitter(Emitter &e, ID3D11Device * device);
	~Emitter();

	void Update(float deltaTime);
	void UpdateParticle(float deltaTime, int i);
	void Spawn();

	void SetPosition(XMFLOAT3 p);
	void SetVelocity(XMFLOAT3 v);
	void SetAcceleration(XMFLOAT3 a);

	void Dump(ID3D11DeviceContext* context);
	void DumpOne(int index);

	void Draw(ID3D11DeviceContext* context, Camera* camera);
};

