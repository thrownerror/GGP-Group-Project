#pragma once
#include "GameEntity.h"
class Bullet :
	public GameEntity
{
private:
	float distTravelled;
	float maxDistTravelled;
public:
	Bullet();
	Bullet(Mesh * mesh, Material * material, GameEntity * pl);
	~Bullet();

	void UpdateEntity(float deltaTime);
};

