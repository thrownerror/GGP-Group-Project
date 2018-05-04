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
	if (distTravelled >= maxDistTravelled) {
		this->~Bullet();
	}
	GameEntity::UpdateEntity();
}