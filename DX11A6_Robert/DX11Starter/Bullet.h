#pragma once
#include "GameEntity.h"
class Bullet :
	public GameEntity
{
public:
	Bullet();
	Bullet(Mesh* mesh, Material* material);
	~Bullet();
};

