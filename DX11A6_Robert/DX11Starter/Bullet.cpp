#include "Bullet.h"



Bullet::Bullet() : Bullet(nullptr, nullptr, nullptr)
{
}

Bullet::Bullet(Mesh * mesh, Material * material, GameEntity * pl) : GameEntity(mesh, material)
{
	distTravelled = 0;
	maxDistTravelled = 5.0f;
}


Bullet::~Bullet()
{
}

void Bullet::UpdateEntity(float deltaTime)
{
	/*
	if (distTravelled >= maxDistTravelled) {
		this->~Bullet();
	}
	*/
	// To normalize rotation vector before calculation

	//XMVECTOR pos = XMLoadFloat3(&GetPosition());

	XMFLOAT3 temp = GetRotation();
	temp = Normalize(temp);

	XMVECTOR pos = XMLoadFloat3(&GetPosition());
	XMVECTOR rot = XMLoadFloat3(&temp);
	XMVECTOR sca = XMLoadFloat3(&GetScale());

	pos += rot;

	XMStoreFloat3(&temp, rot * 0.1);
	TransformTranslation(temp);

	GameEntity::UpdateEntity();
}