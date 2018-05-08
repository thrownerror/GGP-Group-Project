#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "Enemy.h"
#include "Camera.h"
#include "Lights.h"
#include "CollisionManager.h"
#include "Emitter.h"
class Game 
	: public DXCore
{

public:


	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);

private:

	CollisionManager collidingMaster = CollisionManager();
	bool testBox;
	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();
	void BuildCollisionGeometry();
	void CreateStartingSpace();
	void CreateShaftGeometry();
	void CreateAreneaGeometry();
	void BuildLevelGeometry();
	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	//Mesh objects
	Mesh* meshQuad;
	Mesh* meshModel1;
	Mesh* meshModel2;

	//Material objects
	Material* mat1;
	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* nrm;
	ID3D11SamplerState* sampState;

	Material* mat2;
	ID3D11ShaderResourceView* srv2;
	ID3D11ShaderResourceView* nrm2;
	ID3D11SamplerState* sampState2;

	//GameEntity objects
	GameEntity** entityArray;
	GameEntity** collisionArray;
	int collisionArraySize;
	int entityArraySize;

	GameEntity* gePlayer;
	GameEntity* ge1;

	//walls - first room
	GameEntity* wall1; //front 
	GameEntity* wall2; //back
	GameEntity* wall3; //left
	GameEntity* wall4; //right
	GameEntity* wall5; //top
	GameEntity* wall6; //bottom

	//hallway1
	GameEntity* wall7; //top
	GameEntity* wall8; //bottom
	GameEntity* wall9; //left
	GameEntity* wall10; //right

	//Shaft1
	GameEntity* wall11; //bottom - checked 
	GameEntity* wall12; //left wall 1 - checked 
	GameEntity* wall13; //right wall 1 - checked
	GameEntity* wall14; //left wall 2 - checked
	GameEntity* wall15; //right wall 2 - cheked
	GameEntity* wall16; //back wall height 2 - checked
	GameEntity* wall17; //back wall 2 - checked
	GameEntity* wall18; //top wall 3 - cheeck
	GameEntity* wall19; //Front wall 2 - checked
	GameEntity* wall20; //right 3 wall - checked
	GameEntity* wall21; //left 3 wall - checked

	//GameEntity* wall11;
	//arena spaces
	GameEntity* arenaF11;
	GameEntity* arenaF12;
	GameEntity* arenaF13;
	GameEntity* arenaF14;

	GameEntity* arenaF21;
	GameEntity* arenaF22;
	GameEntity* arenaF23;
	GameEntity* arenaF24;

	GameEntity* arenaF31;
	GameEntity* arenaF32;
	GameEntity* arenaF33;
	GameEntity* arenaF34;

	GameEntity* arenaT11;
	GameEntity* arenaT12;
	GameEntity* arenaT13;
	GameEntity* arenaT14;
					 
	GameEntity* arenaT21;
	GameEntity* arenaT22;
	GameEntity* arenaT23;
	GameEntity* arenaT24;
					 
	GameEntity* arenaT31;
	GameEntity* arenaT32;
	GameEntity* arenaT33;
	GameEntity* arenaT34;

	GameEntity* arenaB1;
	GameEntity* arenaB3;
	GameEntity* arenaB4;

	GameEntity* arenaL1;
	GameEntity* arenaL2;
	GameEntity* arenaL3;
	GameEntity* arenaL4;

	GameEntity* arenaR1;
	GameEntity* arenaR2;
	GameEntity* arenaR3;

	GameEntity* arenaFW2;
	GameEntity* arenaFW3;
	GameEntity* arenaFW4;
	
	//End cap 
	GameEntity* wallEnd; // front wall 3 - checked


	//CollisionVolumes
	//initial space
	GameEntity* ce1; //left
	GameEntity* ce2; //right
	GameEntity* ce3; //up
	GameEntity* ce4; //down
	GameEntity* ce5; //back
	
	//shaft1;
	GameEntity* shaftFront;
	GameEntity* shaftBack;
	GameEntity* shaftLeft;
	GameEntity* shaftRight;



	//Arena 
	GameEntity* arenaTop;
	GameEntity* arenaLeft;
	GameEntity* arenaRight;
	GameEntity* arenaFront;
	GameEntity* arenaBack;
	GameEntity* arenaBack2;
	GameEntity* arenaBottom;



	Enemy* e0;
	Enemy* e1;

	Camera* camera;

	//Lighting
	DirectionalLight dLight;
	DirectionalLight dLight2;
	PointLight pLight;

	// Shadow Map Data
	ID3D11Texture2D* shadowTexture;
	ID3D11DepthStencilView* shadowDepth;
	ID3D11ShaderResourceView* shadowSRV;
	ID3D11SamplerState* shadowSampler;
	ID3D11RasterizerState* shadowRasterizer;
	SimpleVertexShader* shadowShader;
	XMFLOAT4X4 shadowView;
	XMFLOAT4X4 shadowProjection;

	// Particle Emitter Data
	ID3D11ShaderResourceView* particleTexture;
	SimpleVertexShader* particleVertex;
	SimplePixelShader* particlePixel;
	ID3D11DepthStencilState* particleDS;
	ID3D11BlendState* particleBlender;
	Emitter* emitter;
	std::vector<Emitter*> emitters;

	XMFLOAT3 movementRight		= XMFLOAT3(+1.0f, 0.0f, 0.0f);
	XMFLOAT3 movementLeft		= XMFLOAT3(-1.0f, 0.0f, 0.0f);
	XMFLOAT3 movementDown		= XMFLOAT3(0.0f, -1.0f, 0.0f);
	XMFLOAT3 movementUp			= XMFLOAT3(0.0f, +1.0f, 0.0f);
	XMFLOAT3 movementBackward	= XMFLOAT3(0.0f, 0.0f, -1.0f);
	XMFLOAT3 movementForward	= XMFLOAT3(0.0f, 0.0f, +1.0f);

	XMFLOAT3 rotation180AroundY = XMFLOAT3(0.0f, 180.0f, 0.0f);
	XMFLOAT3 rotate90CWAroundY	= XMFLOAT3(0.0f, 90.0f, 0.0f);
	XMFLOAT3 rotate90CCWAroundY = XMFLOAT3(0.0f, -90.0f, 0.0f);
	XMFLOAT3 rotate90CWAroundX	= XMFLOAT3(90.0f, 0.0f, 0.0f);
	XMFLOAT3 rotate90CCWAroundX = XMFLOAT3(-90.0f, 0.0f, 0.0f);
};

