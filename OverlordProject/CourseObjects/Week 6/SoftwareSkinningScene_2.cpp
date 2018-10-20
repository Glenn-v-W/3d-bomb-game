//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"

#include "SoftwareSkinningScene_2.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "Prefabs/SkyBoxPrefab.h"
#include "Diagnostics\DebugRenderer.h"
#include "Physx\PhysxProxy.h"
#include "..\..\Materials\ColorMaterial.h"

SoftwareSkinningScene_2::SoftwareSkinningScene_2(void) :
	GameScene(L"SoftwareSkinningScene_2"),
	m_pBone0(nullptr),
	m_pBone1(nullptr),
	m_BoneRotation(0.0f),
	m_RotationSign(1)
{
}


SoftwareSkinningScene_2::~SoftwareSkinningScene_2(void)
{
}

void SoftwareSkinningScene_2::Initialize(const GameContext& gameContext)
{
	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	UNREFERENCED_PARAMETER(gameContext);

	auto colorMaterial = new ColorMaterial(true);
	gameContext.pMaterialManager->AddMaterial(colorMaterial, 0);

	auto root = new GameObject();
	m_pBone0 = new BoneObject(0, 0, 15.0f);
	m_pBone1 = new BoneObject(1, 0, 15.0f);

	m_pBone0->AddBone(m_pBone1);
	root->AddChild(m_pBone0);
	
	root->GetTransform()->Rotate(0, -90, 0);

	AddChild(root);

	m_pBone0->CalculateBindPose();

	auto empty = new GameObject();
	m_pMeshDrawer = new MeshDrawComponent(24);
	empty->AddComponent(m_pMeshDrawer);
	AddChild(empty);

	CreateMesh(15.0f);
}

void SoftwareSkinningScene_2::Update(const GameContext& gameContext)
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

	XMFLOAT4X4 bone0Transform;
	XMFLOAT4X4 bone1Transform;
	XMStoreFloat4x4(&bone0Transform, XMMatrixMultiply(XMLoadFloat4x4(&m_pBone0->GetBindPose()), XMLoadFloat4x4(&m_pBone0->GetTransform()->GetWorld())));
	XMStoreFloat4x4(&bone1Transform, XMMatrixMultiply(XMLoadFloat4x4(&m_pBone1->GetBindPose()), XMLoadFloat4x4(&m_pBone1->GetTransform()->GetWorld())));

	for (size_t i = 0; i < m_SkinnedVertices.size(); i++)
	{
		auto& transformToUse = bone0Transform;

		if (i >= 24)
		{
			transformToUse = bone1Transform;
		}

		XMFLOAT4 currPos{ 
			m_SkinnedVertices[i].OriginalVertex.Position.x,
			m_SkinnedVertices[i].OriginalVertex.Position.y, 
			m_SkinnedVertices[i].OriginalVertex.Position.z, 
			1 };
		
		XMStoreFloat4(&currPos, XMVector4Transform(XMLoadFloat4(&currPos), XMLoadFloat4x4(&transformToUse)));
		m_SkinnedVertices[i].TransformedVertex.Position.x = currPos.x;
		m_SkinnedVertices[i].TransformedVertex.Position.y = currPos.y;
		m_SkinnedVertices[i].TransformedVertex.Position.z = currPos.z;
	}

}

void SoftwareSkinningScene_2::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pMeshDrawer->RemoveTriangles();
	for (size_t i = 0; i < m_SkinnedVertices.size(); i += 4)
	{
		m_pMeshDrawer->AddQuad(QuadPosNormCol(
			m_SkinnedVertices[i].TransformedVertex,
			m_SkinnedVertices[i + 1].TransformedVertex,
			m_SkinnedVertices[i + 2].TransformedVertex,
			m_SkinnedVertices[i + 3].TransformedVertex));
	}
	m_pMeshDrawer->UpdateBuffer();
}

void SoftwareSkinningScene_2::CreateMesh(float length) {
	auto pos = XMFLOAT3(length / 2, 0, 0); 
	auto offset = XMFLOAT3(length / 2, 2.5f, 2.5f); 
	auto col = XMFLOAT4(1, 0, 0, 0.5f);
	//***** 
	//BOX1* 
	//*****

	//FRONT 
	auto norm = XMFLOAT3(0, 0, -1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//BACK 
	norm = XMFLOAT3(0, 0, 1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//TOP 
	norm = XMFLOAT3(0, 1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//BOTTOM 
	norm = XMFLOAT3(0, -1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//LEFT 
	norm = XMFLOAT3(-1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//RIGHT 
	norm = XMFLOAT3(1, 0, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
																																																													   //***** //BOX2* //*****
	col = XMFLOAT4(0, 1, 0, 0.5f); pos = XMFLOAT3(22.5f, 0, 0); 
	//FRONT 
	norm = XMFLOAT3(0, 0, -1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//BACK 
	norm = XMFLOAT3(0, 0, 1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//TOP 
	norm = XMFLOAT3(0, 1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//BOTTOM 
	norm = XMFLOAT3(0, -1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//LEFT 
	norm = XMFLOAT3(-1, 0, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//RIGHT 
	norm = XMFLOAT3(1, 0, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
}