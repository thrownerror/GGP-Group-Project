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
	calcWorldMatrix();
	recalculateWorldMatrix = false;

}

GameEntity::GameEntity(Mesh * mesh, Material * material)
{
	entityMesh = mesh;
	entityMaterial = material;
	position = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
	calcWorldMatrix();
	recalculateWorldMatrix = false;
}


GameEntity::~GameEntity()
{
}

float GameEntity::GetColliderXWidth()
{
	return colliderXWidth;
}

float GameEntity::GetColliderYHeight()
{
	return colliderYHeight;
}

float GameEntity::GetColliderZDepth()
{
	return colliderZDepth;
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

void GameEntity::SetPosition(XMFLOAT3 setPos)
{
	position = setPos;
}

void GameEntity::SetRotation(XMFLOAT3 setRot)
{
	rotation = setRot;
}

void GameEntity::SetScale(XMFLOAT3 setSca)
{
	scale = setSca;
}

void GameEntity::TransformTranslation(XMFLOAT3 value)
{
	XMVECTOR cur = XMLoadFloat3(&position);
	XMVECTOR manip = XMLoadFloat3(&value);
	cur = cur + manip;
	XMStoreFloat3(&position, cur);
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
}

void GameEntity::PrepareMaterial(XMFLOAT4X4 camView, XMFLOAT4X4 camProj)
{


	entityMaterial->GetVertPointer()->SetMatrix4x4("world", worldMatrix);
	entityMaterial->GetVertPointer()->SetMatrix4x4("view", camView);
	entityMaterial->GetVertPointer()->SetMatrix4x4("projection", camProj);
	entityMaterial->GetVertPointer()->CopyAllBufferData();

	entityMaterial->GetPixPointer()->SetSamplerState("basicSampler", entityMaterial->GetSamplerState());
	entityMaterial->GetPixPointer()->SetShaderResourceView("textureSRV", entityMaterial->GetSRV());
	entityMaterial->GetPixPointer()->CopyAllBufferData();

	entityMaterial->GetVertPointer()->SetShader();
	entityMaterial->GetPixPointer()->SetShader();

}

void GameEntity::calcWorldMatrix()
{
	//Conversions
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR rot = XMLoadFloat3(&rotation);
	XMVECTOR sca = XMLoadFloat3(&scale);
	//Settings
	XMMATRIX posTranslate = XMMatrixTranslationFromVector(pos);
	XMMATRIX rotTranslate = XMMatrixRotationRollPitchYawFromVector(rot);
	XMMATRIX scaTranslate = XMMatrixScalingFromVector(sca);
	//Final math
	XMMATRIX resultant = scaTranslate * rotTranslate * posTranslate;
	//Storage
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(resultant));
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
