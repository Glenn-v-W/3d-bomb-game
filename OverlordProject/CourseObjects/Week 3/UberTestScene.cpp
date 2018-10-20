//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"

#include "UberTestScene.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "..\..\Materials\ColorMaterial.h"
#include "..\..\Materials\UberMaterial.h"
#include "..\..\Materials\DiffuseMaterial.h"


UberTestScene::UberTestScene(void) :
	GameScene(L"UberTestScene"),
	m_pTeapot(nullptr)
{
}


UberTestScene::~UberTestScene(void)
{
}

void UberTestScene::Initialize(const GameContext& gameContext)
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
	m_pTeapot = new GameObject();

	m_pTeapot->AddComponent(new ModelComponent(L"Resources/Meshes/teapot.ovm"));

	auto material = new UberMaterial();
	material->SetLightDirection(XMFLOAT3(0.997f, 0.f, 0.083f));
	material->EnableDiffuseTexture(true);
	material->SetDiffuseColor(XMFLOAT4(1, 1, 1, 1));
	material->SetSpecularColor(XMFLOAT4(1, 1, 1, 1));
	material->EnableSpecularLevelTexture(false);
	material->SetShininess(15);
	material->SetAmbientColor(XMFLOAT4(0, 0, 0, 1));
	material->SetAmbientIntensity(0);
	material->FlipNormalGreenCHannel(false);
	material->EnableNormalMapping(false);
	material->EnableEnvironmentMapping(true);
	material->SetReflectionStrength(0.5f);
	material->SetRefractionStrength(0.f);
	material->SetRefractionIndex(0.3f);
	material->EnableFresnelFaloff(false);
	material->SetFresnelColor(XMFLOAT4(1, 1, 1, 1));
	material->SetFresnelPower(1);
	material->SetFresnelMultiplier(1);
	material->SetFresnelHardness(0);
	material->SetOpacity(1);
	material->EnableOpacityMap(false);
	material->EnableSpecularBlinn(false);
	material->EnableSpecularPhong(true);
	material->SetDiffuseTexture(L"Resources/Textures/Chair_Dark.dds"); 
	//material->SetDiffuseTexture(L"Resources/Textures/CobbleStone_DiffuseMap.dds"); 
	material->SetSpecularLevelTexture(L"Resources/Textures/CobbleStone_HeightMap.dds");
	material->SetNormalMapTexture(L"Resources/Textures/CobbleStone_NormalMap.dds");
	material->SetEnvironmentCube(L"Resources/Textures/Sunol_Cubemap.dds");
	material->SetOpacityTexture(L"Resources/Textures/Specular_Level.jpg");



	////5. Assign the material to the modelcomponent
	gameContext.pMaterialManager->AddMaterial(material, 1);
	m_pTeapot->GetComponent<ModelComponent>()->SetMaterial(1);

	////5. Assign the material to the modelcomponent
	//// Build and Run
	//m_pTeapot->AddComponent(new RigidBodyComponent());
	////6. Attach a rigidbody component (pure-dynamic)
	////7. Attach a collider component (Use a PxConvexMeshGeometry [chair.ovpc])
	//auto pMesh = ContentManager::Load<PxConvexMesh>(L"Resources/chair.ovpc");
	//std::shared_ptr<PxGeometry> geom2(new PxConvexMeshGeometry(pMesh));
	//auto coolMaterial = physX->createMaterial(0.6f, 0.6f, 0.6f);
	//m_pTeapot->AddComponent(new ColliderComponent(geom2, *coolMaterial));
	//m_pTeapot->GetTransform()->Translate(0.f, 10.f, 0.f);
	AddChild(m_pTeapot);
	// Build and Run

}

void UberTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void UberTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
