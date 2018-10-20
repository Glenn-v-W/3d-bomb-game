#include "./base/stdafx.h"
#include "Graphics/TextureData.h"
#include "UberMaterial.h"
#include "Content/ContentManager.h"

ID3DX11EffectVectorVariable* UberMaterial::m_pLightDirectionVariable = nullptr;
ID3DX11EffectScalarVariable*UberMaterial::m_pUseDiffuseTextureVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pDiffuseColorVariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pSpecularColorVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseSpecularLevelTextureVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pSpecularLevelSRVvariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pShininessVariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pAmbientColorVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pAmbientIntensityVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFlipGreenChannelVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseNormalMappingVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pNormalMappingSRVvariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseEnvironmentMappingVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pEnvironmentSRVvariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pReflectionStrengthVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionStrengthVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionIndexVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pOpacityVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseOpacityMapVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pOpacitySRVvariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseBlinnVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUsePhongVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseFresnelFalloffVariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pFresnelColorVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelPowerVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelMultiplierVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelHardnessVariable = nullptr;


UberMaterial::UberMaterial()
	:Material(L"./Resources/Effects/UberShader.fx")
{
}

UberMaterial::~UberMaterial()
{
}

void UberMaterial::SetLightDirection(XMFLOAT3 direction)
{
	m_LightDirection = direction;
}

void UberMaterial::EnableDiffuseTexture(bool enable)
{
	m_bDiffuseTexture = enable;
}

