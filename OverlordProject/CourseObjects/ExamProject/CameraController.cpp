//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"

#include "CameraController.h"
#include "Components\Components.h"
#include "Prefabs\Prefabs.h"

CameraController::CameraController():
	m_pCamera(nullptr)
{
}

CameraController::~CameraController(void)
{
}

void CameraController::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto pFixedCamera = new FixedCamera();
	AddChild(pFixedCamera);

	m_pCamera = pFixedCamera->GetComponent<CameraComponent>();

}

void CameraController::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_pCamera = GetChild<FixedCamera>()->GetComponent<CameraComponent>();
	m_pCamera->SetActive();
	GetTransform()->Translate(0, 210, -80);
	GetTransform()->Rotate(71.5, 0, 0, true);
}

void CameraController::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

}