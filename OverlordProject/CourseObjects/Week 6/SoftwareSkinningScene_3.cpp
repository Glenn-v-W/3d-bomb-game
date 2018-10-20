//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"

#include "SoftwareSkinningScene_3.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "Prefabs/SkyBoxPrefab.h"
#include "Diagnostics\DebugRenderer.h"
#include "Physx\PhysxProxy.h"
#include "..\..\Materials\ColorMaterial.h"

SoftwareSkinningScene_3::SoftwareSkinningScene_3(void) :
	GameScene(L"SoftwareSkinningScene_3"),
	m_pBone0(nullptr),
	m_pBone1(nullptr),
	m_BoneRotation(0.0f),
	m_RotationSign(1)
{
}


SoftwareSkinningScene_3::~SoftwareSkinningScene_3(void)
{
}

void SoftwareSkinningScene_3::Initialize(const GameContext& gameContext)
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

void SoftwareSkinningScene_3::Update(const GameContext& gameContext)
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
		XMFLOAT4 startPos{ 
			m_SkinnedVertices[i].OriginalVertex.Position.x,
			m_SkinnedVertices[i].OriginalVertex.Position.y, 
			m_SkinnedVertices[i].OriginalVertex.Position.z, 
			1 };

		XMFLOAT4 bone0Pos{ 0,0,0,0 };
		XMFLOAT4 bone1Pos{ 0,0,0,0 };
		if (m_SkinnedVertices.at(i).BlendWeight0 != 0)
		{
			XMStoreFloat4(&bone0Pos, XMVector4Transform(XMLoadFloat4(&startPos), XMLoadFloat4x4(&bone0Transform)));
			bone0Pos.x *= m_SkinnedVertices.at(i).BlendWeight0;
			bone0Pos.y *= m_SkinnedVertices.at(i).BlendWeight0;
			bone0Pos.z *= m_SkinnedVertices.at(i).BlendWeight0;
		}
		if (m_SkinnedVertices.at(i).BlendWeight1 != 0)
		{
			XMStoreFloat4(&bone1Pos, XMVector4Transform(XMLoadFloat4(&startPos), XMLoadFloat4x4(&bone1Transform)));
			bone1Pos.x *= m_SkinnedVertices.at(i).BlendWeight1;
			bone1Pos.y *= m_SkinnedVertices.at(i).BlendWeight1;
			bone1Pos.z *= m_SkinnedVertices.at(i).BlendWeight1;
		}

		m_SkinnedVertices[i].TransformedVertex.Position.x = bone0Pos.x + bone1Pos.x;
		m_SkinnedVertices[i].TransformedVertex.Position.y = bone0Pos.y + bone1Pos.y;
		m_SkinnedVertices[i].TransformedVertex.Position.z = bone0Pos.z + bone1Pos.z;
	}
}

void SoftwareSkinningScene_3::Draw(const GameContext& gameContext)
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

void SoftwareSkinningScene_3::CreateMesh(float length) {
	auto pos = XMFLOAT3(length / 2, 0, 0); 
	auto offset = XMFLOAT3(length / 2, 2.5f, 2.5f); 
	auto col = XMFLOAT4(1, 0, 0, 0.5f);
	//***** 
	//BOX1* 
	//*****

	//FRONT 
	auto norm = XMFLOAT3(0, 0, -1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	//BACK 
	norm = XMFLOAT3(0, 0, 1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//TOP 
	norm = XMFLOAT3(0, 1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	//BOTTOM 
	norm = XMFLOAT3(0, -1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 1.0f, 0.0f));
	//LEFT 
	norm = XMFLOAT3(-1, 0, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 1.0f, 0.0f));
	//RIGHT 
	norm = XMFLOAT3(1, 0, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
																																																													   //***** //BOX2* //*****
	col = XMFLOAT4(0, 1, 0, 0.5f); pos = XMFLOAT3(22.5f, 0, 0); 
	//FRONT 
	norm = XMFLOAT3(0, 0, -1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//BACK 
	norm = XMFLOAT3(0, 0, 1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f));
	//TOP 
	norm = XMFLOAT3(0, 1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//BOTTOM 
	norm = XMFLOAT3(0, -1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//LEFT 
	norm = XMFLOAT3(-1, 0, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//RIGHT 
	norm = XMFLOAT3(1, 0, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.0f, 1.0f));
}