void UberMaterial::SetDiffuseTexture(const wstring & assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetDiffuseColor(XMFLOAT4 color)
{
	m_ColorDiffuse = color;
}

void UberMaterial::SetSpecularColor(XMFLOAT4 color)
{
	m_ColorSpecular = color;
}

void UberMaterial::EnableSpecularLevelTexture(bool enable)
{
	m_bSpecularLevelTexture = enable;
}

void UberMaterial::SetSpecularLevelTexture(const wstring & assetFile)
{
	m_pSpecularLevelTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetShininess(int shininess)
{
	m_Shininess = shininess;
}

void UberMaterial::SetAmbientColor(XMFLOAT4 color)
{
	m_ColorAmbient = color;
}

void UberMaterial::SetAmbientIntensity(float intensity)
{
	m_AmbientIntensity = intensity;
}

void UberMaterial::FlipNormalGreenCHannel(bool flip)
{
	m_bFlipGreenChannel = flip;
}

void UberMaterial::EnableNormalMapping(bool enable)
{
	m_bNormalMapping = enable;
}

void UberMaterial::SetNormalMapTexture(const wstring & assetFile)
{
	m_pNormalMappingTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::EnableEnvironmentMapping(bool enable)
{
	m_bEnvironmentMapping = enable;
}

void UberMaterial::SetEnvironmentCube(const wstring & assetFile)
{
	m_pEnvironmentCube = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetReflectionStrength(float strength)
{
	m_ReflectionStrength = strength;
}

void UberMaterial::SetRefractionStrength(float strength)
{
	m_RefractionStrength = strength;
}

void UberMaterial::SetRefractionIndex(float index)
{
	m_RefractionIndex = index;
}

void UberMaterial::SetOpacity(float opacity)
{
	m_Opacity = opacity;
}

void UberMaterial::EnableOpacityMap(bool enable)
{
	m_bOpacityMap = enable;
}

void UberMaterial::SetOpacityTexture(const wstring & assetFile)
{
	m_pOpacityMap = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::EnableSpecularBlinn(bool enable)
{
	m_bSpecularBlinn = enable;
}

void UberMaterial::EnableSpecularPhong(bool enable)
{
	m_bSpecularPhong = enable;
}

void UberMaterial::EnableFresnelFaloff(bool enable)
{
	m_bFresnelFaloff = enable;
}

void UberMaterial::SetFresnelColor(XMFLOAT4 color)
{
	m_ColorFresnel = color;
}

void UberMaterial::SetFresnelPower(float power)
{
	m_FresnelPower = power;
}

void UberMaterial::SetFresnelMultiplier(float multiplier)
{
	m_FresnelMultiplier = multiplier;
}

void UberMaterial::SetFresnelHardness(float hardness)
{
	m_FresnelHardness = hardness;
}

void UberMaterial::LoadEffectVariables()
{
	if (!m_pLightDirectionVariable)
	{
		m_pLightDirectionVariable = m_pEffect->GetVariableByName("gLightDirection")->AsVector();
		if (!m_pLightDirectionVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gLightDirection\' variable not found!");
			m_pLightDirectionVariable = nullptr;
		}
	}

	if (!m_pUseDiffuseTextureVariable)
	{
		m_pUseDiffuseTextureVariable = m_pEffect->GetVariableByName("gUseTextureDiffuse")->AsScalar();
		if (!m_pUseDiffuseTextureVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gUseTextureDiffuse\' variable not found!");
			m_pUseDiffuseTextureVariable = nullptr;
		}
	}

	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gTextureDiffuse")->AsShaderResource();
		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"DiffuseMaterial::LoadEffectVariables() > \'gTextureDiffuse\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	}

	if (!m_pDiffuseColorVariable)
	{
		m_pDiffuseColorVariable = m_pEffect->GetVariableByName("gColorDiffuse")->AsVector();
		if (!m_pDiffuseColorVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gColorDiffuse\' variable not found!");
			m_pDiffuseColorVariable = nullptr;
		}
	}

	if (!m_pSpecularColorVariable)
	{
		m_pSpecularColorVariable = m_pEffect->GetVariableByName("gColorSpecular")->AsVector();
		if (!m_pSpecularColorVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gColorSpecular\' variable not found!");
			m_pSpecularColorVariable = nullptr;
		}
	}

	if (!m_pUseSpecularLevelTextureVariable)
	{
		m_pUseSpecularLevelTextureVariable = m_pEffect->GetVariableByName("gUseTextureSpecularIntensity")->AsScalar();
		if (!m_pUseSpecularLevelTextureVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gUseTextureSpecularIntensity\' variable not found!");
			m_pUseSpecularLevelTextureVariable = nullptr;
		}
	}

	if (!m_pSpecularLevelSRVvariable)
	{
		m_pSpecularLevelSRVvariable = m_pEffect->GetVariableByName("gTextureSpecularIntensity")->AsShaderResource();
		if (!m_pSpecularLevelSRVvariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gTextureSpecularIntensity\' variable not found!");
			m_pSpecularLevelSRVvariable = nullptr;
		}
	}

	if (!m_pShininessVariable)
	{
		m_pShininessVariable = m_pEffect->GetVariableByName("gShininess")->AsScalar();
		if (!m_pShininessVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gShininess\' variable not found!");
			m_pShininessVariable = nullptr;
		}
	}

	if (!m_pAmbientColorVariable)
	{
		m_pAmbientColorVariable = m_pEffect->GetVariableByName("gColorAmbient")->AsVector();
		if (!m_pAmbientColorVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gColorAmbient\' variable not found!");
			m_pAmbientColorVariable = nullptr;
		}
	}

	if (!m_pAmbientIntensityVariable)
	{
		m_pAmbientIntensityVariable = m_pEffect->GetVariableByName("gAmbientIntensity")->AsScalar();
		if (!m_pAmbientIntensityVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gAmbientIntensity\' variable not found!");
			m_pAmbientIntensityVariable = nullptr;
		}
	}

	if (!m_pFlipGreenChannelVariable)
	{
		m_pFlipGreenChannelVariable = m_pEffect->GetVariableByName("gFlipGreenChannel")->AsScalar();
		if (!m_pFlipGreenChannelVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gFlipGreenChannel\' variable not found!");
			m_pFlipGreenChannelVariable = nullptr;
		}
	}

	if (!m_pUseNormalMappingVariable)
	{
		m_pUseNormalMappingVariable = m_pEffect->GetVariableByName("gUseTextureNormal")->AsScalar();
		if (!m_pUseNormalMappingVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gUseTextureNormal\' variable not found!");
			m_pUseNormalMappingVariable = nullptr;
		}
	}

	if (!m_pNormalMappingSRVvariable)
	{
		m_pNormalMappingSRVvariable = m_pEffect->GetVariableByName("gTextureNormal")->AsShaderResource();
		if (!m_pNormalMappingSRVvariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gTextureNormal\' variable not found!");
			m_pNormalMappingSRVvariable = nullptr;
		}
	}

	if (!m_pUseEnvironmentMappingVariable)
	{
		m_pUseEnvironmentMappingVariable = m_pEffect->GetVariableByName("gUseTextureEnvironment")->AsScalar();
		if (!m_pUseEnvironmentMappingVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gUseTextureEnvironment\' variable not found!");
			m_pUseEnvironmentMappingVariable = nullptr;
		}
	}

	if (!m_pEnvironmentSRVvariable)
	{
		m_pEnvironmentSRVvariable = m_pEffect->GetVariableByName("gCubeEnvironment")->AsShaderResource();
		if (!m_pEnvironmentSRVvariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gCubeEnvironment\' variable not found!");
			m_pEnvironmentSRVvariable = nullptr;
		}
	}

	if (!m_pReflectionStrengthVariable)
	{
		m_pReflectionStrengthVariable = m_pEffect->GetVariableByName("gReflectionStrength")->AsScalar();
		if (!m_pReflectionStrengthVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gReflectionStrength\' variable not found!");
			m_pReflectionStrengthVariable = nullptr;
		}
	}

	if (!m_pRefractionStrengthVariable)
	{
		m_pRefractionStrengthVariable = m_pEffect->GetVariableByName("gRefractionStrength")->AsScalar();
		if (!m_pRefractionStrengthVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gRefractionStrength\' variable not found!");
			m_pRefractionStrengthVariable = nullptr;
		}
	}

	if (!m_pRefractionIndexVariable)
	{
		m_pRefractionIndexVariable = m_pEffect->GetVariableByName("gRefractionIndex")->AsScalar();
		if (!m_pRefractionIndexVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gRefractionIndex\' variable not found!");
			m_pRefractionIndexVariable = nullptr;
		}
	}

	if (!m_pOpacityVariable)
	{
		m_pOpacityVariable = m_pEffect->GetVariableByName("gOpacityIntensity")->AsScalar();
		if (!m_pOpacityVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gOpacityIntensity\' variable not found!");
			m_pOpacityVariable = nullptr;
		}
	}

	if (!m_pUseOpacityMapVariable)
	{
		m_pUseOpacityMapVariable = m_pEffect->GetVariableByName("gUseTextureOpacity")->AsScalar();
		if (!m_pUseOpacityMapVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gUseTextureOpacity\' variable not found!");
			m_pUseOpacityMapVariable = nullptr;
		}
	}

	if (!m_pOpacitySRVvariable)
	{
		m_pOpacitySRVvariable = m_pEffect->GetVariableByName("gTextureOpacity")->AsShaderResource();
		if (!m_pOpacitySRVvariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gTextureOpacity\' variable not found!");
			m_pOpacitySRVvariable = nullptr;
		}
	}

	if (!m_pUseBlinnVariable)
	{
		m_pUseBlinnVariable = m_pEffect->GetVariableByName("gUseSpecularBlinn")->AsScalar();
		if (!m_pUseBlinnVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gUseSpecularBlinn\' variable not found!");
			m_pUseBlinnVariable = nullptr;
		}
	}

	if (!m_pUsePhongVariable)
	{
		m_pUsePhongVariable = m_pEffect->GetVariableByName("gUseSpecularPhong")->AsScalar();
		if (!m_pUsePhongVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gUseSpecularPhong\' variable not found!");
			m_pUsePhongVariable = nullptr;
		}
	}

	if (!m_pUseFresnelFalloffVariable)
	{
		m_pUseFresnelFalloffVariable = m_pEffect->GetVariableByName("gUseFresnelFalloff")->AsScalar();
		if (!m_pUseFresnelFalloffVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gUseFresnelFalloff\' variable not found!");
			m_pUseFresnelFalloffVariable = nullptr;
		}
	}

	if (!m_pFresnelColorVariable)
	{
		m_pFresnelColorVariable = m_pEffect->GetVariableByName("gColorFresnel")->AsVector();
		if (!m_pFresnelColorVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gColorFresnel\' variable not found!");
			m_pFresnelColorVariable = nullptr;
		}
	}

	if (!m_pFresnelPowerVariable)
	{
		m_pFresnelPowerVariable = m_pEffect->GetVariableByName("gFresnelPower")->AsScalar();
		if (!m_pFresnelPowerVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gFresnelPower\' variable not found!");
			m_pFresnelPowerVariable = nullptr;
		}
	}

	if (!m_pFresnelMultiplierVariable)
	{
		m_pFresnelMultiplierVariable = m_pEffect->GetVariableByName("gFresnelMultiplier")->AsScalar();
		if (!m_pFresnelMultiplierVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gFresnelMultiplier\' variable not found!");
			m_pFresnelMultiplierVariable = nullptr;
		}
	}

	if (!m_pFresnelHardnessVariable)
	{
		m_pFresnelHardnessVariable = m_pEffect->GetVariableByName("gFresnelHardness")->AsScalar();
		if (!m_pFresnelHardnessVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'gFresnelHardness\' variable not found!");
			m_pFresnelHardnessVariable = nullptr;
		}
	}

}

void UberMaterial::UpdateEffectVariables(const GameContext & gameContext, ModelComponent * pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
	//LIGHT
	//*****
	if (m_LightDirection.x && m_LightDirection.y && m_LightDirection.z && m_pLightDirectionVariable)
	{
		vector<float> floatVct(3);
		floatVct[0] = m_LightDirection.x;
		floatVct[1] = m_LightDirection.y;
		floatVct[2] = m_LightDirection.z;
		m_pLightDirectionVariable->SetFloatVector(&floatVct[0]);
	}

	//DIFFUSE
	//*******
	if (m_bDiffuseTexture && m_pUseDiffuseTextureVariable)
	{
		m_pUseDiffuseTextureVariable->SetBool(m_bDiffuseTexture);
	}
	if (m_pDiffuseTexture && m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}
	if (m_ColorDiffuse.x && m_ColorDiffuse.y && m_ColorDiffuse.z && m_ColorDiffuse.w && m_pDiffuseColorVariable)
	{
		vector<float> floatVct(4);
		floatVct[0] = m_ColorDiffuse.x;
		floatVct[1] = m_ColorDiffuse.y;
		floatVct[2] = m_ColorDiffuse.z;
		floatVct[3] = m_ColorDiffuse.w;
		m_pDiffuseColorVariable->SetFloatVector(&floatVct[0]);
	}

	//SPECULAR
	//********
	if (m_ColorSpecular.x && m_ColorSpecular.y && m_ColorSpecular.z && m_ColorSpecular.w && m_pSpecularColorVariable)
	{
		vector<float> floatVct(4);
		floatVct[0] = m_ColorSpecular.x;
		floatVct[1] = m_ColorSpecular.y;
		floatVct[2] = m_ColorSpecular.z;
		floatVct[3] = m_ColorSpecular.w;
		m_pSpecularColorVariable->SetFloatVector(&floatVct[0]);
	}
	if (m_bSpecularLevelTexture && m_pUseSpecularLevelTextureVariable)
	{
		m_pUseSpecularLevelTextureVariable->SetBool(m_bSpecularLevelTexture);
	}
	if (m_pSpecularLevelTexture && m_pSpecularLevelSRVvariable)
	{
		m_pSpecularLevelSRVvariable->SetResource(m_pSpecularLevelTexture->GetShaderResourceView());
	}
	if (m_Shininess && m_pShininessVariable)
	{
		m_pShininessVariable->SetInt(m_Shininess);
	}

	//AMBIENT
	//*******
	if (m_ColorAmbient.x && m_ColorAmbient.y && m_ColorAmbient.z && m_ColorAmbient.w && m_pAmbientColorVariable)
	{
		vector<float> floatVct(4);
		floatVct[0] = m_ColorAmbient.x;
		floatVct[1] = m_ColorAmbient.y;
		floatVct[2] = m_ColorAmbient.z;
		floatVct[3] = m_ColorAmbient.w;
		m_pAmbientColorVariable->SetFloatVector(&floatVct[0]);
	}
	if (m_AmbientIntensity && m_pAmbientIntensityVariable)
	{
		m_pAmbientIntensityVariable->SetFloat(m_AmbientIntensity);
	}

	//NORMAL MAPPING
	//**************
	if (m_bFlipGreenChannel && m_pFlipGreenChannelVariable)
	{
		m_pFlipGreenChannelVariable->SetBool(m_bFlipGreenChannel);
	}
	if (m_bNormalMapping && m_pUseNormalMappingVariable)
	{
		m_pUseNormalMappingVariable->SetBool(m_bNormalMapping);
	}
	if (m_pNormalMappingTexture && m_pNormalMappingSRVvariable)
	{
		m_pNormalMappingSRVvariable->SetResource(m_pNormalMappingTexture->GetShaderResourceView());
	}

	//ENVIRONMENT MAPPING
	//*******************
	if (m_bEnvironmentMapping && m_pUseEnvironmentMappingVariable)
	{
		m_pUseEnvironmentMappingVariable->SetBool(m_bEnvironmentMapping);
	}
	if (m_pEnvironmentCube && m_pEnvironmentSRVvariable)
	{
		m_pEnvironmentSRVvariable->SetResource(m_pEnvironmentCube->GetShaderResourceView());
	}
	if (m_ReflectionStrength && m_pReflectionStrengthVariable)
	{
		m_pReflectionStrengthVariable->SetFloat(m_ReflectionStrength);
	}
	if (m_RefractionStrength && m_pRefractionStrengthVariable)
	{
		m_pRefractionStrengthVariable->SetFloat(m_RefractionStrength);
	}
	if (m_RefractionIndex && m_pRefractionIndexVariable)
	{
		m_pRefractionIndexVariable->SetFloat(m_RefractionIndex);
	}

	//OPACITY
	//***************
	if (m_Opacity && m_pOpacityVariable)
	{
		m_pOpacityVariable->SetFloat(m_Opacity);
	}
	if (m_bOpacityMap && m_pUseOpacityMapVariable)
	{
		m_pUseOpacityMapVariable->SetBool(m_bOpacityMap);
	}
	if (m_pOpacityMap && m_pOpacitySRVvariable)
	{
		m_pOpacitySRVvariable->SetResource(m_pOpacityMap->GetShaderResourceView());
	}

	//SPECULAR MODELS
	//***************
	if (m_bSpecularBlinn && m_pUseBlinnVariable)
	{
		m_pUseBlinnVariable->SetBool(m_bSpecularBlinn);
	}
	if (m_bSpecularPhong && m_pUsePhongVariable)
	{
		m_pUsePhongVariable->SetBool(m_bSpecularPhong);
	}

	//FRESNEL FALLOFF
	//***************
	if (m_bFresnelFaloff && m_pUseFresnelFalloffVariable)
	{
		m_pUseFresnelFalloffVariable->SetBool(m_bFresnelFaloff);
	}
	if (m_ColorFresnel.x && m_ColorFresnel.y && m_ColorFresnel.z && m_ColorFresnel.w && m_pFresnelColorVariable)
	{
		vector<float> floatVct(4);
		floatVct[0] = m_ColorFresnel.x;
		floatVct[1] = m_ColorFresnel.y;
		floatVct[2] = m_ColorFresnel.z;
		floatVct[3] = m_ColorFresnel.w;
		m_pFresnelColorVariable->SetFloatVector(&floatVct[0]);
	}
	if (m_FresnelPower && m_pFresnelPowerVariable)
	{
		m_pFresnelPowerVariable->SetFloat(m_FresnelPower);
	}
	if (m_FresnelMultiplier && m_pFresnelMultiplierVariable)
	{
		m_pFresnelMultiplierVariable->SetFloat(m_FresnelMultiplier);
	}
	if (m_FresnelHardness && m_pFresnelHardnessVariable)
	{
		m_pFresnelHardnessVariable->SetFloat(m_FresnelHardness);
	}
}
