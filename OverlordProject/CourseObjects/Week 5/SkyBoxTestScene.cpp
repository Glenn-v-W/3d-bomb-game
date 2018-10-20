//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"

#include "SkyBoxTestScene.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "Prefabs/SkyBoxPrefab.h"
#include "Diagnostics\DebugRenderer.h"
#include "Physx\PhysxProxy.h"

SkyBoxTestScene::SkyBoxTestScene(void) :
	GameScene(L"SkyBoxTestScene"),
	m_pSkyBox(nullptr)
{
}


SkyBoxTestScene::~SkyBoxTestScene(void)
{
}

void SkyBoxTestScene::Initialize(const GameContext& gameContext)
{
	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	UNREFERENCED_PARAMETER(gameContext);

	//GROUND PLANE
	//************
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(ground);

	//SKYBOX OBJECT
	//************
	SkyBoxPrefab * m_pSkyBox = new SkyBoxPrefab();
	AddChild(m_pSkyBox);

	m_pTerrain = new GameObject();
	TerrainComponent * pTerrainComp = new TerrainComponent(L"./Resources/Terrain/Hawai_HeightMap_128x128x16.raw",
		L"./Resources/Terrain/Hawai_TexMap.dds", 128, 128, 400, 400, 62.5f);

	//Test
	//TerrainComponent * pTerrainComp = new TerrainComponent(L"./Resources/Terrain/Hawai_HeightMap_128x128x16.raw",
	//	L"./Resources/Terrain/Hawai_Yellow.dds", 128, 128, 400, 400, 100);
	m_pTerrain->AddComponent(pTerrainComp);
	AddChild(m_pTerrain);
}

void SkyBoxTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SkyBoxTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
