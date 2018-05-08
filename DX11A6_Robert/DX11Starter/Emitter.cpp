#include "Emitter.h"

Emitter::Emitter(int max, int pps, float lifetime, float start, float end, DirectX::XMFLOAT4 startCol, DirectX::XMFLOAT4 endCol, DirectX::XMFLOAT3 vel, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 accel, ID3D11Device * device, SimpleVertexShader * vs, SimplePixelShader * ps, ID3D11ShaderResourceView * texture)
{
	if (device != nullptr) {
		this->maxParticles = max;
		this->particlesPerSecond = pps;
		this->secondsPerParticle = 1.0f / particlesPerSecond;
		this->lifetime = lifetime;
		this->startSize = start;
		this->endSize = end;
		this->startColor = startCol;
		this->endColor = endCol;
		this->velocity = vel;
		this->position = pos;
		this->accel = accel;
		this->vertexShader = vs;
		this->pixelShader = ps;
		this->texture = texture;

		timeSinceEmit = 0;
		particleCount = 0;
		firstAliveIndex = 0;
		firstDeadIndex = 0;

		particles = new Particle[maxParticles];
		localParticleVertices = new ParticleVertex[4 * maxParticles];
		for (int i = 0; i < maxParticles * 4; i += 4)
		{
			localParticleVertices[i + 0].UV = XMFLOAT2(0, 0);
			localParticleVertices[i + 1].UV = XMFLOAT2(1, 0);
			localParticleVertices[i + 2].UV = XMFLOAT2(1, 1);
			localParticleVertices[i + 3].UV = XMFLOAT2(0, 1);
		}

		// Create buffers for drawing particles

		// DYNAMIC vertex buffer (no initial data necessary)
		D3D11_BUFFER_DESC vbDesc = {};
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		vbDesc.ByteWidth = sizeof(ParticleVertex) * 4 * maxParticles;
		device->CreateBuffer(&vbDesc, 0, &vertexBuffer);

		// Index buffer data
		unsigned int* indices = new unsigned int[maxParticles * 6];
		int indexCount = 0;
		for (int i = 0; i < maxParticles * 4; i += 4)
		{
			indices[indexCount++] = i;
			indices[indexCount++] = i + 1;
			indices[indexCount++] = i + 2;
			indices[indexCount++] = i;
			indices[indexCount++] = i + 2;
			indices[indexCount++] = i + 3;
		}
		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = indices;

		// Regular index buffer
		D3D11_BUFFER_DESC ibDesc = {};
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.ByteWidth = sizeof(unsigned int) * maxParticles * 6;
		device->CreateBuffer(&ibDesc, &indexData, &indexBuffer);

		delete[] indices;
	}
	else {
		particles = new Particle[1];
		localParticleVertices = new ParticleVertex[1];
		vertexShader = nullptr;
	}
}

Emitter::Emitter() : Emitter(0, 0, 0, 0, 0, XMFLOAT4(), XMFLOAT4(), XMFLOAT3(), XMFLOAT3(), XMFLOAT3(), nullptr, nullptr, nullptr, nullptr)
{

}

Emitter::~Emitter()
{
	delete[] particles;
	delete[] localParticleVertices;
	if (vertexShader != nullptr)
	{
		vertexBuffer->Release();
		indexBuffer->Release();
	}
}

void Emitter::Update(float deltaTime)
{
	if (firstAliveIndex < firstDeadIndex) {
		for (int i = firstAliveIndex; i < firstDeadIndex; i++) UpdateParticle(deltaTime, i);
	}
	else {
		for (int i = firstAliveIndex; i < maxParticles; i++) UpdateParticle(deltaTime, i);
		for (int i = 0; i < firstDeadIndex; i++) UpdateParticle(deltaTime, i);
	}

	timeSinceEmit += deltaTime;
	while (timeSinceEmit > secondsPerParticle) {
		Spawn();
		timeSinceEmit -= secondsPerParticle;
	}
}

