#include "Game.h"
#include "Vertex.h"
#include "WICTextureLoader.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexBuffer = 0;
	indexBuffer = 0;
	vertexShader = 0;
	pixelShader = 0;


#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");

	// Use this to debug memory leaks. Replace the 518 with a
	// number in {} that gets dumped in memory leaks.
	// For example, this block:
	//		{518} normal block at 0x000001DD233143C0, 12 bytes long.
	//		Data: < ? ? \ ? > 00 00 80 3F 00 00 80 3F 5C 8F 82 3F
	// Should be checked using 518, the number in {}.
	// Visual Studio in DEBUG mode will automatically breakpoint
	// when that object in memory is allocated.
	//_CrtSetBreakAlloc(2372);
	//D3D11_CREATE_DEVICE_DEBUG;
#else
	0
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	delete meshQuad;
	delete meshModel1;

	delete ge1;
	delete wall1;
	delete wall2;
	delete wall3;
	delete wall4;
	delete wall5;
	delete wall6;

	delete wall7;
	delete wall8;
	delete wall9;
	delete wall10;

	delete wall11;
	delete wall12;
	delete wall13;
	delete wall14;
	delete wall15;
	delete wall16;
	delete wall17;
	delete wall18;
	delete wall19;
	delete wall20;
	delete wall21;

	delete arenaF11;
	delete arenaF12;
	delete arenaF13;
	delete arenaF21;
	delete arenaF22;
	delete arenaF23;
	delete arenaF31;
	delete arenaF32;
	delete arenaF33;
	delete arenaF34;
	delete arenaF24;
	delete arenaF14;
	
	delete arenaT11;
	delete arenaT12;
	delete arenaT13;
	delete arenaT21;
	delete arenaT22;
	delete arenaT23;
	delete arenaT31;
	delete arenaT32;
	delete arenaT33;
	delete arenaT34;
	delete arenaT24;
	delete arenaT14;

	delete arenaB1;
	delete arenaB3;
	delete arenaB4;
	delete arenaL1;
	delete arenaL2;
	delete arenaL3;
	delete arenaL4;
	delete arenaR1;
	delete arenaR2;
	delete arenaR3;
//	delete arenaR4;

	delete arenaFW2;
	delete arenaFW3;
	delete arenaFW4;
	delete wallEnd;




	delete ce1;
	delete ce2;
	delete ce3;
	delete ce4;
	delete ce5;
	delete shaftFront;
	delete shaftBack;
	delete shaftLeft;
	delete shaftRight;
	delete arenaTop;
	delete arenaRight;
	delete arenaLeft;
	delete arenaFront;
	delete arenaBack;
	delete arenaBottom;
	delete arenaBack2;



	delete gePlayer;

	delete e0;

	delete[] entityArray;
	delete[] collisionArray;

	delete mat1;
	delete camera;

	// Clean up shadow map
	shadowDepth->Release();
	shadowSRV->Release();
	shadowSampler->Release();
	shadowRasterizer->Release();
	delete shadowShader;

	// Delete particle stuff
	particleTexture->Release();
	particleBlender->Release();
	particleDS->Release();
	delete particlePixel;
	delete particleVertex;
	delete emitter;

	srv->Release();
	sampState->Release();
	//mat1->GetSRV()->Release();
	//mat1->GetSamplerState()->Release();
	//commenting one of these will cause a memory leak
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();

	camera = new Camera();
	//ID3D11ShaderResourceView* srv;
	//ID3D11SamplerState* sampState;
	D3D11_SAMPLER_DESC samDesc = {};
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT samplerTest = device->CreateSamplerState(&samDesc, &sampState);
	HRESULT h1 = CreateWICTextureFromFile(device, context, L"../../Assets/Textures/Rock1.png", 0, &srv);
	HRESULT h2 = CreateWICTextureFromFile(device, context, L"../../Assets/Textures/RockNorm.png", 0, &nrm);
	HRESULT h3 = CreateWICTextureFromFile(device, context, L"../../Assets/Textures/fireParticle.jpg", 0, &particleTexture);

	mat1 = new Material(vertexShader, pixelShader, srv, nrm, sampState);

	prevMousePos.x = 0;
	prevMousePos.y = 0;
	CreateMatrices();
	CreateBasicGeometry();

	dLight.AmbientColor = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	dLight.DiffuseColor = XMFLOAT4(1, 1, 1, 0.5);
	dLight.Direction = XMFLOAT3(0, -1, 0);

	dLight2.AmbientColor = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	dLight2.DiffuseColor = XMFLOAT4(.5f, .5f, 0, 1);
	dLight2.Direction = XMFLOAT3(-1, .5f, .1f);

	// Real-Time Shadow Data Descriptions

	// Create the actual texture that will be the shadow map
	D3D11_TEXTURE2D_DESC shadowTexDesc = {};
	shadowTexDesc.Width = shadowTexDesc.Height = 1024;
	shadowTexDesc.MipLevels = 1;
	shadowTexDesc.ArraySize = 1;
	shadowTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	shadowTexDesc.CPUAccessFlags = 0;
	shadowTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowTexDesc.MipLevels = 1;
	shadowTexDesc.MiscFlags = 0;
	shadowTexDesc.SampleDesc.Count = 1;
	shadowTexDesc.SampleDesc.Quality = 0;
	shadowTexDesc.Usage = D3D11_USAGE_DEFAULT;
	device->CreateTexture2D(&shadowTexDesc, NULL, &shadowTexture);

	// Create the depth/stencil
	D3D11_DEPTH_STENCIL_VIEW_DESC shadowDSDesc = {};
	shadowDSDesc.Format = DXGI_FORMAT_D32_FLOAT;
	shadowDSDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	shadowDSDesc.Texture2D.MipSlice = 0;
	device->CreateDepthStencilView(shadowTexture, &shadowDSDesc, &shadowDepth);

	// Create the SRV for the shadow map
	D3D11_SHADER_RESOURCE_VIEW_DESC shadowSRVDesc = {};
	shadowSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shadowSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shadowSRVDesc.Texture2D.MipLevels = 1;
	shadowSRVDesc.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView(shadowTexture, &shadowSRVDesc, &shadowSRV);

	// Release the texture reference since we don't need it
	shadowTexture->Release();
	
	D3D11_SAMPLER_DESC shadowSampDesc = {};
	shadowSampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	shadowSampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	shadowSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.BorderColor[0] = 1.0f;
	shadowSampDesc.BorderColor[1] = 1.0f;
	shadowSampDesc.BorderColor[2] = 1.0f;
	shadowSampDesc.BorderColor[3] = 1.0f;
	device->CreateSamplerState(&shadowSampDesc, &shadowSampler);
		
	// Create a rasterizer state
	D3D11_RASTERIZER_DESC shadowRastDesc = {};
	shadowRastDesc.FillMode = D3D11_FILL_SOLID;
	shadowRastDesc.CullMode = D3D11_CULL_BACK;
	shadowRastDesc.DepthClipEnable = true;
	shadowRastDesc.DepthBias = 1000; // Multiplied by (smallest possible value > 0 in depth buffer)
	shadowRastDesc.DepthBiasClamp = 0.0f;
	shadowRastDesc.SlopeScaledDepthBias = 1.0f;
	device->CreateRasterizerState(&shadowRastDesc, &shadowRasterizer);

	// Do the particle stuff

	// A depth state for the particles
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // Turns off depth writing
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	device->CreateDepthStencilState(&dsDesc, &particleDS);

	// Blend for particles (additive)
	D3D11_BLEND_DESC blend = {};
	blend.AlphaToCoverageEnable = false;
	blend.IndependentBlendEnable = false;
	blend.RenderTarget[0].BlendEnable = true;
	blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&blend, &particleBlender);

	// Set up particles
	emitter = new Emitter(
		100,							// Max particles
		10,								// Particles per second
		5,								// Particle lifetime
		0.1f,							// Start size
		5.0f,							// End size
		XMFLOAT4(1, 0.1f, 0.1f, 0.2f),	// Start color
		XMFLOAT4(1, 0.6f, 0.1f, 0),		// End color
		XMFLOAT3(0, 2, 2),				// Start velocity
		XMFLOAT3(0, 0, 2),				// Start position
		XMFLOAT3(0, -1, 0),				// Start acceleration
		device,
		particleVertex,
		particlePixel,
		particleTexture);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	srv->Release();
	//	sampState->Release();
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	shadowShader = new SimpleVertexShader(device, context);
	shadowShader->LoadShaderFile(L"ShadowShader.cso");

	particleVertex = new SimpleVertexShader(device, context);
	particleVertex->LoadShaderFile(L"ParticleVertex.cso");

	particlePixel = new SimplePixelShader(device, context);
	particlePixel->LoadShaderFile(L"ParticlePixel.cso");
}

// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
	camera->UpdateProjectionMatrix((float)width, (float)height);

	// Calculate the shadow map view and projection for the light
	XMMATRIX shView = XMMatrixLookToLH(
		XMVectorSet(0, 20, -2, 0),			// Light 'position'
		XMLoadFloat3(&dLight.Direction),	// Light 'direction'
		XMVectorSet(0, 1, 0, 0));			// Up is up
	XMStoreFloat4x4(&shadowView, XMMatrixTranspose(shView));

	XMMATRIX shProj = XMMatrixOrthographicLH(40.0f, 40.0f, 0.01f, 100.0f);
	XMStoreFloat4x4(&shadowProjection, XMMatrixTranspose(shProj));
}

