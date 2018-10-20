//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"

#include "Character.h"
#include "Components\Components.h"
#include "Prefabs\Prefabs.h"
#include "Physx/PhysxManager.h"

Character::Character(float radius, float height, float moveSpeed) :
	m_Radius(radius),
	m_Height(height),
	m_MoveSpeed(moveSpeed),
	m_pCamera(nullptr),
	m_pController(nullptr),
	m_TotalPitch(0),
	m_TotalYaw(0),
	m_RotationSpeed(90.f),
	//Running
	m_MaxRunVelocity(50.0f),
	m_TerminalVelocity(20),
	m_Gravity(9.81f),
	m_RunAccelerationTime(0.3f),
	m_JumpAccelerationTime(0.8f),
	m_RunAcceleration(m_MaxRunVelocity / m_RunAccelerationTime),
	m_JumpAcceleration(m_Gravity / m_JumpAccelerationTime),
	m_RunVelocity(0),
	m_JumpVelocity(0),
	m_Velocity(0, 0, 0)
{
}


Character::~Character(void)
{
}

void Character::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	// Create controller
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	m_pController = new ControllerComponent(bouncyMaterial);
	AddComponent(m_pController);

	// Add a fixed camera as child
	auto pFixedCamera = new FixedCamera();
	AddChild(pFixedCamera);
	//fixedCamera->GetComponent<CameraComponent>()->GetTransform()->Translate(0, 50, 0);
	//fixedCamera->GetComponent<CameraComponent>()->GetTransform()->Rotate(90, 90, 90, true);
	m_pCamera = pFixedCamera->GetComponent<CameraComponent>();
	
	// Register all Input Actions
	//gameContext.pInput->AddInputAction(InputAction(CharacterMovement::LEFT, Down, VK_LEFT));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::LEFT, Down, 'A'));
	//gameContext.pInput->AddInputAction(InputAction(CharacterMovement::RIGHT, Down, VK_RIGHT));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::RIGHT, Down, 'D'));
	//gameContext.pInput->AddInputAction(InputAction(CharacterMovement::FORWARD, Down, VK_UP));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::FORWARD, Down, 'W'));
	//gameContext.pInput->AddInputAction(InputAction(CharacterMovement::BACKWARD, Down, VK_DOWN));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::BACKWARD, Down, 'S'));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::JUMP, Down, VK_SPACE));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::UP, Down, 'Q'));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::DOWN, Down, 'E'));


}

void Character::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	// Set the camera as active
	m_pCamera = GetChild<FixedCamera>()->GetComponent<CameraComponent>();
	m_pCamera->SetActive();
	// We need to do this in the PostInitialize because child game objects only get initialized after the Initialize of the current object finishes
	
}

void Character::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//Update the character (Camera rotation, Character Movement, Character Gravity)

	//CAMERA ROTATION
	if (m_pCamera->IsActive())
	{
		//HANDLE INPUT
		XMFLOAT3 move = XMFLOAT3(0, 0, 0);
		move.y = gameContext.pInput->IsActionTriggered(FORWARD) ? 1.0f : 0.0f;
		if (move.y == 0) move.y = -(gameContext.pInput->IsActionTriggered(BACKWARD) ? 1.0f : 0.0f);
		if (move.y == 0) move.y = gameContext.pInput->GetThumbstickPosition().y;

		move.x = gameContext.pInput->IsActionTriggered(RIGHT) ? 1.0f : 0.0f;
		if (move.x == 0) move.x = -(gameContext.pInput->IsActionTriggered(LEFT) ? 1.0f : 0.0f);
		if (move.x == 0) move.x = gameContext.pInput->GetThumbstickPosition().x;

		move.z = gameContext.pInput->IsActionTriggered(UP) ? 1.0f : 0.0f;
		if (move.z == 0) move.z = -(gameContext.pInput->IsActionTriggered(DOWN) ? 1.0f : 0.0f);

		//auto currSpeed = m_MoveSpeed;

		XMFLOAT2 look = XMFLOAT2(0, 0);
		auto mouseMove = gameContext.pInput->GetMouseMovement();
		look.x = static_cast<float>(mouseMove.x);
		look.y = static_cast<float>(mouseMove.y);

		if (look.x == 0 && look.y == 0)
		{
			look = gameContext.pInput->GetThumbstickPosition(false);
		}

		//CALCULATE TRANSFORMS
		XMVECTOR forward = XMLoadFloat3(&GetTransform()->GetForward());
		XMVECTOR right = XMLoadFloat3(&GetTransform()->GetRight());
		XMVECTOR up = XMLoadFloat3(&GetTransform()->GetUp());
		XMVECTOR currPos = XMLoadFloat3(&GetTransform()->GetPosition());
		//XMVECTOR currRot = XMLoadFloat4(&GetTransform()->GetRotation());
		//XMVECTOR finalQuat;

		PxVec3 dir;

		dir.x = GetTransform()->GetForward().x * move.x;
		dir.y = GetTransform()->GetForward().y * move.y;
		dir.z = 0.f;
		dir.normalize();
		/*currPos += forward * move.y * currSpeed * gameContext.pGameTime->GetElapsed();
		currPos += right * move.x * currSpeed * gameContext.pGameTime->GetElapsed();
		currPos += up * move.z * currSpeed * gameContext.pGameTime->GetElapsed();*/

		m_TotalYaw += look.x * m_RotationSpeed * gameContext.pGameTime->GetElapsed();
		m_TotalPitch += look.y * m_RotationSpeed * gameContext.pGameTime->GetElapsed();

		GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);
		m_pController->GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);

		if ((move.x != 0) || (move.y != 0))
		{
			m_RunVelocity += m_RunAcceleration * gameContext.pGameTime->GetElapsed();
			if (m_RunVelocity > m_MaxRunVelocity)
			{
				m_RunVelocity = m_MaxRunVelocity;
			}
		}
		//float previousVelocityY = m_Velocity.y;
		m_Velocity.x = dir.x * m_RunVelocity;
		m_Velocity.z = dir.y * m_RunVelocity;

		m_Velocity.y = m_JumpVelocity;

		if ((move.x == 0) && (move.y == 0) && (move.z == 0))
		{
			m_Velocity.x = 0.f;
			m_Velocity.z = 0.f;
		}
	}

	XMFLOAT3 end{ m_Velocity.x * gameContext.pGameTime->GetElapsed(), m_Velocity.y * gameContext.pGameTime->GetElapsed(), m_Velocity.z * gameContext.pGameTime->GetElapsed() };
	m_pController->Move(end);
}