void Emitter::UpdateParticle(float deltaTime, int i)
{
	// Check for valid particle age before doing anything
	if (particles[i].Age >= lifetime)
		return;

	// Update and check for death
	particles[i].Age += deltaTime;
	if (particles[i].Age >= lifetime)
	{
		// Recent death, so retire by moving alive count
		firstAliveIndex++;
		firstAliveIndex %= maxParticles;
		particleCount--;
		return;
	}

	// Calculate age percentage for lerp
	float agePercent = particles[i].Age / lifetime;

	// Interpolate the color
	XMStoreFloat4(
		&particles[i].Color,
		XMVectorLerp(
			XMLoadFloat4(&startColor),
			XMLoadFloat4(&endColor),
			agePercent));

	// Lerp size
	particles[i].Size = startSize + agePercent * (endSize - startSize);

	// Adjust the position
	XMVECTOR startPos = XMLoadFloat3(&position);
	XMVECTOR startVel = XMLoadFloat3(&particles[i].StartVel);
	XMVECTOR acc = XMLoadFloat3(&accel);
	float t = particles[i].Age;

	// Use constant acceleration function
	XMStoreFloat3(
		&particles[i].Position,
		acc * t * t / 2.0f + startVel * t + startPos);
}

void Emitter::Spawn()
{
	if (particleCount == maxParticles)
		return;

	// Reset the first dead particle
	particles[firstDeadIndex].Age = 0;
	particles[firstDeadIndex].Size = startSize;
	particles[firstDeadIndex].Color = startColor;
	particles[firstDeadIndex].Position = position;
	particles[firstDeadIndex].StartVel = velocity;
	particles[firstDeadIndex].StartVel.x += ((float)rand() / RAND_MAX) * 0.4f - 0.2f;
	particles[firstDeadIndex].StartVel.y += ((float)rand() / RAND_MAX) * 0.4f - 0.2f;
	particles[firstDeadIndex].StartVel.z += ((float)rand() / RAND_MAX) * 0.4f - 0.2f;

	// Increment and wrap
	firstDeadIndex++;
	firstDeadIndex %= maxParticles;

	particleCount++;
}

void Emitter::SetPosition(XMFLOAT3 p)
{
	position = p;
}

void Emitter::Dump(ID3D11DeviceContext * context)
{
	if (firstAliveIndex < firstDeadIndex) {
		for (int i = firstAliveIndex; i < firstDeadIndex; i++)
			DumpOne(i);
	}
	else {
		for (int i = firstAliveIndex; i < maxParticles; i++)
			DumpOne(i);
		for (int i = 0; i < firstDeadIndex; i++)
			DumpOne(i);
	}

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, localParticleVertices, sizeof(ParticleVertex) * 4 * maxParticles);
	context->Unmap(vertexBuffer, 0);
}

void Emitter::DumpOne(int index) {
	int i = index * 4;

	localParticleVertices[i + 0].Position = particles[index].Position;
	localParticleVertices[i + 1].Position = particles[index].Position;
	localParticleVertices[i + 2].Position = particles[index].Position;
	localParticleVertices[i + 3].Position = particles[index].Position;

	localParticleVertices[i + 0].Size = particles[index].Size;
	localParticleVertices[i + 1].Size = particles[index].Size;
	localParticleVertices[i + 2].Size = particles[index].Size;
	localParticleVertices[i + 3].Size = particles[index].Size;

	localParticleVertices[i + 0].Color = particles[index].Color;
	localParticleVertices[i + 1].Color = particles[index].Color;
	localParticleVertices[i + 2].Color = particles[index].Color;
	localParticleVertices[i + 3].Color = particles[index].Color;
}

void Emitter::Draw(ID3D11DeviceContext * context, Camera* camera)
{
	// Copy to dynamic buffer
	Dump(context);

	// Set up buffers
	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	vertexShader->SetMatrix4x4("view", camera->GetCamMatrix());
	vertexShader->SetMatrix4x4("projection", camera->GetProjectionMatrix());
	vertexShader->SetShader();
	vertexShader->CopyAllBufferData();

	pixelShader->SetShaderResourceView("particle", texture);
	pixelShader->SetShader();
	pixelShader->CopyAllBufferData();

	// Draw the correct parts of the buffer
	if (firstAliveIndex < firstDeadIndex) {
		context->DrawIndexed(particleCount * 6, firstAliveIndex * 6, 0);
	}
	else {
		context->DrawIndexed(firstDeadIndex * 6, 0, 0);
		context->DrawIndexed((maxParticles - firstAliveIndex) * 6, firstAliveIndex * 6, 0);
	}

}