XMFLOAT3 CalculateNormal(const XMFLOAT3& vert, const XMFLOAT3& v1, const XMFLOAT3& v2) {
	XMFLOAT3 normal;

	XMVECTOR vector0 = XMLoadFloat3(&vert);
	XMVECTOR vector1 = XMLoadFloat3(&v1);
	XMVECTOR vector2 = XMLoadFloat3(&v2);

	XMVECTOR vectorSub1 = XMVectorSubtract(vector1, vector0);
	XMVECTOR vectorSub2 = XMVectorSubtract(vector2, vector0);

	XMVECTOR crossProduct = XMVector3Cross(vectorSub1, vectorSub2);
	XMStoreFloat3(&normal, crossProduct);

	return normal;
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	testBox = true;

	Vertex squareVertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, +0.0f), XMFLOAT3(), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, +1.0f, +0.0f), XMFLOAT3(), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(+1.0f, +1.0f, +0.0f), XMFLOAT3(), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), XMFLOAT3(), XMFLOAT2(0.0f, 0.0f) }
	};
	unsigned int squareIndices[]{ 0, 1, 2,
								  2, 3, 0 };
	// Calculate square normals
	squareVertices[0].Normal = CalculateNormal(squareVertices[0].Position, squareVertices[1].Position, squareVertices[2].Position);
	squareVertices[1].Normal = CalculateNormal(squareVertices[1].Position, squareVertices[0].Position, squareVertices[3].Position);
	squareVertices[2].Normal = CalculateNormal(squareVertices[2].Position, squareVertices[0].Position, squareVertices[3].Position);
	squareVertices[3].Normal = CalculateNormal(squareVertices[3].Position, squareVertices[1].Position, squareVertices[2].Position);

	//mesh1 = new Mesh("../../Assets/Models/cube.obj", device);
	meshQuad = new Mesh(squareVertices, 4, squareIndices, 6, device);
	meshModel1 = new Mesh("../../Assets/Models/cone.obj", device);

	ge1 = new GameEntity(meshQuad, mat1);

	wall1 = new GameEntity(meshQuad, mat1);
	wall2 = new GameEntity(meshQuad, mat1);
	wall3 = new GameEntity(meshQuad, mat1);
	wall4 = new GameEntity(meshQuad, mat1);
	wall5 = new GameEntity(meshQuad, mat1);
	wall6 = new GameEntity(meshQuad, mat1);

	//hallway
	wall7	= new GameEntity(meshQuad, mat1);
	wall8	= new GameEntity(meshQuad, mat1);
	wall9	= new GameEntity(meshQuad, mat1);
	wall10	= new GameEntity(meshQuad, mat1);

	wall11	= new GameEntity(meshQuad, mat1);
	wall12	= new GameEntity(meshQuad, mat1);
	wall13	= new GameEntity(meshQuad, mat1);
	wall14	= new GameEntity(meshQuad, mat1);
	wall15	= new GameEntity(meshQuad, mat1);
	wall16	= new GameEntity(meshQuad, mat1);
	wall17	= new GameEntity(meshQuad, mat1);
	wall18	= new GameEntity(meshQuad, mat1);
	wall19	= new GameEntity(meshQuad, mat1);
	wall20	= new GameEntity(meshQuad, mat1);
	wall21  = new GameEntity(meshQuad, mat1);

	arenaF11 = new GameEntity(meshQuad, mat1);
	arenaF12 = new GameEntity(meshQuad, mat1);
	arenaF13 = new GameEntity(meshQuad, mat1);
	arenaF14 = new GameEntity(meshQuad, mat1);
	arenaF21 = new GameEntity(meshQuad, mat1);
	arenaF22 = new GameEntity(meshQuad, mat1);
	arenaF23 = new GameEntity(meshQuad, mat1);
	arenaF24 = new GameEntity(meshQuad, mat1);
	arenaF31 = new GameEntity(meshQuad, mat1);
	arenaF32 = new GameEntity(meshQuad, mat1);
	arenaF33 = new GameEntity(meshQuad, mat1);
	arenaF34 = new GameEntity(meshQuad, mat1);
	
	arenaT11 = new GameEntity(meshQuad, mat1);
	arenaT12 = new GameEntity(meshQuad, mat1);
	arenaT13 = new GameEntity(meshQuad, mat1);
	arenaT14 = new GameEntity(meshQuad, mat1);
	arenaT21 = new GameEntity(meshQuad, mat1);
	arenaT22 = new GameEntity(meshQuad, mat1);
	arenaT23 = new GameEntity(meshQuad, mat1);
	arenaT24 = new GameEntity(meshQuad, mat1);
	arenaT31 = new GameEntity(meshQuad, mat1);
	arenaT32 = new GameEntity(meshQuad, mat1);
	arenaT33 = new GameEntity(meshQuad, mat1);
	arenaT34 = new GameEntity(meshQuad, mat1);

	arenaB1 = new GameEntity(meshQuad, mat1);
	arenaB3 = new GameEntity(meshQuad, mat1);
	arenaB4 = new GameEntity(meshQuad, mat1);

	arenaL1 = new GameEntity(meshQuad, mat1);
	arenaL2 = new GameEntity(meshQuad, mat1);
	arenaL3 = new GameEntity(meshQuad, mat1);
	arenaL4 = new GameEntity(meshQuad, mat1);

	arenaR1 = new GameEntity(meshQuad, mat1);
	arenaR2 = new GameEntity(meshQuad, mat1);
	arenaR3 = new GameEntity(meshQuad, mat1);

	arenaFW2 = new GameEntity(meshQuad, mat1);
	arenaFW3 = new GameEntity(meshQuad, mat1);
	arenaFW4 = new GameEntity(meshQuad, mat1);

//	arenaR4 = new GameEntity(meshQuad, mat1);

	wallEnd = new GameEntity(meshQuad, mat1);
	//wall7 = new GameEntity(meshQuad, mat1);


	//collision spaces
	ce1 = new GameEntity(meshQuad, mat1);
	ce2 = new GameEntity(meshQuad, mat1);
	ce3 = new GameEntity(meshQuad, mat1);
	ce4 = new GameEntity(meshQuad, mat1);
	ce5 = new GameEntity(meshQuad, mat1);

	shaftFront	= new GameEntity(meshQuad, mat1);
	shaftBack	= new GameEntity(meshQuad, mat1);
	shaftLeft = new GameEntity(meshQuad, mat1);
	shaftRight = new GameEntity(meshQuad, mat1);


	arenaTop = new GameEntity(meshQuad, mat1);
	arenaLeft = new GameEntity(meshQuad, mat1);
	arenaRight = new GameEntity(meshQuad, mat1);
	arenaFront = new GameEntity(meshQuad, mat1);
	arenaBack = new GameEntity(meshQuad, mat1);
	arenaBottom = new GameEntity(meshQuad, mat1);
	arenaBack2 = new GameEntity(meshQuad, mat1);

	gePlayer = new GameEntity(meshQuad);
	gePlayer->SetCollisionBox(.1f, .1f, .1f);

	e0 = new Enemy(meshModel1, mat1, meshModel1, mat1, gePlayer);
	e0->SetWanderPoints(XMFLOAT3(0, 0, 0), XMFLOAT3(5, 5, 5));
	e0->SetCollisionBox(.1f, .1f, .1f);

	if (testBox) {
		collisionArraySize = 15;
		collisionArray = new GameEntity*[collisionArraySize];
		collisionArray[0] = ce1;
		collisionArray[1] = ce2;
		collisionArray[2] = ce3;
		collisionArray[3] = ce4;
		collisionArray[4] = ce5;

		collisionArray[5] = shaftFront;
		collisionArray[6] = shaftBack;
		collisionArray[7] = shaftLeft;
		collisionArray[8] = shaftRight;


		collisionArray[9] = arenaTop;
		collisionArray[10] = arenaLeft;
		collisionArray[11] = arenaRight;
		collisionArray[12] = arenaFront;
		collisionArray[13] = arenaBack;
		collisionArray[14] = arenaBottom;



		entityArraySize = 60;

		entityArray = new GameEntity*[entityArraySize + 1];
		entityArray[0] = ge1;
		entityArray[1] = wall1;
		entityArray[2] = wall2;
		entityArray[3] = wall3;
		entityArray[4] = wall4;
		entityArray[5] = wall5;
		entityArray[6] = wall6;
		//
		entityArray[7] = wall7;
		entityArray[8] = wall8;
		entityArray[9] = wall9;
		entityArray[10] = wall10;

		entityArray[11] = wall11;
		entityArray[12] = wall12;
		entityArray[13] = wall13;
		entityArray[14] = wall14;
		entityArray[15] = wall15;
		entityArray[16] = wall16;
		entityArray[17] = wall17;
		entityArray[18] = wall18;
		entityArray[19] = wall19;
		entityArray[20] = wall20;
		entityArray[21] = wall21;

		entityArray[22] = wallEnd;

		entityArray[23] = arenaF11;
		entityArray[24] = arenaF12;
		entityArray[25] = arenaF13;
		entityArray[26] = arenaF14;

		entityArray[27] = arenaF21;
		entityArray[28] = arenaF22;
		entityArray[29] = arenaF23;
		entityArray[30] = arenaF24;

		entityArray[31] = arenaF31;
		entityArray[32] = arenaF32;
		entityArray[33] = arenaF33;
		entityArray[34] = arenaF34;

		entityArray[35] = arenaB1;
		entityArray[36] = arenaB3;
		entityArray[37] = arenaB4;

		entityArray[38] = arenaL1;
		entityArray[39] = arenaL2;
		entityArray[40] = arenaL3;
		entityArray[41] = arenaL4;

		entityArray[42] = arenaR1;
		entityArray[43] = arenaR2;
		entityArray[44] = arenaR3;

		entityArray[45] = arenaT11;
		entityArray[46] = arenaT12;
		entityArray[47] = arenaT13;
		entityArray[48] = arenaT14;
							   
		entityArray[49] = arenaT21;
		entityArray[50] = arenaT22;
		entityArray[51] = arenaT23;
		entityArray[52] = arenaT24;
							   
		entityArray[53] = arenaT31;
		entityArray[54] = arenaT32;
		entityArray[55] = arenaT33;
		entityArray[56] = arenaT34;

		entityArray[57] = arenaFW2;
		entityArray[58] = arenaFW3;
		entityArray[59] = arenaFW4;
	//	entityArray[45] = arenaR4;


		for (int i = 0; i < entityArraySize; i++) {
			entityArray[i]->SetCollisionBox(1.0f, 1.0f, .02f);
		}
		for (int i = 0; i < collisionArraySize; i++) {
			collisionArray[i]->SetCollisionBox(1.0f, 1.0f, .02f);
		}

		//XMFLOAT3 rotate
		//XMFLOAT3 scaleValue = XMFLOAT3(10.0f, 10.0f, 10.0f);
		//Reserved for any random entity

		//wall5->SetCollisionBox(1.0f, 1.0f, 1.0f);
		//wall6->SetCollisionBox(1.0f, 1.0f, 1.0f);
		//ge8->SetCollisionBox(1.0f, 1.0f, 1.0f);

		//Test wall/entity
		ge1->TransformTranslation(movementLeft);
		ge1->TransformTranslation(movementLeft);
		ge1->TransformTranslation(movementLeft);
		ge1->TransformTranslation(movementLeft);
		ge1->TransformTranslation(movementForward);
		ge1->SetCollisionBox(1.0f, 1.0f, .01f);
		ge1->UpdateEntity();

		BuildLevelGeometry();
		BuildCollisionGeometry();
	}
}
void Game::BuildCollisionGeometry() {

	//first hallway
	//left bounds
	ce1->TransformTranslation(movementLeft);
	ce1->SetCollisionBox(.1f, 1.0f, 2.5f);
	ce1->UpdateEntity();
	//right bounds
	ce2->TransformTranslation(movementRight);
	ce2->TransformTranslation(movementForward);
	ce2->SetCollisionBox(.1f, 1.0f, 2.5f);
	ce2->UpdateEntity();
	//up bounds
	ce3->TransformTranslation(movementUp);
	//ce3->TransformTranslation(movementUp);
	//ce3->TransformTranslation(movementForward);
	ce3->SetCollisionBox(1.0f, .1f, 1.0f);
	ce3->UpdateEntity();
	
	//down
	ce4->TransformTranslation(movementDown);
	//ce4->TransformTranslation(movementUp);
	//ce4->TransformTranslation(movementForward);
	ce4->SetCollisionBox(1.0f, .1f, 2.0f);
	ce4->UpdateEntity();

	ce5->TransformTranslation(movementBackward);
	ce5->TransformTranslation(movementBackward);
	ce5->SetCollisionBox(1.0f, 1.0f, 0.2f);
	ce5->UpdateEntity();

	shaftFront->TransformTranslation(movementForward);
	shaftFront->TransformTranslation(movementForward);
	shaftFront->TransformTranslation(movementForward);
	shaftFront->TransformTranslation(movementForward);
	shaftFront->TransformTranslation(movementUp);
	shaftFront->TransformTranslation(movementUp);
	shaftFront->SetCollisionBox(1.0f, 1.5f, 0.2f);
	shaftFront->UpdateEntity();

	shaftBack->TransformTranslation(movementForward);
	shaftBack->TransformTranslation(movementForward);
	shaftBack->TransformTranslation(movementUp);
	shaftBack->TransformTranslation(movementUp);
	shaftBack->TransformTranslation(movementUp);
	shaftBack->SetCollisionBox(1.0f, 1.0f, 0.2f);
	shaftBack->UpdateEntity();

	shaftLeft->TransformTranslation(movementForward);
	shaftLeft->TransformTranslation(movementForward);
	shaftLeft->TransformTranslation(movementLeft);
	shaftLeft->SetCollisionBox(.1f, 10.0f, 2.5f);
	shaftLeft->UpdateEntity();


	
}
void Game::BuildLevelGeometry() {
	CreateStartingSpace();
	CreateShaftGeometry();
	CreateAreneaGeometry();
}

