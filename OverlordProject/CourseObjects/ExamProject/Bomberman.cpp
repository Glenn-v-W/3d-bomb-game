//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"

#include "Bomberman.h"
#include "CubeTile.h"
#include "Bomb.h"
#include "Scenegraph\GameObject.h"
#include "Components\TransformComponent.h"
#include "Components\ModelComponent.h"
#include "Graphics\ModelAnimator.h"
#include <memory>
#include "Base/SoundManager.h"

#include "..\..\Materials\SkinnedDiffuseMaterial.h"

Bomberman::Bomberman(int id, XMFLOAT3 gridMiddle, float gridSpacing, int xGridMax, int yGridMax, int xPos, int yPos
	, std::shared_ptr<vector<vector<CubeTile*>>> pGrid, std::shared_ptr<vector<Bomberman *>> pPlayers) :
	m_PlayerId(id),
	m_GridSpacing(gridSpacing),
	m_GridMiddle(gridMiddle),
	m_pModel(nullptr),
	m_pPlayers(pPlayers),
	m_pGrid(pGrid),
	m_XGridPos(xPos),
	m_YGridPos(yPos),
	m_XGridMax(xGridMax),
	m_YGridMax(yGridMax)
{
	m_Position = gridMiddle;
	m_Position.x -= xGridMax / 2 * gridSpacing;
	m_Position.z -= yGridMax / 2 * gridSpacing;


	m_Position.x += xPos * gridSpacing;
	m_Position.z += yPos * gridSpacing;
}

Bomberman::~Bomberman(void)
{
}

void Bomberman::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//auto skinnedDiffuseMaterial = new SkinnedDiffuseMaterial();

	GamepadIndex gamepadIndex = GamepadIndex::PlayerOne;

	switch (m_PlayerId)
	{
	case 1:
		/*skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Boy01_diffuse.jpg");*/
		gamepadIndex = GamepadIndex::PlayerOne;
		break;
	case 2:
		//skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Boy02_diffuse.jpg");
		gamepadIndex = GamepadIndex::PlayerTwo;
		break;
	case 3:
		//skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Boy03_diffuse.jpg");
		gamepadIndex = GamepadIndex::PlayerThree;
		break;
	case 4:
		//skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Boy04_diffuse.jpg");
		gamepadIndex = GamepadIndex::PlayerFour;
		break;
	default:
		break;
	}
	//gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial, m_PlayerId);

	m_pModel = new ModelComponent(L"./Resources/Meshes/aj.ovm");
	m_pModel->SetMaterial(m_PlayerId);
	m_pPlayer = new GameObject();
	m_pPlayer->AddComponent(m_pModel);
	AddChild(m_pPlayer);


	m_pPlayer->GetTransform()->Scale(1.0f, 1.0f, 1.0f);
	m_pPlayer->GetTransform()->Translate(m_Position.x, m_Position.y, m_Position.z);


	gameContext.pInput->AddInputAction(InputAction(m_PlayerId * m_MaxNumInputActions + 0, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_UP, gamepadIndex));
	gameContext.pInput->AddInputAction(InputAction(m_PlayerId * m_MaxNumInputActions + 1, Released, -1, -1, XINPUT_GAMEPAD_DPAD_UP, gamepadIndex));
	gameContext.pInput->AddInputAction(InputAction(m_PlayerId * m_MaxNumInputActions + 2, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_RIGHT, gamepadIndex));
	gameContext.pInput->AddInputAction(InputAction(m_PlayerId * m_MaxNumInputActions + 3, Released, -1, -1, XINPUT_GAMEPAD_DPAD_RIGHT, gamepadIndex));
	gameContext.pInput->AddInputAction(InputAction(m_PlayerId * m_MaxNumInputActions + 4, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_DOWN, gamepadIndex));
	gameContext.pInput->AddInputAction(InputAction(m_PlayerId * m_MaxNumInputActions + 5, Released, -1, -1, XINPUT_GAMEPAD_DPAD_DOWN, gamepadIndex));
	gameContext.pInput->AddInputAction(InputAction(m_PlayerId * m_MaxNumInputActions + 6, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_LEFT, gamepadIndex));
	gameContext.pInput->AddInputAction(InputAction(m_PlayerId * m_MaxNumInputActions + 7, Released, -1, -1, XINPUT_GAMEPAD_DPAD_LEFT, gamepadIndex));
	gameContext.pInput->AddInputAction(InputAction(m_PlayerId * m_MaxNumInputActions + 8, Pressed, -1, -1, XINPUT_GAMEPAD_X, gamepadIndex));

	m_pBombs = std::make_shared<std::vector<Bomb*>>();
}

