#include "GameEntity.h"



GameEntity::GameEntity()
{
}

GameEntity::GameEntity(Mesh * mesh)
{
	entityMesh = mesh;
	position = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
	collisionPosition = XMFLOAT3(0, 0, 0);
	collisionBox = nullptr;
	calcWorldMatrix();
	recalculateWorldMatrix = false;

}

GameEntity::GameEntity(Mesh * mesh, Material * material)
{
	entityMesh = mesh;
	entityMaterial = material;
	position = XMFLOAT3(0, 0, 0);
	collisionPosition = XMFLOAT3(0, 0, 1);
	rotation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
	collisionBox = nullptr;
	calcWorldMatrix();
	recalculateWorldMatrix = false;
}


GameEntity::~GameEntity()
{
	if (collisionBox != nullptr) { delete collisionBox; }
}

float GameEntity::GetColliderXWidth()
{
	return collisionBox->GetColliderXWidth();
}

float GameEntity::GetColliderYHeight()
{
	return collisionBox->GetColliderYHeight();
}

float GameEntity::GetColliderZDepth()
{
	return collisionBox->GetColliderZDepth();
}

XMFLOAT4X4 GameEntity::GetWorldMatrix()
{
	return worldMatrix;
}

void GameEntity::SetWorldMatrix(XMFLOAT4X4 setMatrix)
{
	worldMatrix = setMatrix;
}

XMFLOAT3 GameEntity::GetPosition()
{
	return position;
}

XMFLOAT3 GameEntity::GetRotation()
{
	return rotation;
}

XMFLOAT3 GameEntity::GetScale()
{
	return scale;
}

Collider* GameEntity::GetCollider()
{
	if (collisionBox == nullptr) { return nullptr; }
	else { return collisionBox; }
}

void GameEntity::SetPosition(XMFLOAT3 setPos)
{
	position = setPos;
	if (collisionBox != nullptr) { 
		collisionPosition = position;
		collisionBox->SetColliderPosition(setPos); 
	}
	//printf("Player position updated%d %d %d\n", position.x, position.y, position.z);
}
void GameEntity::PrintPosition() {
	//printf("Pos x: %d	Pos y: %d	Pos z: %d \n", position.x, position.y, position.z);
}

XMFLOAT3 GameEntity::Lerp(XMFLOAT3 start, XMFLOAT3 end, float percent)
{
	XMVECTOR s = XMLoadFloat3(&start);
	XMVECTOR e = XMLoadFloat3(&end);
	XMFLOAT3 result;

	XMStoreFloat3(&result, s + (percent * (e - s)));


	printf("\nResult x: %f", result.x);
	printf("\nResult y: %f", result.y);
	printf("\nResult z: \n%f", result.z);

	return result;
}
void GameEntity::SetRotation(XMFLOAT3 setRot)
{
	rotation = setRot;
}

void GameEntity::SetScale(XMFLOAT3 setSca)
{
	scale = setSca;
}

void GameEntity::SetCollisionBox(float xDim, float yDim, float zDim)
{
	collisionBox = new Collider(xDim, yDim, zDim, collisionPosition);
}

void GameEntity::TransformTranslation(XMFLOAT3 value)
{
	XMVECTOR cur = XMLoadFloat3(&position);
	XMVECTOR curCollide = XMLoadFloat3(&collisionPosition);
	XMVECTOR manip = XMLoadFloat3(&value);
	cur = cur + manip;
	curCollide = curCollide + manip;
	XMStoreFloat3(&position, cur);
	XMStoreFloat3(&collisionPosition, curCollide);
	recalculateWorldMatrix = true;
}

void GameEntity::TransformScale(XMFLOAT3 value)
{
	XMVECTOR cur = XMLoadFloat3(&scale);
	XMVECTOR manip = XMLoadFloat3(&value);
	cur = cur + manip;
	XMStoreFloat3(&scale, cur);
	recalculateWorldMatrix = true;
}

void GameEntity::TransformRotation(XMFLOAT3 value)
{
	//Takes it in degrees
	//Goes off directx standard of positive clockwise, radian data
	XMVECTOR cur = XMLoadFloat3(&rotation);
	value.x = (value.x * XM_PI) / 180;
	value.y = (value.y * XM_PI) / 180;
	value.z = (value.z * XM_PI) / 180;
	XMVECTOR manip = XMLoadFloat3(&value);
	cur = cur + manip;
	XMStoreFloat3(&rotation, cur);
	recalculateWorldMatrix = true;
	//vector math - rotate collision box

	///note
	/*TO DO
	Need to reevaluate bounding box
	Rather than radii, have coordinate edges based on bounds
	manipulate those based on rotation/scale at start
	update translation is fine, since it's based off pos.
	cont.
	*/

}