void Game::CreateStartingSpace() {
	//Forward and back walls

	wall1->TransformTranslation(movementForward);
	wall1->TransformTranslation(movementForward);
	wall1->TransformTranslation(movementForward);
	wall1->TransformTranslation(movementForward);
	wall1->TransformTranslation(movementForward);
	wall1->SetCollisionBox(1.0f, 1.0f, 0.2f);
	wall1->UpdateEntity(); //moving wall1 back of hallway

	wall2->TransformTranslation(movementBackward);
	wall2->TransformRotation(rotation180AroundY);
	wall2->SetCollisionBox(1.0f, 1.0f, 0.2f);
	wall2->UpdateEntity(); //back wall

						   //Left and right walls

	wall3->TransformTranslation(movementLeft);
	wall3->TransformRotation(rotate90CCWAroundY);
	wall3->SetCollisionBox(0.2f, 1.0f, 1.0f);
	wall3->UpdateEntity(); //left

	wall4->TransformTranslation(movementRight);
	wall4->TransformRotation(rotate90CWAroundY);
	wall4->SetCollisionBox(0.2f, 1.0f, 1.0f);
	wall4->UpdateEntity(); //right

						   //Top and Bottom walls

	wall5->TransformTranslation(movementUp);
	wall5->TransformRotation(rotate90CCWAroundX);
	wall5->SetCollisionBox(1.0f, 0.2f, 1.0f);
	wall5->UpdateEntity(); // Top

	wall6->TransformTranslation(movementDown);
	wall6->TransformRotation(rotate90CWAroundX);
	wall6->SetCollisionBox(1.0f, 0.2f, 1.0f);
	wall6->UpdateEntity(); // Bottom

						   //Hallway 4

	wall7->TransformTranslation(movementForward);
	wall7->TransformTranslation(movementForward);
	wall7->TransformTranslation(movementUp);
	wall7->TransformRotation(rotate90CCWAroundX);
	wall7->SetCollisionBox(1.0f, 0.2f, 1.0f);
	wall7->UpdateEntity(); // Ceiling

	wall8->TransformTranslation(movementForward);
	wall8->TransformTranslation(movementForward);
	wall8->TransformTranslation(movementDown);
	wall8->TransformRotation(rotate90CWAroundX);
	wall8->SetCollisionBox(1.0f, 0.2f, 1.0f);
	wall8->UpdateEntity(); // Floor

	wall9->TransformTranslation(movementForward);
	wall9->TransformTranslation(movementForward);
	wall9->TransformTranslation(movementLeft);
	wall9->TransformRotation(rotate90CCWAroundY);
	wall9->SetCollisionBox(0.2f, 1.0f, 1.0f);
	wall9->UpdateEntity(); // Left

	wall10->TransformTranslation(movementForward);
	wall10->TransformTranslation(movementForward);
	wall10->TransformTranslation(movementRight);
	wall10->TransformRotation(rotate90CWAroundY);
	wall10->SetCollisionBox(0.2f, 1.0f, 1.0f);
	wall10->UpdateEntity(); // Right
}
void Game::CreateShaftGeometry(){

	// Shaft Floor 1

	wall11->TransformTranslation(movementForward);
	wall11->TransformTranslation(movementForward);
	wall11->TransformTranslation(movementForward);
	wall11->TransformTranslation(movementForward);
	wall11->TransformTranslation(movementDown);
	wall11->TransformRotation(rotate90CWAroundX);
	wall11->SetCollisionBox(1.0f, 0.2f, 1.0f);
	wall11->UpdateEntity(); // floor

	wall12->TransformTranslation(movementForward);
	wall12->TransformTranslation(movementForward);
	wall12->TransformTranslation(movementForward);
	wall12->TransformTranslation(movementForward);
	wall12->TransformTranslation(movementLeft);
	wall12->TransformRotation(rotate90CCWAroundY);
	wall12->SetCollisionBox(0.2f, 1.0f, 0.2f);
	wall12->UpdateEntity(); // left

	wall13->TransformTranslation(movementForward);
	wall13->TransformTranslation(movementForward);
	wall13->TransformTranslation(movementForward);
	wall13->TransformTranslation(movementForward);
	wall13->TransformTranslation(movementRight);
	wall13->TransformRotation(rotate90CWAroundY);
	wall13->SetCollisionBox(0.2f, 1.0f, 0.2f);
	wall13->UpdateEntity(); // right

							// Shaft Floor 2

	wall14->TransformTranslation(movementForward);
	wall14->TransformTranslation(movementForward);
	wall14->TransformTranslation(movementForward);
	wall14->TransformTranslation(movementForward);
	wall14->TransformTranslation(movementUp);
	wall14->TransformTranslation(movementUp);
	wall14->TransformTranslation(movementLeft);
	wall14->TransformRotation(rotate90CCWAroundY);
	wall14->SetCollisionBox(0.2f, 1.0f, 0.2f);
	wall14->UpdateEntity(); //left 2

	wall15->TransformTranslation(movementForward);
	wall15->TransformTranslation(movementForward);
	wall15->TransformTranslation(movementForward);
	wall15->TransformTranslation(movementForward);
	wall15->TransformTranslation(movementUp);
	wall15->TransformTranslation(movementUp);
	wall15->TransformTranslation(movementRight);
	wall15->TransformRotation(rotate90CWAroundY);
	wall15->SetCollisionBox(0.2f, 1.0f, 0.2f);
	wall15->UpdateEntity(); //right 2

	wall16->TransformTranslation(movementForward);
	wall16->TransformTranslation(movementForward);
	wall16->TransformTranslation(movementForward);
	//wall16->TransformTranslation(movementForward);
	wall16->TransformTranslation(movementUp);
	wall16->TransformTranslation(movementUp);
	wall16->TransformRotation(rotation180AroundY);
	wall16->SetCollisionBox(0.2f, 1.0f, 0.2f);
	wall16->UpdateEntity(); // back floor 2

	wall17->TransformTranslation(movementForward);
	wall17->TransformTranslation(movementForward);
	wall17->TransformTranslation(movementForward);
	wall17->TransformTranslation(movementUp);
	wall17->TransformTranslation(movementUp);
	wall17->TransformTranslation(movementUp);
	wall17->TransformTranslation(movementUp);
	wall17->TransformRotation(rotation180AroundY);
	wall17->SetCollisionBox(0.2f, 1.0f, 0.2f);
	wall17->UpdateEntity(); // back floor 3



	wall18->TransformTranslation(movementForward);
	wall18->TransformTranslation(movementForward);
	wall18->TransformTranslation(movementForward);
	wall18->TransformTranslation(movementForward);
	wall18->TransformTranslation(movementUp);
	wall18->TransformTranslation(movementUp);
	wall18->TransformTranslation(movementUp);
	wall18->TransformTranslation(movementUp);
	wall18->TransformTranslation(movementUp);

	wall18->TransformRotation(rotate90CCWAroundX);
	wall18->SetCollisionBox(1.0f, 0.2f, 1.0f);
	wall18->UpdateEntity(); // Top 3

	wall19->TransformTranslation(movementForward);
	wall19->TransformTranslation(movementForward);
	wall19->TransformTranslation(movementForward);
	wall19->TransformTranslation(movementForward);
	wall19->TransformTranslation(movementForward);
	wall19->TransformTranslation(movementUp);
	wall19->TransformTranslation(movementUp);
	wall19->SetCollisionBox(1.0f, 1.0f, 0.2f);
	wall19->UpdateEntity(); // Front 2 - checked
}
void Game::CreateAreneaGeometry(){
	// Floor 2 area





	wall20->TransformTranslation(movementForward);
	wall20->TransformTranslation(movementForward);
	wall20->TransformTranslation(movementForward);
	wall20->TransformTranslation(movementForward);
	wall20->TransformTranslation(movementUp);
	wall20->TransformTranslation(movementUp);
	wall20->TransformTranslation(movementUp);
	wall20->TransformTranslation(movementUp);

	wall20->TransformTranslation(movementRight);
	wall20->TransformRotation(rotate90CWAroundY);
	wall20->SetCollisionBox(1.0f, 1.0f, 0.2f);
	wall20->UpdateEntity(); // right 3

	wall21->TransformTranslation(movementForward);
	wall21->TransformTranslation(movementForward);
	wall21->TransformTranslation(movementForward);
	wall21->TransformTranslation(movementForward);
	wall21->TransformTranslation(movementUp);
	wall21->TransformTranslation(movementUp);
	wall21->TransformTranslation(movementUp);
	wall21->TransformTranslation(movementUp);
	wall21->TransformTranslation(movementLeft);
	wall21->TransformRotation(rotate90CCWAroundY);
	wall21->SetCollisionBox(1.0f, 0.2f, 1.0f);
	wall21->UpdateEntity(); // left 3



							//Floor 3 space
	arenaF11->TransformTranslation(movementForward);
	arenaF11->TransformTranslation(movementForward);
	arenaF11->TransformTranslation(movementForward);
	arenaF11->TransformTranslation(movementForward); //down the hallway
	arenaF11->TransformTranslation(movementForward); //forward 1
	arenaF11->TransformTranslation(movementForward); //forward 1
	arenaF11->TransformTranslation(movementUp);
	//arenaF11->TransformTranslation(movementUp);
	arenaF11->TransformTranslation(movementUp);
	arenaF11->TransformTranslation(movementUp); //up the shaft
	arenaF11->TransformRotation(rotate90CWAroundX);
	arenaF11->UpdateEntity();

	arenaF12->TransformTranslation(movementForward);
	arenaF12->TransformTranslation(movementForward);
	arenaF12->TransformTranslation(movementForward);
	arenaF12->TransformTranslation(movementForward); //down the hallway
	arenaF12->TransformTranslation(movementForward); //forward 1
	arenaF12->TransformTranslation(movementForward); //forward 1
	arenaF12->TransformTranslation(movementUp);
	//arenaF11->TransformTranslation(movementUp);
	arenaF12->TransformTranslation(movementUp);
	arenaF12->TransformTranslation(movementUp); //up the shaft
	arenaF12->TransformTranslation(movementLeft);
	arenaF12->TransformTranslation(movementLeft);
	arenaF12->TransformRotation(rotate90CWAroundX);
	arenaF12->UpdateEntity();

	arenaF13->TransformTranslation(movementForward);
	arenaF13->TransformTranslation(movementForward);
	arenaF13->TransformTranslation(movementForward);
	arenaF13->TransformTranslation(movementForward); //down the hallway
	arenaF13->TransformTranslation(movementForward); //forward 1
	arenaF13->TransformTranslation(movementForward); //forward 1
	arenaF13->TransformTranslation(movementUp);
	//arena11->TransformTranslation(movementUp);
	arenaF13->TransformTranslation(movementUp);
	arenaF13->TransformTranslation(movementUp); //up the shaft
	arenaF13->TransformTranslation(movementRight);
	arenaF13->TransformTranslation(movementRight);
	arenaF13->TransformRotation(rotate90CWAroundX);
	arenaF13->UpdateEntity();

	arenaF14->TransformTranslation(movementForward);
	arenaF14->TransformTranslation(movementForward);
	arenaF14->TransformTranslation(movementForward);
	arenaF14->TransformTranslation(movementForward); //down the hallway
	arenaF14->TransformTranslation(movementForward); //forward 1
	arenaF14->TransformTranslation(movementForward); //forward 1
	arenaF14->TransformTranslation(movementUp);
	//arena41->TransformTranslation(movementUp);
	arenaF14->TransformTranslation(movementUp);
	arenaF14->TransformTranslation(movementUp); //up the shaft
	arenaF14->TransformTranslation(movementRight);
	arenaF14->TransformTranslation(movementRight);
	arenaF14->TransformTranslation(movementRight);
	arenaF14->TransformTranslation(movementRight);
	arenaF14->TransformRotation(rotate90CWAroundX);
	arenaF14->UpdateEntity();


	arenaF21->TransformTranslation(movementForward);
	arenaF21->TransformTranslation(movementForward);
	arenaF21->TransformTranslation(movementForward);
	arenaF21->TransformTranslation(movementForward);
	arenaF21->TransformTranslation(movementForward);
	arenaF21->TransformTranslation(movementForward); //down the hallway
	arenaF21->TransformTranslation(movementForward); //forward 1
	arenaF21->TransformTranslation(movementForward); //forward 1
	arenaF21->TransformTranslation(movementUp);
	//arenaF11->TransformTranslation(movementUp);
	arenaF21->TransformTranslation(movementUp);
	arenaF21->TransformTranslation(movementUp); //up the shaft
	arenaF21->TransformRotation(rotate90CWAroundX);
	arenaF21->UpdateEntity();

	arenaF22->TransformTranslation(movementForward);
	arenaF22->TransformTranslation(movementForward);
	arenaF22->TransformTranslation(movementForward);
	arenaF22->TransformTranslation(movementForward);
	arenaF22->TransformTranslation(movementForward);
	arenaF22->TransformTranslation(movementForward); //down the hallway
	arenaF22->TransformTranslation(movementForward); //forward 1
	arenaF22->TransformTranslation(movementForward); //forward 1
	arenaF22->TransformTranslation(movementUp);
	//arenaF11->TransformTranslation(movementUp);
	arenaF22->TransformTranslation(movementUp);
	arenaF22->TransformTranslation(movementUp); //up the shaft
	arenaF22->TransformTranslation(movementLeft);
	arenaF22->TransformTranslation(movementLeft);
	arenaF22->TransformRotation(rotate90CWAroundX);
	arenaF22->UpdateEntity();

	arenaF23->TransformTranslation(movementForward);
	arenaF23->TransformTranslation(movementForward);
	arenaF23->TransformTranslation(movementForward);
	arenaF23->TransformTranslation(movementForward);
	arenaF23->TransformTranslation(movementForward);
	arenaF23->TransformTranslation(movementForward); //down the hallway
	arenaF23->TransformTranslation(movementForward); //forward 1
	arenaF23->TransformTranslation(movementForward); //forward 1
	arenaF23->TransformTranslation(movementUp);
	//aren211->TransformTranslation(movementUp);
	arenaF23->TransformTranslation(movementUp);
	arenaF23->TransformTranslation(movementUp); //up the shaft
	arenaF23->TransformTranslation(movementRight);
	arenaF23->TransformTranslation(movementRight);
	arenaF23->TransformRotation(rotate90CWAroundX);
	arenaF23->UpdateEntity();

	arenaF24->TransformTranslation(movementForward);
	arenaF24->TransformTranslation(movementForward);
	arenaF24->TransformTranslation(movementForward);
	arenaF24->TransformTranslation(movementForward);
	arenaF24->TransformTranslation(movementForward);
	arenaF24->TransformTranslation(movementForward); //down the hallway
	arenaF24->TransformTranslation(movementForward); //forward 1
	arenaF24->TransformTranslation(movementForward); //forward 1
	arenaF24->TransformTranslation(movementUp);
	//aren241->TransformTranslation(movementUp);
	arenaF24->TransformTranslation(movementUp);
	arenaF24->TransformTranslation(movementUp); //up the shaft
	arenaF24->TransformTranslation(movementRight);
	arenaF24->TransformTranslation(movementRight);
	arenaF24->TransformTranslation(movementRight);
	arenaF24->TransformTranslation(movementRight);
	arenaF24->TransformRotation(rotate90CWAroundX);
	arenaF24->UpdateEntity();


	arenaF31->TransformTranslation(movementForward);
	arenaF31->TransformTranslation(movementForward);
	arenaF31->TransformTranslation(movementForward);
	arenaF31->TransformTranslation(movementForward);
	arenaF31->TransformTranslation(movementForward);
	arenaF31->TransformTranslation(movementForward);
	arenaF31->TransformTranslation(movementForward);
	arenaF31->TransformTranslation(movementForward); //down the hallway
	arenaF31->TransformTranslation(movementForward); //forward 1
	arenaF31->TransformTranslation(movementForward); //forward 1
	arenaF31->TransformTranslation(movementUp);
	//aren3F11->TransformTranslation(movementUp);
	arenaF31->TransformTranslation(movementUp);
	arenaF31->TransformTranslation(movementUp); //up the shaft
	arenaF31->TransformRotation(rotate90CWAroundX);
	arenaF31->UpdateEntity();

	arenaF32->TransformTranslation(movementForward);
	arenaF32->TransformTranslation(movementForward);
	arenaF32->TransformTranslation(movementForward);
	arenaF32->TransformTranslation(movementForward);
	arenaF32->TransformTranslation(movementForward);
	arenaF32->TransformTranslation(movementForward);
	arenaF32->TransformTranslation(movementForward);
	arenaF32->TransformTranslation(movementForward); //down the hallway
	arenaF32->TransformTranslation(movementForward); //forward 1
	arenaF32->TransformTranslation(movementForward); //forward 1
	arenaF32->TransformTranslation(movementUp);
	//aren3F11->TransformTranslation(movementUp);
	arenaF32->TransformTranslation(movementUp);
	arenaF32->TransformTranslation(movementUp); //up the shaft
	arenaF32->TransformTranslation(movementLeft);
	arenaF32->TransformTranslation(movementLeft);
	arenaF32->TransformRotation(rotate90CWAroundX);
	arenaF32->UpdateEntity();

	arenaF33->TransformTranslation(movementForward);
	arenaF33->TransformTranslation(movementForward);
	arenaF33->TransformTranslation(movementForward);
	arenaF33->TransformTranslation(movementForward);
	arenaF33->TransformTranslation(movementForward);
	arenaF33->TransformTranslation(movementForward);
	arenaF33->TransformTranslation(movementForward);
	arenaF33->TransformTranslation(movementForward); //down the hallway
	arenaF33->TransformTranslation(movementForward); //forward 1
	arenaF33->TransformTranslation(movementForward); //forward 1
	arenaF33->TransformTranslation(movementUp);
	//aren311->TransformTranslation(movementUp);
	arenaF33->TransformTranslation(movementUp);
	arenaF33->TransformTranslation(movementUp); //up the shaft
	arenaF33->TransformTranslation(movementRight);
	arenaF33->TransformTranslation(movementRight);
	arenaF33->TransformRotation(rotate90CWAroundX);
	arenaF33->UpdateEntity();

	arenaF34->TransformTranslation(movementForward);
	arenaF34->TransformTranslation(movementForward);
	arenaF34->TransformTranslation(movementForward);
	arenaF34->TransformTranslation(movementForward);
	arenaF34->TransformTranslation(movementForward);
	arenaF34->TransformTranslation(movementForward);
	arenaF34->TransformTranslation(movementForward);
	arenaF34->TransformTranslation(movementForward); //down the hallway
	arenaF34->TransformTranslation(movementForward); //forward 1
	arenaF34->TransformTranslation(movementForward); //forward 1
	arenaF34->TransformTranslation(movementUp);
	//aren341->TransformTranslation(movementUp);
	arenaF34->TransformTranslation(movementUp);
	arenaF34->TransformTranslation(movementUp); //up the shaft
	arenaF34->TransformTranslation(movementRight);
	arenaF34->TransformTranslation(movementRight);
	arenaF34->TransformTranslation(movementRight);
	arenaF34->TransformTranslation(movementRight);
	arenaF34->TransformRotation(rotate90CWAroundX);
	arenaF34->UpdateEntity();


	arenaL1->TransformTranslation(movementForward);
	arenaL1->TransformTranslation(movementForward);
	arenaL1->TransformTranslation(movementForward);
	arenaL1->TransformTranslation(movementForward);
	arenaL1->TransformTranslation(movementForward);
	arenaL1->TransformTranslation(movementForward);
	arenaL1->TransformTranslation(movementUp);
	arenaL1->TransformTranslation(movementUp);
	arenaL1->TransformTranslation(movementUp);
	arenaL1->TransformTranslation(movementUp);
	arenaL1->TransformTranslation(movementLeft);
	arenaL1->TransformTranslation(movementLeft);
	arenaL1->TransformTranslation(movementLeft);
	arenaL1->TransformRotation(rotate90CCWAroundY);
	arenaL1->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaL1->UpdateEntity();

	arenaL2->TransformTranslation(movementForward);
	arenaL2->TransformTranslation(movementForward);
	arenaL2->TransformTranslation(movementForward);
	arenaL2->TransformTranslation(movementForward);
	arenaL2->TransformTranslation(movementForward);
	arenaL2->TransformTranslation(movementForward);
	arenaL2->TransformTranslation(movementForward);
	arenaL2->TransformTranslation(movementForward);
	arenaL2->TransformTranslation(movementUp);
	arenaL2->TransformTranslation(movementUp);
	arenaL2->TransformTranslation(movementUp);
	arenaL2->TransformTranslation(movementUp);
	arenaL2->TransformTranslation(movementLeft);
	arenaL2->TransformTranslation(movementLeft);
	arenaL2->TransformTranslation(movementLeft);
	arenaL2->TransformRotation(rotate90CCWAroundY);
	arenaL2->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaL2->UpdateEntity();

	arenaL3->TransformTranslation(movementForward);
	arenaL3->TransformTranslation(movementForward);
	arenaL3->TransformTranslation(movementForward);
	arenaL3->TransformTranslation(movementForward);
	arenaL3->TransformTranslation(movementForward);
	arenaL3->TransformTranslation(movementForward);
	arenaL3->TransformTranslation(movementForward);
	arenaL3->TransformTranslation(movementForward);
	arenaL3->TransformTranslation(movementForward);
	arenaL3->TransformTranslation(movementForward);
	arenaL3->TransformTranslation(movementUp);
	arenaL3->TransformTranslation(movementUp);
	arenaL3->TransformTranslation(movementUp);
	arenaL3->TransformTranslation(movementUp);
	arenaL3->TransformTranslation(movementLeft);
	arenaL3->TransformTranslation(movementLeft);
	arenaL3->TransformTranslation(movementLeft);
	arenaL3->TransformRotation(rotate90CCWAroundY);
	arenaL3->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaL3->UpdateEntity();

	arenaL4->TransformTranslation(movementForward);
	arenaL4->TransformTranslation(movementForward);
	arenaL4->TransformTranslation(movementForward);
	arenaL4->TransformTranslation(movementForward);
	arenaL4->TransformTranslation(movementForward);
	arenaL4->TransformTranslation(movementForward);
	arenaL4->TransformTranslation(movementForward);
	arenaL4->TransformTranslation(movementForward);
	arenaL4->TransformTranslation(movementForward);
	arenaL4->TransformTranslation(movementForward);
	arenaL4->TransformTranslation(movementForward);
	arenaL4->TransformTranslation(movementForward);
	arenaL4->TransformTranslation(movementUp);
	arenaL4->TransformTranslation(movementUp);
	arenaL4->TransformTranslation(movementUp);
	arenaL4->TransformTranslation(movementUp);
	arenaL4->TransformTranslation(movementLeft);
	arenaL4->TransformTranslation(movementLeft);
	arenaL4->TransformTranslation(movementLeft);
	arenaL4->TransformRotation(rotate90CCWAroundY);
	arenaL4->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaL4->UpdateEntity();


	arenaR1->TransformTranslation(movementForward);
	arenaR1->TransformTranslation(movementForward);
	arenaR1->TransformTranslation(movementForward);
	arenaR1->TransformTranslation(movementForward);
	arenaR1->TransformTranslation(movementForward);
	arenaR1->TransformTranslation(movementForward);
	arenaR1->TransformTranslation(movementUp);
	arenaR1->TransformTranslation(movementUp);
	arenaR1->TransformTranslation(movementUp);
	arenaR1->TransformTranslation(movementUp);
	arenaR1->TransformTranslation(movementRight);
	arenaR1->TransformTranslation(movementRight);
	arenaR1->TransformTranslation(movementRight);
	arenaR1->TransformTranslation(movementRight);
	arenaR1->TransformTranslation(movementRight);
	arenaR1->TransformRotation(rotate90CWAroundY);
	arenaR1->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaR1->UpdateEntity();

	arenaR2->TransformTranslation(movementForward);
	arenaR2->TransformTranslation(movementForward);
	arenaR2->TransformTranslation(movementForward);
	arenaR2->TransformTranslation(movementForward);
	arenaR2->TransformTranslation(movementForward);
	arenaR2->TransformTranslation(movementForward);
	arenaR2->TransformTranslation(movementForward);
	arenaR2->TransformTranslation(movementForward);
	arenaR2->TransformTranslation(movementUp);
	arenaR2->TransformTranslation(movementUp);
	arenaR2->TransformTranslation(movementUp);
	arenaR2->TransformTranslation(movementUp);
	arenaR2->TransformTranslation(movementRight);
	arenaR2->TransformTranslation(movementRight);
	arenaR2->TransformTranslation(movementRight);
	arenaR2->TransformTranslation(movementRight);
	arenaR2->TransformTranslation(movementRight);
	arenaR2->TransformRotation(rotate90CWAroundY);
	arenaR2->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaR2->UpdateEntity();

	arenaR3->TransformTranslation(movementForward);
	arenaR3->TransformTranslation(movementForward);
	arenaR3->TransformTranslation(movementForward);
	arenaR3->TransformTranslation(movementForward);
	arenaR3->TransformTranslation(movementForward);
	arenaR3->TransformTranslation(movementForward);
	arenaR3->TransformTranslation(movementForward);
	arenaR3->TransformTranslation(movementForward);
	arenaR3->TransformTranslation(movementForward);
	arenaR3->TransformTranslation(movementForward);
	arenaR3->TransformTranslation(movementUp);
	arenaR3->TransformTranslation(movementUp);
	arenaR3->TransformTranslation(movementUp);
	arenaR3->TransformTranslation(movementUp);
	arenaR3->TransformTranslation(movementRight);
	arenaR3->TransformTranslation(movementRight);
	arenaR3->TransformTranslation(movementRight);
	arenaR3->TransformTranslation(movementRight);
	arenaR3->TransformTranslation(movementRight);
	arenaR3->TransformRotation(rotate90CWAroundY);
	arenaR3->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaR3->UpdateEntity();


	arenaB1->TransformTranslation(movementForward);
	arenaB1->TransformTranslation(movementForward);
	arenaB1->TransformTranslation(movementForward);
	arenaB1->TransformTranslation(movementForward);
	arenaB1->TransformTranslation(movementForward);
	arenaB1->TransformTranslation(movementUp);
	arenaB1->TransformTranslation(movementUp);
	arenaB1->TransformTranslation(movementUp);
	arenaB1->TransformTranslation(movementUp);
	arenaB1->TransformTranslation(movementLeft);
	arenaB1->TransformTranslation(movementLeft);
	arenaB1->TransformRotation(rotation180AroundY);
	arenaB1->SetCollisionBox(0.2f, 1.0f, 0.2f);
	arenaB1->UpdateEntity(); // back floor 3

	arenaB3->TransformTranslation(movementForward);
	arenaB3->TransformTranslation(movementForward);
	arenaB3->TransformTranslation(movementForward);
	arenaB3->TransformTranslation(movementForward);
	arenaB3->TransformTranslation(movementForward);
	arenaB3->TransformTranslation(movementUp);
	arenaB3->TransformTranslation(movementUp);
	arenaB3->TransformTranslation(movementUp);
	arenaB3->TransformTranslation(movementUp);
	arenaB3->TransformTranslation(movementRight);
	arenaB3->TransformTranslation(movementRight);
	arenaB3->TransformRotation(rotation180AroundY);
	arenaB3->SetCollisionBox(0.2f, 1.0f, 0.2f);
	arenaB3->UpdateEntity(); // back floor 3

	arenaB4->TransformTranslation(movementForward);
	arenaB4->TransformTranslation(movementForward);
	arenaB4->TransformTranslation(movementForward);
	arenaB4->TransformTranslation(movementForward);
	arenaB4->TransformTranslation(movementForward);
	arenaB4->TransformTranslation(movementUp);
	arenaB4->TransformTranslation(movementUp);
	arenaB4->TransformTranslation(movementUp);
	arenaB4->TransformTranslation(movementUp);
	arenaB4->TransformTranslation(movementRight);
	arenaB4->TransformTranslation(movementRight);
	arenaB4->TransformTranslation(movementRight);
	arenaB4->TransformTranslation(movementRight);
	arenaB4->TransformRotation(rotation180AroundY);
	arenaB4->SetCollisionBox(0.2f, 1.0f, 0.2f);
	arenaB4->UpdateEntity(); // back floor 3

	arenaT11->TransformTranslation(movementForward);
	arenaT11->TransformTranslation(movementForward);
	arenaT11->TransformTranslation(movementForward);
	arenaT11->TransformTranslation(movementForward);
	arenaT11->TransformTranslation(movementForward);
	arenaT11->TransformTranslation(movementForward);
	arenaT11->TransformTranslation(movementUp);
	arenaT11->TransformTranslation(movementUp);
	arenaT11->TransformTranslation(movementUp);
	arenaT11->TransformTranslation(movementUp);
	arenaT11->TransformTranslation(movementUp);
	arenaT11->TransformTranslation(movementLeft);
	arenaT11->TransformTranslation(movementLeft);
	arenaT11->TransformRotation(rotate90CCWAroundX);
	arenaT11->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaT11->UpdateEntity(); // Top 3

	arenaT12->TransformTranslation(movementForward);
	arenaT12->TransformTranslation(movementForward);
	arenaT12->TransformTranslation(movementForward);
	arenaT12->TransformTranslation(movementForward);
	arenaT12->TransformTranslation(movementForward);
	arenaT12->TransformTranslation(movementForward);
	arenaT12->TransformTranslation(movementUp);
	arenaT12->TransformTranslation(movementUp);
	arenaT12->TransformTranslation(movementUp);
	arenaT12->TransformTranslation(movementUp);
	arenaT12->TransformTranslation(movementUp);
	arenaT12->TransformRotation(rotate90CCWAroundX);
	arenaT12->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaT12->UpdateEntity(); // Top 3

	arenaT13->TransformTranslation(movementForward);
	arenaT13->TransformTranslation(movementForward);
	arenaT13->TransformTranslation(movementForward);
	arenaT13->TransformTranslation(movementForward);
	arenaT13->TransformTranslation(movementForward);
	arenaT13->TransformTranslation(movementForward);
	arenaT13->TransformTranslation(movementUp);
	arenaT13->TransformTranslation(movementUp);
	arenaT13->TransformTranslation(movementUp);
	arenaT13->TransformTranslation(movementUp);
	arenaT13->TransformTranslation(movementUp);
	arenaT13->TransformTranslation(movementRight);
	arenaT13->TransformTranslation(movementRight);
	arenaT13->TransformRotation(rotate90CCWAroundX);
	arenaT13->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaT13->UpdateEntity(); // Top 3

	arenaT14->TransformTranslation(movementForward);
	arenaT14->TransformTranslation(movementForward);
	arenaT14->TransformTranslation(movementForward);
	arenaT14->TransformTranslation(movementForward);
	arenaT14->TransformTranslation(movementForward);
	arenaT14->TransformTranslation(movementForward);
	arenaT14->TransformTranslation(movementUp);
	arenaT14->TransformTranslation(movementUp);
	arenaT14->TransformTranslation(movementUp);
	arenaT14->TransformTranslation(movementUp);
	arenaT14->TransformTranslation(movementUp);
	arenaT14->TransformTranslation(movementRight);
	arenaT14->TransformTranslation(movementRight);
	arenaT14->TransformTranslation(movementRight);
	arenaT14->TransformTranslation(movementRight);
	arenaT14->TransformRotation(rotate90CCWAroundX);
	arenaT14->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaT14->UpdateEntity(); // Top 3

	arenaT21->TransformTranslation(movementForward);
	arenaT21->TransformTranslation(movementForward);
	arenaT21->TransformTranslation(movementForward);
	arenaT21->TransformTranslation(movementForward);
	arenaT21->TransformTranslation(movementForward);
	arenaT21->TransformTranslation(movementForward);
	arenaT21->TransformTranslation(movementForward);
	arenaT21->TransformTranslation(movementForward);
	arenaT21->TransformTranslation(movementUp);
	arenaT21->TransformTranslation(movementUp);
	arenaT21->TransformTranslation(movementUp);
	arenaT21->TransformTranslation(movementUp);
	arenaT21->TransformTranslation(movementUp);
	arenaT21->TransformTranslation(movementLeft);
	arenaT21->TransformTranslation(movementLeft);
	arenaT21->TransformRotation(rotate90CCWAroundX);
	arenaT21->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaT21->UpdateEntity(); // Top 3

	arenaT22->TransformTranslation(movementForward);
	arenaT22->TransformTranslation(movementForward);
	arenaT22->TransformTranslation(movementForward);
	arenaT22->TransformTranslation(movementForward);
	arenaT22->TransformTranslation(movementForward);
	arenaT22->TransformTranslation(movementForward);
	arenaT22->TransformTranslation(movementForward);
	arenaT22->TransformTranslation(movementForward);
	arenaT22->TransformTranslation(movementUp);
	arenaT22->TransformTranslation(movementUp);
	arenaT22->TransformTranslation(movementUp);
	arenaT22->TransformTranslation(movementUp);
	arenaT22->TransformTranslation(movementUp);
	arenaT22->TransformRotation(rotate90CCWAroundX);
	arenaT22->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaT22->UpdateEntity(); // Top 3

	arenaT23->TransformTranslation(movementForward);
	arenaT23->TransformTranslation(movementForward);
	arenaT23->TransformTranslation(movementForward);
	arenaT23->TransformTranslation(movementForward);
	arenaT23->TransformTranslation(movementForward);
	arenaT23->TransformTranslation(movementForward);
	arenaT23->TransformTranslation(movementForward);
	arenaT23->TransformTranslation(movementForward);
	arenaT23->TransformTranslation(movementUp);
	arenaT23->TransformTranslation(movementUp);
	arenaT23->TransformTranslation(movementUp);
	arenaT23->TransformTranslation(movementUp);
	arenaT23->TransformTranslation(movementUp);
	arenaT23->TransformTranslation(movementRight);
	arenaT23->TransformTranslation(movementRight);
	arenaT23->TransformRotation(rotate90CCWAroundX);
	arenaT23->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaT23->UpdateEntity(); // Top 3

	arenaT24->TransformTranslation(movementForward);
	arenaT24->TransformTranslation(movementForward);
	arenaT24->TransformTranslation(movementForward);
	arenaT24->TransformTranslation(movementForward);
	arenaT24->TransformTranslation(movementForward);
	arenaT24->TransformTranslation(movementForward);
	arenaT24->TransformTranslation(movementForward);
	arenaT24->TransformTranslation(movementForward);
	arenaT24->TransformTranslation(movementUp);
	arenaT24->TransformTranslation(movementUp);
	arenaT24->TransformTranslation(movementUp);
	arenaT24->TransformTranslation(movementUp);
	arenaT24->TransformTranslation(movementUp);
	arenaT24->TransformTranslation(movementRight);
	arenaT24->TransformTranslation(movementRight);
	arenaT24->TransformTranslation(movementRight);
	arenaT24->TransformTranslation(movementRight);
	arenaT24->TransformRotation(rotate90CCWAroundX);
	arenaT24->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaT24->UpdateEntity(); // Top 3

	arenaT31->TransformTranslation(movementForward);
	arenaT31->TransformTranslation(movementForward);
	arenaT31->TransformTranslation(movementForward);
	arenaT31->TransformTranslation(movementForward);
	arenaT31->TransformTranslation(movementForward);
	arenaT31->TransformTranslation(movementForward);
	arenaT31->TransformTranslation(movementForward);
	arenaT31->TransformTranslation(movementForward);
	arenaT31->TransformTranslation(movementForward);
	arenaT31->TransformTranslation(movementForward);
	arenaT31->TransformTranslation(movementUp);
	arenaT31->TransformTranslation(movementUp);
	arenaT31->TransformTranslation(movementUp);
	arenaT31->TransformTranslation(movementUp);
	arenaT31->TransformTranslation(movementUp);
	arenaT31->TransformTranslation(movementLeft);
	arenaT31->TransformTranslation(movementLeft);
	arenaT31->TransformRotation(rotate90CCWAroundX);
	arenaT31->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaT31->UpdateEntity(); // Top 3

	arenaT32->TransformTranslation(movementForward);
	arenaT32->TransformTranslation(movementForward);
	arenaT32->TransformTranslation(movementForward);
	arenaT32->TransformTranslation(movementForward);
	arenaT32->TransformTranslation(movementForward);
	arenaT32->TransformTranslation(movementForward);
	arenaT32->TransformTranslation(movementForward);
	arenaT32->TransformTranslation(movementForward);
	arenaT32->TransformTranslation(movementForward);
	arenaT32->TransformTranslation(movementForward);
	arenaT32->TransformTranslation(movementUp);
	arenaT32->TransformTranslation(movementUp);
	arenaT32->TransformTranslation(movementUp);
	arenaT32->TransformTranslation(movementUp);
	arenaT32->TransformTranslation(movementUp);
	arenaT32->TransformRotation(rotate90CCWAroundX);
	arenaT32->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaT32->UpdateEntity(); // Top 3

	arenaT33->TransformTranslation(movementForward);
	arenaT33->TransformTranslation(movementForward);
	arenaT33->TransformTranslation(movementForward);
	arenaT33->TransformTranslation(movementForward);
	arenaT33->TransformTranslation(movementForward);
	arenaT33->TransformTranslation(movementForward);
	arenaT33->TransformTranslation(movementForward);
	arenaT33->TransformTranslation(movementForward);
	arenaT33->TransformTranslation(movementForward);
	arenaT33->TransformTranslation(movementForward);
	arenaT33->TransformTranslation(movementUp);
	arenaT33->TransformTranslation(movementUp);
	arenaT33->TransformTranslation(movementUp);
	arenaT33->TransformTranslation(movementUp);
	arenaT33->TransformTranslation(movementUp);
	arenaT33->TransformTranslation(movementRight);
	arenaT33->TransformTranslation(movementRight);
	arenaT33->TransformRotation(rotate90CCWAroundX);
	arenaT33->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaT33->UpdateEntity(); // Top 3

	arenaT34->TransformTranslation(movementForward);
	arenaT34->TransformTranslation(movementForward);
	arenaT34->TransformTranslation(movementForward);
	arenaT34->TransformTranslation(movementForward);
	arenaT34->TransformTranslation(movementForward);
	arenaT34->TransformTranslation(movementForward);
	arenaT34->TransformTranslation(movementForward);
	arenaT34->TransformTranslation(movementForward);
	arenaT34->TransformTranslation(movementForward);
	arenaT34->TransformTranslation(movementForward);
	arenaT34->TransformTranslation(movementUp);
	arenaT34->TransformTranslation(movementUp);
	arenaT34->TransformTranslation(movementUp);
	arenaT34->TransformTranslation(movementUp);
	arenaT34->TransformTranslation(movementUp);
	arenaT34->TransformTranslation(movementRight);
	arenaT34->TransformTranslation(movementRight);
	arenaT34->TransformTranslation(movementRight);
	arenaT34->TransformTranslation(movementRight);
	arenaT34->TransformRotation(rotate90CCWAroundX);
	arenaT34->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaT34->UpdateEntity(); // Top 3



	arenaFW2->TransformTranslation(movementForward);
	arenaFW2->TransformTranslation(movementForward);
	arenaFW2->TransformTranslation(movementForward);
	arenaFW2->TransformTranslation(movementForward);
	arenaFW2->TransformTranslation(movementForward);
	arenaFW2->TransformTranslation(movementUp);
	arenaFW2->TransformTranslation(movementUp);
	arenaFW2->TransformTranslation(movementUp);
	arenaFW2->TransformTranslation(movementUp);
	arenaFW2->TransformTranslation(movementForward);
	arenaFW2->TransformTranslation(movementForward);
	arenaFW2->TransformTranslation(movementForward);
	arenaFW2->TransformTranslation(movementForward);
	arenaFW2->TransformTranslation(movementForward);
	arenaFW2->TransformTranslation(movementForward);
	arenaFW2->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaFW2->UpdateEntity(); // end cap

	arenaFW3->TransformTranslation(movementForward);
	arenaFW3->TransformTranslation(movementForward);
	arenaFW3->TransformTranslation(movementForward);
	arenaFW3->TransformTranslation(movementForward);
	arenaFW3->TransformTranslation(movementForward);
	arenaFW3->TransformTranslation(movementUp);
	arenaFW3->TransformTranslation(movementUp);
	arenaFW3->TransformTranslation(movementUp);
	arenaFW3->TransformTranslation(movementUp);
	arenaFW3->TransformTranslation(movementForward);
	arenaFW3->TransformTranslation(movementForward);
	arenaFW3->TransformTranslation(movementForward);
	arenaFW3->TransformTranslation(movementForward);
	arenaFW3->TransformTranslation(movementForward);
	arenaFW3->TransformTranslation(movementForward);
	arenaFW3->TransformTranslation(movementRight);
	arenaFW3->TransformTranslation(movementRight);
	arenaFW3->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaFW3->UpdateEntity(); // end cap

	arenaFW4->TransformTranslation(movementForward);
	arenaFW4->TransformTranslation(movementForward);
	arenaFW4->TransformTranslation(movementForward);
	arenaFW4->TransformTranslation(movementForward);
	arenaFW4->TransformTranslation(movementForward);
	arenaFW4->TransformTranslation(movementUp);
	arenaFW4->TransformTranslation(movementUp);
	arenaFW4->TransformTranslation(movementUp);
	arenaFW4->TransformTranslation(movementUp);
	arenaFW4->TransformTranslation(movementForward);
	arenaFW4->TransformTranslation(movementForward);
	arenaFW4->TransformTranslation(movementForward);
	arenaFW4->TransformTranslation(movementForward);
	arenaFW4->TransformTranslation(movementForward);
	arenaFW4->TransformTranslation(movementForward);
	arenaFW4->TransformTranslation(movementRight);
	arenaFW4->TransformTranslation(movementRight);
	arenaFW4->TransformTranslation(movementRight);
	arenaFW4->TransformTranslation(movementRight);
	arenaFW4->SetCollisionBox(1.0f, 0.2f, 1.0f);
	arenaFW4->UpdateEntity(); // end cap



	wallEnd->TransformTranslation(movementForward);
	wallEnd->TransformTranslation(movementForward);
	wallEnd->TransformTranslation(movementForward);
	wallEnd->TransformTranslation(movementForward);
	wallEnd->TransformTranslation(movementForward);
	wallEnd->TransformTranslation(movementUp);
	wallEnd->TransformTranslation(movementUp);
	wallEnd->TransformTranslation(movementUp);
	wallEnd->TransformTranslation(movementUp);
	wallEnd->TransformTranslation(movementLeft);
	wallEnd->TransformTranslation(movementLeft);
	wallEnd->TransformTranslation(movementForward);
	wallEnd->TransformTranslation(movementForward);
	wallEnd->TransformTranslation(movementForward);
	wallEnd->TransformTranslation(movementForward);
	wallEnd->TransformTranslation(movementForward);
	wallEnd->TransformTranslation(movementForward);
	wallEnd->SetCollisionBox(1.0f, 0.2f, 1.0f);
	wallEnd->UpdateEntity(); // end cap
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
	camera->UpdateProjectionMatrix((float)width, (float)height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
	XMFLOAT3 movementValue = XMFLOAT3(1.0f * deltaTime, 0.0f, 0.0f);
	XMFLOAT3 rotationValue = XMFLOAT3(0.0f, 20.0f * deltaTime, 0.0f);
	XMFLOAT3 scaleValue = XMFLOAT3(1.0f * deltaTime, 1.0f * deltaTime, 1.0f* deltaTime);
	//	printf("%d %d %d\n", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
	gePlayer->SetPosition(camera->GetPosition());
	//	printf("\nPlayer x: %f, y: %f, z: %f", gePlayer->GetPosition().x, gePlayer->GetPosition().y, gePlayer->GetPosition().z);
		//ge1->TransformRotation(rotationValue);
		//ge1->UpdateEntity();
		//ge2->TransformTranslation(movementValue);
		//ge2->UpdateEntity();
		//ge3->TransformScale(scaleValue);
		//ge3->UpdateEntity();
		//ge4->TransformRotation(rotationValue);
		//ge5->TransformTranslation(movementValue);
		//ge4->UpdateEntity();
	gePlayer->PrintPosition();

	e0->UpdateEntity(deltaTime);

	/*
	printf("\nPlayer position.x: %f", gePlayer->GetPosition().x);
	printf("\nPlayer position.y: %f", gePlayer->GetPosition().y);
	printf("\nPlayer position.z: %f", gePlayer->GetPosition().z);

	printf("\nEnemy position.x: %f", e0->GetPosition().x);
	printf("\nEnemy position.y: %f", e0->GetPosition().y);
	printf("\nEnemy position.z: %f", e0->GetPosition().z);
	*/
	for (int i = 0; i <= entityArraySize - 1; i++) {
		//gePla
		//printf("\nObject: %f\n", i);
		entityArray[i]->UpdateEntity();
		//printf("\nPlayer position.x %f", gePlayer->GetColliderPosition().x);
		//printf("\nPlayer position.y %f", gePlayer->GetColliderPosition().y);
		//printf("\nPlayer position.z %f", gePlayer->GetColliderPosition().z);

		if (collidingMaster.isColliding(gePlayer, entityArray[i])) {
			collidingMaster.isColliding(gePlayer, entityArray[i]);
			printf("collision between player and %d\n", i);
		}
		else {
			//printf("no collision between player and %d\n", i);
		}

		//if(i != )
	}
	for (int i = 0; i <= collisionArraySize - 1; i++) {
		collisionArray[i]->UpdateEntity();
		if (collidingMaster.isColliding(gePlayer, collisionArray[i])) {
			collidingMaster.isColliding(gePlayer, entityArray[i]);
			printf("Collision between player and collider volume %d\n", i);
		}
		else {
			//printf("no Collision between player and collider volume %d\n", i);
		}
	}

	emitter->Update(deltaTime);

	camera->Update(deltaTime);
	//ge5->Move(movementValue);
//	ge5->UpdateEntity();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

#pragma region Render Shadow Map

	// Set shadow map render target
	context->OMSetRenderTargets(0, 0, shadowDepth);
	// Set the shadow map depth view
	context->ClearDepthStencilView(shadowDepth, D3D11_CLEAR_DEPTH, 1.0f, 0);
	// Set the shadow rasterizer
	context->RSSetState(shadowRasterizer);

	// We need a viewport that matches the shadow map resolution
	D3D11_VIEWPORT shadowVP = {};
	shadowVP.TopLeftX = 0;
	shadowVP.TopLeftY = 0;
	shadowVP.Width = shadowVP.Height = 1024.0f;
	shadowVP.MinDepth = 0.0f;
	shadowVP.MaxDepth = 1.0f;
	context->RSSetViewports(1, &shadowVP);

	// Set the shadow vertex shader
	shadowShader->SetShader();
	shadowShader->SetMatrix4x4("view", shadowView);
	shadowShader->SetMatrix4x4("projection", shadowProjection);

	// Turn OFF the pixel shader
	context->PSSetShader(0, 0, 0);

	// Draw the enemy in the shader
	ID3D11Buffer* vert = e0->GetVertBuffer();
	context->IASetVertexBuffers(0, 1, &vert, &stride, &offset);
	context->IASetIndexBuffer(e0->GetIndBuffer(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(e0->GetIndCount(), 0, 0);
	shadowShader->SetMatrix4x4("world", e0->GetWorldMatrix());
	shadowShader->CopyAllBufferData();

	for (int i = 0; i < e0->NumBullets(); i++) {
		vert = e0->Bullets()[i].GetVertBuffer();
		context->IASetVertexBuffers(0, 1, &vert, &stride, &offset);
		context->IASetIndexBuffer(e0->Bullets()[i].GetIndBuffer(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(e0->Bullets()[i].GetIndCount(), 0, 0);
		shadowShader->SetMatrix4x4("world", e0->Bullets()[i].GetWorldMatrix());
		shadowShader->CopyAllBufferData();
	}

	for (int i = 0; i < entityArraySize; i++) {
		if (entityArray[i] != nullptr) {

			// Set buffers in the input assembler
			ID3D11Buffer* vertexHolder = entityArray[i]->GetVertBuffer();
			context->IASetVertexBuffers(0, 1, &vertexHolder, &stride, &offset);
			context->IASetIndexBuffer(entityArray[i]->GetIndBuffer(), DXGI_FORMAT_R32_UINT, 0);

			// Set the world position data
			shadowShader->SetMatrix4x4("world", entityArray[i]->GetWorldMatrix());
			shadowShader->CopyAllBufferData();

			// Finally do the actual drawing
			context->DrawIndexed(entityArray[i]->GetIndCount(),	0, 0);
		}
	}

	// Reset it to the default render target
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
	// Set it to the standard rasterizer
	context->RSSetState(0);
	// Set it back to the default viewport
	shadowVP.Width = (float)this->width;
	shadowVP.Height = (float)this->height;
	context->RSSetViewports(1, &shadowVP);
#pragma endregion

#pragma region Render Full Scene

	pixelShader->SetData(
		"light",
		&dLight,
		sizeof(DirectionalLight));
	pixelShader->SetData(
		"light2",
		&dLight2,
		sizeof(DirectionalLight));


	// Linear Fog
	// vertexShader->SetFloat("fogStart", 0.5f);
	// vertexShader->SetFloat("fogEnd", 3.0f);

	// Exponential Fog
	vertexShader->SetFloat("fogDensity", 0.26f);

	// Fog Color
	pixelShader->SetFloat4("fogColor", { 0.5f, 0.5f, 0.5f, 1.0f });

	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	//vertexShader->SetMatrix4x4("world", worldMatrix);
	vertexShader->SetMatrix4x4("shadowView", shadowView);
	vertexShader->SetMatrix4x4("shadowProjection", shadowProjection);

	// Draw the enemy
	pixelShader->SetShaderResourceView("shadowSRV", shadowSRV);
	pixelShader->SetSamplerState("shadowSampler", shadowSampler);
	e0->PrepareMaterial(camera->GetCamMatrix(), camera->GetProjectionMatrix());
	vert = e0->GetVertBuffer();
	context->IASetVertexBuffers(0, 1, &vert, &stride, &offset);
	context->IASetIndexBuffer(e0->GetIndBuffer(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(e0->GetIndCount(), 0, 0);

	// Draw enemy bullets
	for (int i = 0; i < e0->NumBullets(); i++) {
		pixelShader->SetShaderResourceView("shadowSRV", shadowSRV);
		pixelShader->SetSamplerState("shadowSampler", shadowSampler);
		e0->Bullets()[i].PrepareMaterial(camera->GetCamMatrix(), camera->GetProjectionMatrix());
		vert = e0->Bullets()[i].GetVertBuffer();
		context->IASetVertexBuffers(0, 1, &vert, &stride, &offset);
		context->IASetIndexBuffer(e0->Bullets()[i].GetIndBuffer(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(e0->Bullets()[i].GetIndCount(), 0, 0);
	}

	// Draw the entity array
	for (int i = 0; i <= entityArraySize - 1; i++) {
		if (entityArray[i] != '\0') {

			// Pass in the shadow data
			pixelShader->SetShaderResourceView("shadowSRV", shadowSRV);
			pixelShader->SetSamplerState("shadowSampler", shadowSampler);

			// Prepare all the mateiral data for the shaders
			entityArray[i]->PrepareMaterial(camera->GetCamMatrix(), camera->GetProjectionMatrix());

			// Set buffers in the input assembler
			//  - Do this ONCE PER OBJECT you're drawing, since each object might
			//    have different geometry.
			ID3D11Buffer* vertexHolder = entityArray[i]->GetVertBuffer();
			context->IASetVertexBuffers(0, 1, &vertexHolder, &stride, &offset);
			context->IASetIndexBuffer(entityArray[i]->GetIndBuffer(), DXGI_FORMAT_R32_UINT, 0);

			// Finally do the actual drawing
			//  - Do this ONCE PER OBJECT you intend to draw
			//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
			//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
			//     vertices in the currently set VERTEX BUFFER
			context->DrawIndexed(
				entityArray[i]->GetIndCount(),
				0,
				0);
		}
	}

	// Particle states
	float blend[4] = { 1, 1, 1, 1 }; // RGBA
	context->OMSetBlendState(particleBlender, blend, 0xffffffff);	// Additive blending
	context->OMSetDepthStencilState(particleDS, 0);					// No depth WRITING
	emitter->Draw(context, camera);									// Draw the emitter

#pragma endregion

	// Reset any states we've changed for the next frame!
	context->OMSetBlendState(0, blend, 0xffffffff);
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);

	// Be sure to unbind any shader resources for the next frame
	ID3D11ShaderResourceView* nullSRV[16] = {};
	context->PSSetShaderResources(0, 16, nullSRV);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	//prevMousePos.x = x;
	//prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	//camera->MouseRotateX(x - (float)prevMousePos.x);
	//camera->MouseRotateY(y - (float)prevMousePos.y);
	// Save the previous mouse position, so we have it for the future
	//prevMousePos.x = x;
	//prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}

#pragma endregion