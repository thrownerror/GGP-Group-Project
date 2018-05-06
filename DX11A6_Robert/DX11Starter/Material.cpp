#include "Material.h"



Material::Material()
{
}

Material::Material(SimpleVertexShader * vP, SimplePixelShader * pP, ID3D11ShaderResourceView * srv, ID3D11ShaderResourceView * nrm, ID3D11SamplerState * ssP)
{
	vertPointer = vP;
	pixPointer = pP;
	shadResViewPointer = srv;
	normResViewPointer = nrm;
	samplerStatePointer = ssP;
}


Material::~Material()
{
	if (normResViewPointer != NULL) {
		normResViewPointer->Release();
	}
}

ID3D11SamplerState* ::Material::GetSamplerState() {
	return samplerStatePointer;
}
ID3D11ShaderResourceView* ::Material::GetSRV() {
	return shadResViewPointer;
}
ID3D11ShaderResourceView* ::Material::GetNormalRV() {
	return normResViewPointer;
}
SimpleVertexShader * Material::GetVertPointer()
{
	return vertPointer;
}

SimplePixelShader * Material::GetPixPointer()
{
	return pixPointer;
}
