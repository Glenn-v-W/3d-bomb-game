//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"
#include "PostSepia.h"
#include "Graphics/RenderTarget.h"

ID3DX11EffectShaderResourceVariable* PostSepia::m_pTextureMapVariabele = nullptr;

PostSepia::PostSepia()
	: PostProcessingMaterial(L"./Resources/Effects/Post/Sepia.fx", L"Saturation")
{
}

PostSepia::~PostSepia(void)
{
}

void PostSepia::LoadEffectVariables()
{
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
	m_pTextureMapVariabele = m_pEffect->GetVariableByName("gTexture")->AsShaderResource();
	if (!m_pTextureMapVariabele->IsValid())
		Logger::LogWarning(L"PostSepia: GetVariableByName 'gTexture' not valid!");

}

void PostSepia::UpdateEffectVariables(RenderTarget* rendertarget)
{
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
	m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
}