//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"

#include "ModelTestScene.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "..\..\Materials\ColorMaterial.h"
#include "..\..\Materials\DiffuseMaterial.h"


ModelTestScene::ModelTestScene(void) :
	GameScene(L"ModelTestScene"),
	m_pChair(nullptr)
{
}


ModelTestScene::~ModelTestScene(void)
{
}

void ModelTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);


	//GROUND PLANE
	//************
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	//visualize physx
	physX->getVisualDebugger();

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(ground);

	//CHAIR OBJECT
	//************
	m_pChair = new GameObject();

	//1. Attach a modelcomponent (chair.ovm)
	m_pChair->AddComponent(new ModelComponent(L"Resources/chair.ovm"));
	//2. Create a ColorMaterial and add it to the material manager
	auto colorMaterial = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(colorMaterial, 0);
	//3. Assign the material to the previous created modelcomponent
	m_pChair->GetComponent<ModelComponent>()->SetMaterial(0);
	// Build and Run

	//4. Create a DiffuseMaterial (using PosNormTex3D.fx)
	// Make sure you are able to set a texture (DiffuseMaterial::SetDiffuseTexture(const wstring& assetFile))
	// Load the correct shadervariable and set it during the material variable update
	auto diffuseMaterial = new DiffuseMaterial();
	diffuseMaterial->SetDiffuseTexture(L"Resources/Textures/Chair_Dark.dds");
	//5. Assign the material to the modelcomponent
	gameContext.pMaterialManager->AddMaterial(diffuseMaterial, 1);
	m_pChair->GetComponent<ModelComponent>()->SetMaterial(1);

	//5. Assign the material to the modelcomponent
	// Build and Run
	m_pChair->AddComponent(new RigidBodyComponent());
	//6. Attach a rigidbody component (pure-dynamic)
	//7. Attach a collider component (Use a PxConvexMeshGeometry [chair.ovpc])
	auto pMesh = ContentManager::Load<PxConvexMesh>(L"Resources/chair.ovpc");
	std::shared_ptr<PxGeometry> geom2(new PxConvexMeshGeometry(pMesh));
	auto coolMaterial = physX->createMaterial(0.6f, 0.6f, 0.6f);
	m_pChair->AddComponent(new ColliderComponent(geom2, *coolMaterial));
	m_pChair->GetTransform()->Translate(0.f, 10.f, 0.f);
	AddChild(m_pChair);
	// Build and Run

}

void ModelTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ModelTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