void GameEntity::PrepareMaterial(XMFLOAT4X4 camView, XMFLOAT4X4 camProj)
{


	entityMaterial->GetVertPointer()->SetMatrix4x4("world", worldMatrix);
	entityMaterial->GetVertPointer()->SetMatrix4x4("view", camView);
	entityMaterial->GetVertPointer()->SetMatrix4x4("projection", camProj);
	entityMaterial->GetVertPointer()->CopyAllBufferData();

	entityMaterial->GetPixPointer()->SetSamplerState("basicSampler", entityMaterial->GetSamplerState());
	entityMaterial->GetPixPointer()->SetShaderResourceView("textureSRV", entityMaterial->GetSRV());
	entityMaterial->GetPixPointer()->SetShaderResourceView("textureNRM", entityMaterial->GetNormalRV());
	entityMaterial->GetPixPointer()->CopyAllBufferData();

	entityMaterial->GetVertPointer()->SetShader();
	entityMaterial->GetPixPointer()->SetShader();

}

void GameEntity::calcWorldMatrix()
{
	printf("\nCurrent collision position: %f, %f, %f\n", collisionPosition.x, collisionPosition.y, collisionPosition.z);
	//Conversions
	XMVECTOR colPos = XMLoadFloat3(&collisionPosition);

	XMVECTOR pos = XMLoadFloat3(&position);
	printf("\n%f, %f, %f", position.x, position.y, position.z);
	XMVECTOR rot = XMLoadFloat3(&rotation);
	XMVECTOR sca = XMLoadFloat3(&scale);
	//Settings
	XMMATRIX posTranslate = XMMatrixTranslationFromVector(pos);
	XMMATRIX rotTranslate = XMMatrixRotationRollPitchYawFromVector(rot);
	XMMATRIX scaTranslate = XMMatrixScalingFromVector(sca);
	//Final math
	colPos = XMVector3Transform(colPos, rotTranslate);

	XMMATRIX resultant = scaTranslate * rotTranslate * posTranslate;
	//Storage
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(resultant));
	XMStoreFloat3(&collisionPosition, colPos);
	printf("\nAfter collision position: %f, %f, %f\n", collisionPosition.x, collisionPosition.y, collisionPosition.z);
	if (collisionBox != nullptr) {
		collisionBox->SetColliderPosition(collisionPosition);
	}
}
void GameEntity::ColliderBoxMatrix(bool safeRotation) 
{
	XMFLOAT4X4* manipulationMatrix;
	//Conversions
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR rot = XMLoadFloat3(&rotation);
	XMVECTOR sca = XMLoadFloat3(&scale);

	//Settings
	XMMATRIX posTranslate = XMMatrixTranslationFromVector(pos);
	XMMATRIX rotTranslate = XMMatrixRotationRollPitchYawFromVector(rot);
	XMMATRIX scaTranslate = XMMatrixScalingFromVector(sca);

	XMMATRIX resultant;
	//Final math
	if (safeRotation) {
		resultant = scaTranslate * rotTranslate * posTranslate;
	}
	else {
		resultant = scaTranslate * posTranslate;
	}
	XMStoreFloat4x4(manipulationMatrix, XMMatrixTranspose(resultant));
	collisionBox->ManipulateCollisionBox(manipulationMatrix);
}

//Deprecated - by Transformation Translation
//Keeping in case I want to have more discrete movement functions in future
void GameEntity::Move(XMFLOAT3 value)
{
	XMVECTOR cur = XMLoadFloat3(&position);
	XMVECTOR manip = XMLoadFloat3(&value);
	cur = cur + manip;
	XMStoreFloat3(&position, cur);
	recalculateWorldMatrix = true;

}
//Stub for more discrete movement functions in future
void GameEntity::MoveForward(XMFLOAT3 value)
{
}

XMFLOAT3 GameEntity::GetColliderPosition() {
	return collisionPosition;
}

ID3D11Buffer * GameEntity::GetVertBuffer()
{
	return entityMesh->GetVertexBuffer();
}

ID3D11Buffer * GameEntity::GetIndBuffer()
{
	return entityMesh->GetIndexBuffer();
}

int GameEntity::GetIndCount()
{
	return entityMesh->GetIndexCount();
}

//Currnetly just checks if it needs to recalculate worldMatrix
//Can be expanded in future calls
//Draw functionality in Game.cpp
void GameEntity::UpdateEntity()
{
	if (recalculateWorldMatrix) {
		calcWorldMatrix();
		recalculateWorldMatrix = false;
	}

}
