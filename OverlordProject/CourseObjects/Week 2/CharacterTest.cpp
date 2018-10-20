//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"

#include "CharacterTest.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "Physx/PhysxProxy.h"
#include "Components/ControllerComponent.h"
//#include "Prefabs/CharacterPrefab.h"

CharacterTest::CharacterTest(void) :
	GameScene(L"CharacterTest")
{
}


CharacterTest::~CharacterTest(void)
{
}

void CharacterTest::Initialize(const GameContext& gameContext)
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


	//m_pController = new GameObject();
	//m_pController->AddComponent(new ControllerComponent(bouncyMaterial));
	//m_pController->GetTransform()->Translate(0, 20, 0);
	//AddChild(m_pController);

	m_pCharacter = new Character();
	AddChild(m_pCharacter);

}

void CharacterTest::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

}

void CharacterTest::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