void Bomberman::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pModel->GetAnimator()->SetAnimation(0);
	m_pModel->GetAnimator()->Play();
}

void Bomberman::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (!m_IsDead && !m_InputDisabled)
	{
		auto animator = m_pModel->GetAnimator();
		float deltaSeconds = gameContext.pGameTime->GetElapsed();
		if (m_PlayerState == PlayerState::Idle)
		{
			if (gameContext.pInput->IsActionTriggered(m_PlayerId * m_MaxNumInputActions + 0))
			{
				if (m_FacingDirection != Direction::Up && m_FacingDirection != Direction::Down)
				{
					if (m_TileOffset >= m_GridSpacing / 2)
					{
						cout << m_TileOffset << "\n";
						switch (m_FacingDirection)
						{
						case Direction::Right:
							m_Position.x += m_GridSpacing;
							m_XGridPos++;
							break;
						case Direction::Down:
							m_Position.z -= m_GridSpacing;
							m_YGridPos--;
							break;
						case Direction::Left:
							m_Position.x -= m_GridSpacing;
							m_XGridPos--;
							break;
						default:
							break;
						}
					}
					m_TileOffset = 0;
				}
				else if (m_FacingDirection == Direction::Down)
				{
					m_TileOffset = -m_TileOffset;
				}

				m_pModel->GetTransform()->Rotate(0, 180, 0);
				m_FacingDirection = Direction::Up;
				animator->SetAnimation(1);
				m_pModel->GetAnimator()->Play();
				m_PlayerState = PlayerState::Running;
			}
			if (gameContext.pInput->IsActionTriggered(m_PlayerId * m_MaxNumInputActions + 2))
			{
				if (m_FacingDirection != Direction::Right && m_FacingDirection != Direction::Left)
				{
					if (m_TileOffset >= m_GridSpacing / 2)
					{
						cout << m_TileOffset << "\n";
						switch (m_FacingDirection)
						{
						case Direction::Up:
							m_Position.z += m_GridSpacing;
							m_YGridPos++;
							break;
						case Direction::Down:
							m_Position.z -= m_GridSpacing;
							m_YGridPos--;
							break;
						case Direction::Left:
							m_Position.x -= m_GridSpacing;
							m_XGridPos--;
							break;
						default:
							break;
						}
					}
					m_TileOffset = 0;
				}
				else if (m_FacingDirection == Direction::Left)
				{
					m_TileOffset = -m_TileOffset;
				}

				m_pModel->GetTransform()->Rotate(0, -90, 0);
				m_FacingDirection = Direction::Right;
				animator->SetAnimation(1);
				m_pModel->GetAnimator()->Play();
				m_PlayerState = PlayerState::Running;
			}
			if (gameContext.pInput->IsActionTriggered(m_PlayerId * m_MaxNumInputActions + 4))
			{
				if (m_FacingDirection != Direction::Down && m_FacingDirection != Direction::Up)
				{
					if (m_TileOffset >= m_GridSpacing / 2)
					{
						cout << m_TileOffset << "\n";
						switch (m_FacingDirection)
						{
						case Direction::Up:
							m_Position.z += m_GridSpacing;
							m_YGridPos++;
							break;
						case Direction::Right:
							m_Position.x += m_GridSpacing;
							m_XGridPos++;
							break;
						case Direction::Left:
							m_Position.x -= m_GridSpacing;
							m_XGridPos--;
							break;
						default:
							break;
						}
					}
					m_TileOffset = 0;
				}
				else if (m_FacingDirection == Direction::Up)
				{
					m_TileOffset = -m_TileOffset;
				}

				m_pModel->GetTransform()->Rotate(0, 0, 0);
				m_FacingDirection = Direction::Down;
				animator->SetAnimation(1);
				m_pModel->GetAnimator()->Play();
				m_PlayerState = PlayerState::Running;
			}
			if (gameContext.pInput->IsActionTriggered(m_PlayerId * m_MaxNumInputActions + 6))
			{
				if (m_FacingDirection != Direction::Left && m_FacingDirection != Direction::Right)
				{
					if (m_TileOffset >= m_GridSpacing * 0.75)
					{
						cout << m_TileOffset << "\n";
						switch (m_FacingDirection)
						{
						case Direction::Up:
							m_Position.z += m_GridSpacing;
							m_YGridPos++;
							break;
						case Direction::Right:
							m_Position.x += m_GridSpacing;
							m_XGridPos++;
							break;
						case Direction::Down:
							m_Position.z -= m_GridSpacing;
							m_YGridPos--;
							break;
						default:
							break;
						}
					}
					m_TileOffset = 0;
				}
				else if (m_FacingDirection == Direction::Right)
				{
					m_TileOffset = -m_TileOffset;
				}

				m_pModel->GetTransform()->Rotate(0, 90, 0);
				m_FacingDirection = Direction::Left;
				animator->SetAnimation(1);
				m_pModel->GetAnimator()->Play();
				m_PlayerState = PlayerState::Running;
			}
		}

		if (m_PlayerState == PlayerState::Running)
		{
			if (gameContext.pInput->IsActionTriggered(m_PlayerId * m_MaxNumInputActions + 1)
				||
				gameContext.pInput->IsActionTriggered(m_PlayerId * m_MaxNumInputActions + 3)
				||
				gameContext.pInput->IsActionTriggered(m_PlayerId * m_MaxNumInputActions + 5)
				||
				gameContext.pInput->IsActionTriggered(m_PlayerId * m_MaxNumInputActions + 7))
			{
				animator->SetAnimation(0);
				m_pModel->GetAnimator()->Play();
				m_PlayerState = PlayerState::Idle;
			}
			else
			{
				//Handle Movement
				if (m_FacingDirection == Direction::Up)
				{
					m_TileOffset += m_MovementSpeed * deltaSeconds;
					if (m_TileOffset > m_GridSpacing / 2)
					{
						m_Position.z += m_GridSpacing;
						m_YGridPos++;
						m_TileOffset = -m_TileOffset;
						//Check if next tile has block
					}
					else if (m_TileOffset > (m_GridSpacing / 4))
					{
						CheckNextTileForBlock();
					}
					auto newPosition = m_Position;
					newPosition.z += m_TileOffset;
					m_pModel->GetTransform()->Translate(newPosition);
				}
				else if (m_FacingDirection == Direction::Right)
				{
					m_TileOffset += m_MovementSpeed * deltaSeconds;
					if (m_TileOffset > m_GridSpacing / 2)
					{
						m_Position.x += m_GridSpacing;
						m_XGridPos++;
						m_TileOffset = -m_TileOffset;
						//Check if next tile has block
					}
					else if (m_TileOffset > (m_GridSpacing / 4))
					{
						CheckNextTileForBlock();
					}
					auto newPosition = m_Position;
					newPosition.x += m_TileOffset;
					m_pModel->GetTransform()->Translate(newPosition);
				}
				else if (m_FacingDirection == Direction::Down)
				{
					m_TileOffset += m_MovementSpeed * deltaSeconds;
					if (m_TileOffset > m_GridSpacing / 2)
					{
						m_Position.z -= m_GridSpacing;
						m_YGridPos--;
						m_TileOffset = -m_TileOffset;
						//Check if next tile has block
					}
					else if (m_TileOffset > (m_GridSpacing / 4))
					{
						CheckNextTileForBlock();
					}
					auto newPosition = m_Position;
					newPosition.z -= m_TileOffset;
					m_pModel->GetTransform()->Translate(newPosition);
				}
				else if (m_FacingDirection == Direction::Left)
				{
					m_TileOffset += m_MovementSpeed * deltaSeconds;
					if (m_TileOffset > m_GridSpacing / 2)
					{
						m_Position.x -= m_GridSpacing;
						m_XGridPos--;
						m_TileOffset = -m_TileOffset;
						//Check if next tile has block
					}
					else if (m_TileOffset > (m_GridSpacing / 4))
					{
						CheckNextTileForBlock();
					}
					auto newPosition = m_Position;
					newPosition.x -= m_TileOffset;
					m_pModel->GetTransform()->Translate(newPosition);
				}
			}
		}

		//std::cout << m_TileOffset << "\n";

		if (m_TileOffset == 0 || (m_TileOffset > 0 && m_TileOffset < 0.5f) || (m_TileOffset < 0 && m_TileOffset > -0.5f))
		{
			CheckNextTileForBlock();
		}

		//std::cout << int(m_FacingDirection )<< "\n";


		if (gameContext.pInput->IsActionTriggered(m_PlayerId * m_MaxNumInputActions + 8))
		{
			//Create Bomb
			FMOD::Sound* sound;
			SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 16.wav", FMOD_DEFAULT, 0, &sound);
			FMOD::Channel* channel;
			SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);

			Bomb* bomb = new Bomb(m_GridMiddle, m_GridSpacing, m_XGridMax, m_YGridMax, m_XGridPos, m_YGridPos, m_pGrid, m_pPlayers);
			AddChild(bomb);
			(*m_pBombs).push_back(bomb);

			//Bomberman* player2 = new Bomberman(2, XMFLOAT3(0, 0, 0), m_GridSpacing, m_GridWidth, m_GridHeight, m_GridWidth - 1, 0, m_pGrid, m_pPlayers);
			//AddChild(player2);
			//(*m_pPlayers).push_back(player2);

			//Tell Grid Tile that it's occupied
			(*m_pGrid)[m_XGridPos][m_YGridPos]->SetHasBomb(bomb);
		}
	}
	else if (m_InputDisabled)
	{
		if (!m_IsDead)
		{
			auto animator = m_pModel->GetAnimator();

			animator->SetAnimation(0);
			m_pModel->GetAnimator()->Play();
			m_PlayerState = PlayerState::Idle;
		}
	}
}

