#pragma once
#include <d3d11.h>
#include "DXCore.h"
#include "Vertex.h"
#include "DirectXMath.h"
#include <fstream>
//#include <vector>
#include <vector>
using namespace DirectX;


class Mesh {
public:
	Mesh(Vertex* vertArray, int vertCount, unsigned int* indexArray, int indexCt, ID3D11Device* bufferMaker);
	Mesh(char* fileName, ID3D11Device* bufferMaker);
	~Mesh();
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();
	void createBuffers(Vertex* vertArray, unsigned int* indices, ID3D11Device* bufferMaker);

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int indexCount;
	int vertexCount;
};