#include "EffectHelper.h"

#include <fstream>

Effect::Effect(ID3D11Device* device, const std::string& filename)
	:pFx(nullptr)
{
	/**the another way to using the fxo file*/
	std::ifstream fxFile(filename, std::ios::binary);
	fxFile.seekg(0, std::ios_base::end);
	int size = (int)fxFile.tellg();
	fxFile.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fxFile.read(&compiledShader[0], size);
	fxFile.close();

	HRESULT hr=D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, device, &pFx);
	if (FAILED(hr))
	{
		printf_s("read effect file error");
	}
}

Effect::Effect(const Effect&rhp)
{
	//
	pFx = rhp.pFx;
}

Effect::~Effect()
{

}

Effect& Effect::operator=(const Effect& rhp)
{
	//
	return *this;
}

BasicEffect::BasicEffect(ID3D11Device*device, const std::string filename)
	:Effect(device,filename)
{
	pTech = pFx->GetTechniqueByName("LightTech");

	fxWorld = pFx->GetVariableByName("gWorld")->AsMatrix();
	fxWInTranspose = pFx->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	fxWViewProj = pFx->GetVariableByName("gWorldViewProj")->AsMatrix();
	EyePos = pFx->GetVariableByName("gEyePos")->AsVector();

	fxDlight = pFx->GetVariableByName("gDLight");
	fxSkullMaterial = pFx->GetVariableByName("gMaterial");

}

BasicEffect::~BasicEffect()
{

}
