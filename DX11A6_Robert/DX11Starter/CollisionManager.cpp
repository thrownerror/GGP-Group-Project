#include "CollisionManager.h"



CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}

bool CollisionManager::isColliding(GameEntity* ge1, GameEntity* ge2) {
	
//	float x1Width = ge1->GetColliderXWidth();
//	float y1Height = ge1->GetColliderYHeight();
//	float z1Depth = ge1->GetColliderZDepth();
	XMFLOAT3 ge1Position = ge1->GetColliderPosition();
	XMFLOAT3 ge2Position = ge2->GetColliderPosition();
	Collider* ge1Collider = ge1->GetCollider();
	Collider* ge2Collider = ge2->GetCollider();

	XMFLOAT3** ge1Array = ge1Collider->GetColliderCorners();
	XMFLOAT3** ge2Array = ge2Collider->GetColliderCorners();
	//8 notches
	//First 4 - topLeftFront, topRightFront, bottomLeftFront, bottomRightFront
	//				0				1				2				3
	//Next 4 - topLeftBack, topRightBack, bottomLeftBack, bottomRightBack
	//				4			5				6				7


//	float x1Position = ge1Position.x;
//	float y1Position = ge1Position.y;
//	float z1Position = ge1Position.z;

//	float x2Width = ge2->GetColliderXWidth();
//	float y2Height = ge2->GetColliderYHeight();
//	float z2Depth = ge2->GetColliderZDepth();
	//XMFLOAT3 ge2Position = ge2->GetPosition();
	//printf("GE2 position: %d %d %d\n", ge2->GetPosition().x, ge2->GetPosition().y, ge2->GetPosition().z);
	//float x2Position = ge2Position.x;
	//float y2Position = ge2Position.y;
	//float z2Position = ge2Position.z;
	
	//return(((x1Position - x1Width <= x2Position + x2Width) && (x1Position + x1Width >= x2Position - x2Width)) &&
	//	   ((y1Position - y1Height <= y2Position + y2Height) && (y1Position + y1Height >= y2Position - y2Height)) &&
	//	   ((z1Position - z1Depth <= z2Position + z2Depth) && (z1Position + z1Depth >= z2Position - z2Depth)));
	
	float x1Max = ge1Array[1]->x;
	float x1Min = ge1Array[0]->x;
	float y1Max = ge1Array[0]->y;
	float y1Min = ge1Array[2]->y;
	float z1Max = ge1Array[1]->z;
	float z1Min = ge1Array[4]->z;
							 
	float x2Max = ge2Array[1]->x;
	float x2Min = ge2Array[0]->x;
	float y2Max = ge2Array[0]->y;
	float y2Min = ge2Array[2]->y;
	float z2Max = ge2Array[1]->z;
	float z2Min = ge2Array[4]->z;

	return(((x1Min + ge1Position.x <= x2Max + ge2Position.x) && (x1Max + ge1Position.x >= x2Min + ge2Position.x)) &&
		   ((y1Min + ge1Position.y <= y2Max + ge2Position.y) && (y1Max + ge1Position.y >= y2Min + ge2Position.y)) &&
		   ((z1Min + ge1Position.z <= z2Max + ge2Position.z) && (z1Max + ge1Position.z >= z2Min + ge2Position.z))
		  );
		   
	
	//return true;
}
