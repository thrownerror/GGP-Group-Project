#include "Bullet.h"



Bullet::Bullet() : Bullet(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr)
{
}

Bullet::Bullet(Mesh * mesh, Material * material, GameEntity * pl, ID3D11Device * d, SimpleVertexShader* pVert, SimplePixelShader* pPixel, ID3D11ShaderResourceView* pText) : GameEntity(mesh, material)
{
	distTravelled = 0;
	maxDistTravelled = 5.0f;
	direction = XMFLOAT3();
	device = d;

	emitter = new Emitter(
		100,
		10,
		5,
		0.1f,
		5.0f,
		XMFLOAT4(1, 0.1f, 0.1f, 0.2f),
		XMFLOAT4(1, 0.6f, 0.1f, 0),
		XMFLOAT3(0, 2, 2),
		GetPosition(),
		XMFLOAT3(0, -1, 0),
		device,
		pVert,
		pPixel,
		pText
	);
}

XMFLOAT3 Bullet::GetDirection()
{
	return direction;
}


void Bullet::SetDirection(XMFLOAT3 dir)
{
	direction = dir;
}

// Sets position of Bullet AND attached Emitter
void Bullet::SetPosition(XMFLOAT3 pos)
{
	emitter->SetPosition(pos);
	GameEntity::SetPosition(pos);
}

float Bullet::DistTravelled()
{
	return distTravelled;
}

float Bullet::MaxDistTravelled()
{
	return maxDistTravelled;
}

Emitter Bullet::GetEmitter()
{
	return *emitter;
}

Bullet::~Bullet()
{
	delete emitter;
}

void Bullet::UpdateEntity(float deltaTime)
{
	//printf("Bullet pos: %f %f %f \n", GetPosition().x, GetPosition().y, GetPosition().z);
	/*
	if (distTravelled >= maxDistTravelled) {
		this->~Bullet();
	}
	
	// To normalize rotation vector before calculation

	//XMVECTOR pos = XMLoadFloat3(&GetPosition());
	/*
	XMFLOAT3 temp = GetRotation();
	temp = Normalize(temp);

	XMVECTOR pos = XMLoadFloat3(&GetPosition());
	XMVECTOR rot = XMLoadFloat3(&temp);
	XMVECTOR sca = XMLoadFloat3(&GetScale());

	pos += rot;

	XMStoreFloat3(&temp, rot * 0.1);
	TransformTranslation(temp);*/
	// Multiplies direction by change in time before transforming bullet
	XMVECTOR loadDir = XMLoadFloat3(&GetDirection());

	loadDir *= deltaTime;

	XMFLOAT3 storeDir;
	XMStoreFloat3(&storeDir, loadDir);

	TransformTranslation(XMFLOAT3(storeDir.x, 0, storeDir.z));
	distTravelled += deltaTime;

	emitter->Update(deltaTime);

	GameEntity::UpdateEntity();
}