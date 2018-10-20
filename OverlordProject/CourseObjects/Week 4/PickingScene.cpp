//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"

#include "PickingScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"

#include "Prefabs\Prefabs.h"
#include "Components\Components.h"
#include "Physx\PhysxProxy.h"
#include "Physx\PhysxManager.h"
#include "Content\ContentManager.h"
#include "Graphics\MeshFilter.h"
#include "../../Materials/SpikeyMaterial.h"
#include "Components\ModelComponent.h"


#define FPS_COUNTER 1

PickingScene::PickingScene(void) :
	GameScene(L"PickingScene"),
	m_FpsInterval(FPS_COUNTER)
{
}


PickingScene::~PickingScene(void)
{
}

void PickingScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	// Create PhysX ground plane
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(ground);


	////Material Test
	//auto myMaterial = new SpikeyMaterial();
	//myMaterial->SetDiffuseTexture(L"./Resources/Textures/Chair_Dark.dds");
	//gameContext.pMaterialManager->AddMaterial(myMaterial, 60);

	//auto model = new ModelComponent(L"./Resources/Meshes/Teapot.ovm");
	//model->SetMaterial(60);

	//m_pTeapot = new GameObject();
	//m_pTeapot->AddComponent(model);


	//AddChild(m_pTeapot);
	//m_pTeapot->GetTransform()->Translate(0, 0, 20);


	//Initialise cube
	m_pBox = new CubePrefab(1.0f, 2.0f, 1.0f);

	RigidBodyComponent* pRigidBody = new RigidBodyComponent();
	m_pBox->AddComponent(pRigidBody);

	std::shared_ptr<PxGeometry> spGeometry = std::make_shared<PxBoxGeometry>(0.5f, 1.0f, 0.5f);
	m_pBox->AddComponent(new ColliderComponent(spGeometry, *bouncyMaterial));

	AddChild(m_pBox);




	gameContext.pInput->AddInputAction(InputAction(0, InputTriggerState::Pressed, -1, VK_LBUTTON));
}

void PickingScene::Update(const GameContext& gameContext)
{
	m_FpsInterval += gameContext.pGameTime->GetElapsed();
	if (m_FpsInterval >= FPS_COUNTER)
	{
		m_FpsInterval -= FPS_COUNTER;
		Logger::LogFormat(LogLevel::Info, L"FPS: %i", gameContext.pGameTime->GetFPS());
	}

	if (gameContext.pInput->IsActionTriggered(0))
	{
		//std::cout << "Calling it";
		auto hitObject = gameContext.pCamera->GetGameObject()->GetComponent<CameraComponent>()->Pick(gameContext);
		if (hitObject == m_pBox)
		{
			hitObject->GetTransform()->Translate(0.0f, 10.0f, 0.0f);
			cout << "test\n";
		}
	}


}

void PickingScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
