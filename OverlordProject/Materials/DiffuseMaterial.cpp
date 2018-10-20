#include "./base/stdafx.h"
#include "Graphics/TextureData.h"
#include "DiffuseMaterial.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"

ID3DX11EffectShaderResourceVariable* DiffuseMaterial::m_pDiffuseSRVvariable = nullptr;

DiffuseMaterial::DiffuseMaterial()
	:Material(L"./Resources/Effects/PosNormTex3D.fx")

{
}


DiffuseMaterial::~DiffuseMaterial()
{
}

void DiffuseMaterial::SetDiffuseTexture(const wstring & assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);

}

void DiffuseMaterial::LoadEffectVariables()
{
	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"DiffuseMaterial::LoadEffectVariables() > \'gDiffuseMap\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	}
}

void DiffuseMaterial::UpdateEffectVariables(const GameContext & gameContext, ModelComponent * pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pDiffuseTexture && m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}
}
