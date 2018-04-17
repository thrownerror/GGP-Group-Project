#include "Bullet.h"



Bullet::Bullet()
{
}

Bullet::Bullet(Mesh * mesh, Material * material) : GameEntity(mesh, material)
{
}


Bullet::~Bullet()
{
}

void Bullet::UpdateEntity(float deltaTime)
{
	GameEntity::UpdateEntity();
}