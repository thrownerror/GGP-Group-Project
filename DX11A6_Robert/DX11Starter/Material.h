#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
class Material
{
public:
	Material();
	Material(SimpleVertexShader* vP, SimplePixelShader* pP, ID3D11ShaderResourceView* srv, ID3D11ShaderResourceView* nrm, ID3D11SamplerState* ssP);
	~Material();
	SimpleVertexShader* GetVertPointer();
	SimplePixelShader* GetPixPointer();
	ID3D11ShaderResourceView* GetSRV();
	ID3D11ShaderResourceView* GetNormalRV();
	ID3D11SamplerState* GetSamplerState();
private:
	SimpleVertexShader * vertPointer;
	SimplePixelShader * pixPointer;
	ID3D11ShaderResourceView* shadResViewPointer;
	ID3D11ShaderResourceView* normResViewPointer;
	ID3D11SamplerState* samplerStatePointer;
};

