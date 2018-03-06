#include "Material.h"



Material::Material()
{
}

Material::Material(SimpleVertexShader * vP, SimplePixelShader * pP, ID3D11ShaderResourceView * srv, ID3D11SamplerState * ssP)
{
	vertPointer = vP;
	pixPointer = pP;
	shadResViewPointer = srv;
	samplerStatePointer = ssP;
}


Material::~Material()
{
}

ID3D11SamplerState* ::Material::GetSamplerState() {
	return samplerStatePointer;
}
ID3D11ShaderResourceView* ::Material::GetSRV() {
	return shadResViewPointer;
}
SimpleVertexShader * Material::GetVertPointer()
{
	return vertPointer;
}

SimplePixelShader * Material::GetPixPointer()
{
	return pixPointer;
}
