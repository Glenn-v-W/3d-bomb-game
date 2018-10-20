//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"
#include "PostVignette.h"
#include "Graphics/RenderTarget.h"

ID3DX11EffectShaderResourceVariable* PostVignette::m_pTextureMapVariabele = nullptr;

PostVignette::PostVignette()
	: PostProcessingMaterial(L"./Resources/Effects/Post/Vignette.fx", L"Saturation")
{
}

PostVignette::~PostVignette(void)
{
}

void PostVignette::LoadEffectVariables()
{
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
	m_pTextureMapVariabele = m_pEffect->GetVariableByName("gTexture")->AsShaderResource();
	if (!m_pTextureMapVariabele->IsValid())
		Logger::LogWarning(L"PostSepia: GetVariableByName 'gTexture' not valid!");

}

void PostVignette::UpdateEffectVariables(RenderTarget* rendertarget)
{
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
	m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
}