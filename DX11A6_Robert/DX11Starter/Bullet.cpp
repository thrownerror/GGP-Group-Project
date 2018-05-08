#include "Bullet.h"



Bullet::Bullet() : Bullet(nullptr, nullptr, nullptr)
{
}

Bullet::Bullet(Mesh * mesh, Material * material, GameEntity * pl) : GameEntity(mesh, material)
{
	distTravelled = 0;
	maxDistTravelled = 5.0f;
	direction = XMFLOAT3();
}

XMFLOAT3 Bullet::GetDirection()
{
	return direction;
}


void Bullet::SetDirection(XMFLOAT3 dir)
{
	direction = dir;
}

Bullet::~Bullet()
{
}

void Bullet::UpdateEntity(float deltaTime)
{
	//printf("Bullet pos: %f %f %f \n", GetPosition().x, GetPosition().y, GetPosition().z);
	/*
	if (distTravelled >= maxDistTravelled) {
		this->~Bullet();
	}
	*/
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

	GameEntity::UpdateEntity();
}