void Bomberman::Explode(int x, int y)
{
	if (!m_IsDead)
	{
		if (x == m_XGridPos && y == m_YGridPos)
		{
			RemoveChild(m_pPlayer);
			SafeDelete(m_pPlayer);
			m_IsDead = true;
			//std::cout << "RIP\n";

			FMOD::Sound* sound;
			SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 06.wav", FMOD_DEFAULT, 0, &sound);
			FMOD::Channel* channel;
			SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
		}
	}
}

void Bomberman::DisableInput()
{
	m_InputDisabled = true;
	if (m_pBombs)
	{
		for (size_t i = 0; i < (*m_pBombs).size(); i++)
		{
			(*m_pBombs)[i]->DisableInput();
		}
	}
}

void Bomberman::EnableInput()
{
	m_InputDisabled = false;
	if (m_pBombs)
	{
		for (size_t i = 0; i < (*m_pBombs).size(); i++)
		{
			(*m_pBombs)[i]->EnableInput();
		}
	}
}

bool Bomberman::IsAlive()
{
	return !m_IsDead;
}

int Bomberman::GetId()
{
	return m_PlayerId;
}

void Bomberman::CheckNextTileForBlock()
{
	if (!m_IsDead)
	{
		auto animator = m_pModel->GetAnimator();

		switch (m_FacingDirection)
		{
		case Direction::Up:
			if (m_YGridPos + 1 < m_YGridMax)
			{
				if ((*m_pGrid)[m_XGridPos][m_YGridPos + 1]->IsOccupied())
				{
					animator->SetAnimation(0);
					m_pModel->GetAnimator()->Play();
					m_PlayerState = PlayerState::Idle;
					m_TileOffset = 0;
				}
			}
			else
			{
				animator->SetAnimation(0);
				m_pModel->GetAnimator()->Play();
				m_PlayerState = PlayerState::Idle;
				m_TileOffset = 0;
			}
			break;
		case Direction::Right:
			if (m_XGridPos + 1 < m_XGridMax)
			{
				if ((*m_pGrid)[m_XGridPos + 1][m_YGridPos]->IsOccupied())
				{
					animator->SetAnimation(0);
					m_pModel->GetAnimator()->Play();
					m_PlayerState = PlayerState::Idle;
					m_TileOffset = 0;
				}
			}
			else
			{
				animator->SetAnimation(0);
				m_pModel->GetAnimator()->Play();
				m_PlayerState = PlayerState::Idle;
				m_TileOffset = 0;
			}
			break;
		case Direction::Down:
			if (m_YGridPos - 1 >= 0)
			{
				if ((*m_pGrid)[m_XGridPos][m_YGridPos - 1]->IsOccupied())
				{
					animator->SetAnimation(0);
					m_pModel->GetAnimator()->Play();
					m_PlayerState = PlayerState::Idle;
					m_TileOffset = 0;
				}
			}
			else
			{
				animator->SetAnimation(0);
				m_pModel->GetAnimator()->Play();
				m_PlayerState = PlayerState::Idle;
				m_TileOffset = 0;
			}
			break;
		case Direction::Left:
			if (m_XGridPos - 1 >= 0)
			{
				if ((*m_pGrid)[m_XGridPos - 1][m_YGridPos]->IsOccupied())
				{
					animator->SetAnimation(0);
					m_pModel->GetAnimator()->Play();
					m_PlayerState = PlayerState::Idle;
					m_TileOffset = 0;
				}
			}
			else
			{
				animator->SetAnimation(0);
				m_pModel->GetAnimator()->Play();
				m_PlayerState = PlayerState::Idle;
				m_TileOffset = 0;
			}
			break;
		default:
			break;
		}
	}
}

bool Bomberman::CheckTileForBlock(int x, int y)
{
	if (!m_IsDead)
	{
		if (y < m_YGridMax && x < m_XGridMax && y >= 0 && x >= 0)
		{
			return (*m_pGrid)[x][y]->IsOccupied();
		}
		else
		{
			return true;
		}
	}
	return true;
}
