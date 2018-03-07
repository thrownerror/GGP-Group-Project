#include "CollisionManager.h"



CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}

bool CollisionManager::isColliding(GameEntity ge1, GameEntity ge2) {
	
	float x1Width = ge1.GetColliderXWidth();
	float y1Height = ge1.GetColliderYHeight();
	float z1Depth = ge1.GetColliderZDepth();

	XMFLOAT3 ge1Position = ge1.GetPosition();
	float x1Position = ge1Position.x;
	float y1Position = ge1Position.y;
	float z1Position = ge1Position.z;

	float x2Width = ge2.GetColliderXWidth();
	float y2Height = ge2.GetColliderYHeight();
	float z2Depth = ge2.GetColliderZDepth();
	XMFLOAT3 ge2Position = ge2.GetPosition();
	float x2Position = ge2Position.x;
	float y2Position = ge2Position.y;
	float z2Position = ge2Position.z;

	return(((x1Position - x1Width <= x2Position + x2Width) && (x1Position + x1Width >= x2Position - x2Width)) &&
		   ((y1Position - y1Height <= y2Position + y2Height) && (y1Position + y1Height >= y2Position - y2Height)) &&
		   ((z1Position - z1Depth <= z2Position + z2Depth) && (z1Position + z1Depth >= z2Position - z2Depth)));

	//return true;
}
