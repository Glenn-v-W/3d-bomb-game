//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"

#include "SoftwareSkinningScene_1.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "Prefabs/SkyBoxPrefab.h"
#include "Diagnostics\DebugRenderer.h"
#include "Physx\PhysxProxy.h"
#include "..\..\Materials\ColorMaterial.h"

SoftwareSkinningScene_1::SoftwareSkinningScene_1(void) :
	GameScene(L"SoftwareSkinningScene_1"),
	m_pBone0(nullptr),
	m_pBone1(nullptr),
	m_BoneRotation(0.0f),
	m_RotationSign(1)
{
}


SoftwareSkinningScene_1::~SoftwareSkinningScene_1(void)
{
}

void SoftwareSkinningScene_1::Initialize(const GameContext& gameContext)
{
	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	UNREFERENCED_PARAMETER(gameContext);

	auto colorMaterial = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(colorMaterial, 0);

	auto root = new GameObject();
	m_pBone0 = new BoneObject(0, 0, 15.0f);
	m_pBone1 = new BoneObject(1, 0, 15.0f);

	m_pBone0->AddBone(m_pBone1);
	root->AddChild(m_pBone0);
	
	root->GetTransform()->Rotate(0, 90, 0);

	AddChild(root);
}

void SoftwareSkinningScene_1::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_BoneRotation += m_RotationSign * 45 * gameContext.pGameTime->GetElapsed();
	if ((m_RotationSign < 0) && (m_BoneRotation <= -45))
	{
		m_RotationSign = 1;
	}
	if ((m_RotationSign > 0) && (m_BoneRotation >= 45))
	{
		m_RotationSign = -1;
	}
	m_pBone0->GetTransform()->Rotate(m_BoneRotation,0,0);
	m_pBone1->GetTransform()->Rotate(-m_BoneRotation*2, 0, 0);

}

void SoftwareSkinningScene_1::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
