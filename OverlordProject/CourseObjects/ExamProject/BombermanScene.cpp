//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"
#include "BombermanScene.h"

#include <Diagnostics/DebugRenderer.h>
#include "Bomberman.h"
#include "CubeTile.h"
#include "CameraController.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Components\SpriteComponent.h"
#include "..\..\Materials\DiffuseMaterial.h"
#include "..\..\Materials\SkinnedDiffuseMaterial.h"
#include <memory>
#include "Base/OverlordGame.h"
#include "Content\ContentManager.h"
#include <Graphics/TextRenderer.h>
#include "..\..\Materials\Post\PostBlur.h"
#include "..\..\Materials\Post\PostGrayscale.h"
#include "..\..\Materials\Post\PostSepia.h"
#include "..\..\Materials\Post\PostVignette.h"
#include "Base/SoundManager.h"


BombermanScene::BombermanScene():
	GameScene(L"BombermanScene")
{
}

BombermanScene::~BombermanScene()
{
}

void BombermanScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	DebugRenderer::ToggleDebugRenderer();

	//Camera
	m_pCamera = new CameraController();
	AddChild(m_pCamera);

	//LEVEL
	//Materials
	auto player1Material = new SkinnedDiffuseMaterial();
	player1Material->SetDiffuseTexture(L"./Resources/Textures/Boy01_diffuse.jpg");
	gameContext.pMaterialManager->AddMaterial(player1Material, 1);

	auto player2Material = new SkinnedDiffuseMaterial();
	player2Material->SetDiffuseTexture(L"./Resources/Textures/Boy02_diffuse.jpg");
	gameContext.pMaterialManager->AddMaterial(player2Material, 2);

	auto player3Material = new SkinnedDiffuseMaterial();
	player3Material->SetDiffuseTexture(L"./Resources/Textures/Boy03_diffuse.jpg");
	gameContext.pMaterialManager->AddMaterial(player3Material, 3);

	auto player4Material = new SkinnedDiffuseMaterial();
	player4Material->SetDiffuseTexture(L"./Resources/Textures/Boy04_diffuse.jpg");
	gameContext.pMaterialManager->AddMaterial(player4Material, 4);

	auto wallMaterial = new DiffuseMaterial();
	wallMaterial->SetDiffuseTexture(L"./Resources/Textures/Bomberman/Wall.png");
	gameContext.pMaterialManager->AddMaterial(wallMaterial, 99);

	auto floorMaterial = new DiffuseMaterial();
	floorMaterial->SetDiffuseTexture(L"./Resources/Textures/Floor.png");
	gameContext.pMaterialManager->AddMaterial(floorMaterial, 100);

	auto backgroundMaterial = new DiffuseMaterial();
	backgroundMaterial->SetDiffuseTexture(L"./Resources/Textures/Background.png");
	gameContext.pMaterialManager->AddMaterial(backgroundMaterial, 101);

	auto destructibleTileMaterial = new DiffuseMaterial();
	destructibleTileMaterial->SetDiffuseTexture(L"./Resources/Textures/Bomberman/Box2.png");
	gameContext.pMaterialManager->AddMaterial(destructibleTileMaterial, 102);

	auto stationaryTileMaterial = new DiffuseMaterial();
	stationaryTileMaterial->SetDiffuseTexture(L"./Resources/Textures/Bomberman/Stationary.png");
	gameContext.pMaterialManager->AddMaterial(stationaryTileMaterial, 103);

	auto bombeMaterial = new DiffuseMaterial();
	bombeMaterial->SetDiffuseTexture(L"./Resources/Textures/tex_ob001_bomb_AL.png");
	gameContext.pMaterialManager->AddMaterial(bombeMaterial, 104);

	auto destroyedBlockMaterial = new DiffuseMaterial();
	destroyedBlockMaterial->SetDiffuseTexture(L"./Resources/Textures/Bomberman/BoxDestroyed.png");
	gameContext.pMaterialManager->AddMaterial(destroyedBlockMaterial, 105);

	//Outer Wall
	m_pOuterWall = new GameObject();
	m_pOuterWall->AddComponent(new ModelComponent(L"./Resources/Meshes/OuterWall.ovm"));
	m_pOuterWall->GetComponent<ModelComponent>()->SetMaterial(99);
	AddChild(m_pOuterWall);
	m_pOuterWall->GetTransform()->Rotate(90, 0, 0);
	m_pOuterWall->GetTransform()->Translate(0, 0, 90);

	//Floor
	m_pFloor = new GameObject();
	m_pFloor->AddComponent(new ModelComponent(L"./Resources/Meshes/BattleUnderground.ovm"));
	m_pFloor->GetComponent<ModelComponent>()->SetMaterial(100);
	AddChild(m_pFloor);
	m_pFloor->GetTransform()->Rotate(90, 0, 0);

	//Background
	m_pBackground = new GameObject();
	m_pBackground->AddComponent(new ModelComponent(L"./Resources/Meshes/Background.ovm"));
	m_pBackground->GetComponent<ModelComponent>()->SetMaterial(101);
	AddChild(m_pBackground);
	m_pBackground->GetTransform()->Rotate(90, 0, 0);
	m_pBackground->GetTransform()->Translate(0, -5, 0);
	
	CreateLevelLayout();

	//MENU
	m_pMenu = new GameObject();
	m_pMenu->AddComponent(new SpriteComponent(L"./Resources/Textures/Menu/MainMenu.png", XMFLOAT2(0, 0), XMFLOAT4(1, 1, 1, 1)));
	AddChild(m_pMenu);
	m_pMenu->GetTransform()->Scale(1.0f, 0.565f, 1.0f);
	m_pMenu->GetTransform()->Translate(0, 0, 0.0f);

	//HUD
	float yOffset = 25.0f;
	float xOffset = 100.0f;
	float xIncrement = 225.0f;
	float hudOpacity = 1.0f;
	m_pTimer = new GameObject();
	m_pTimer->AddComponent(new SpriteComponent(L"./Resources/Textures/Menu/Timer.png", XMFLOAT2(0, 0), XMFLOAT4(1, 1, 1, hudOpacity)));
	AddChild(m_pTimer);
	m_pTimer->GetTransform()->Scale(1.0f, 1.0f, 1.0f);
	m_pTimer->GetTransform()->Translate(xOffset, yOffset, 0.99f);

	m_pP1 = new GameObject();
	m_pP1->AddComponent(new SpriteComponent(L"./Resources/Textures/Menu/P1.png", XMFLOAT2(0, 0), XMFLOAT4(1, 1, 1, hudOpacity)));
	AddChild(m_pP1);
	m_pP1->GetTransform()->Scale(1.0f, 0.565f, 1.0f);
	m_pP1->GetTransform()->Translate(xOffset + 1 * xIncrement, yOffset, 0.99f);

	m_pP2 = new GameObject();
	m_pP2->AddComponent(new SpriteComponent(L"./Resources/Textures/Menu/P2.png", XMFLOAT2(0, 0), XMFLOAT4(1, 1, 1, hudOpacity)));
	AddChild(m_pP2);
	m_pP2->GetTransform()->Scale(1.0f, 0.565f, 1.0f);
	m_pP2->GetTransform()->Translate(xOffset + 2 * xIncrement, yOffset, 0.99f);

	m_pP3 = new GameObject();
	m_pP3->AddComponent(new SpriteComponent(L"./Resources/Textures/Menu/P3.png", XMFLOAT2(0, 0), XMFLOAT4(1, 1, 1, hudOpacity)));
	AddChild(m_pP3);
	m_pP3->GetTransform()->Scale(1.0f, 0.565f, 1.0f);
	m_pP3->GetTransform()->Translate(xOffset + 3 * xIncrement, yOffset, 0.99f);

	m_pP4 = new GameObject();
	m_pP4->AddComponent(new SpriteComponent(L"./Resources/Textures/Menu/P4.png", XMFLOAT2(0, 0), XMFLOAT4(1, 1, 1, hudOpacity)));
	AddChild(m_pP4);
	m_pP4->GetTransform()->Scale(1.0f, 0.565f, 1.0f);
	m_pP4->GetTransform()->Translate(xOffset + 4 * xIncrement, yOffset, 0.99f);


	//SpriteFont
	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Heroes Legend.fnt");

	//INPUT
	gameContext.pInput->AddInputAction(InputAction(1000, Pressed, -1, -1, XINPUT_GAMEPAD_START, GamepadIndex::PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(1001, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex::PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(1002, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex::PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(1003, Pressed, -1, -1, XINPUT_GAMEPAD_A, GamepadIndex::PlayerOne));

	//PP
	//gameContext.pMaterialManager->AddMaterial_PP(new PostSepia(), 0);
	//gameContext.pMaterialManager->AddMaterial_PP(new PostVignette(), 1);

	//AddPostProcessingMaterial(0);
	//AddPostProcessingMaterial(1);
}

void BombermanScene::Update(const GameContext& gameContext)
{	

	if (m_pPlayers)
	{
		int numPlayersAlive = 0;
		Winner winner = Winner::Noone;
		for (size_t i = 0; i < (*m_pPlayers).size(); i++)
		{
			if (m_MainMenuActive || m_PlayerSelectActive || m_InGameActive || m_GameOver)
			{
				(*m_pPlayers)[i]->DisableInput();
			}
			else
			{
				(*m_pPlayers)[i]->EnableInput();
				if ((*m_pPlayers)[i]->IsAlive())
				{
					numPlayersAlive++;
					winner = Winner((*m_pPlayers)[i]->GetId());
				}
			}
		}
		if (!m_GameOver && !m_MainMenuActive && !m_PlayerSelectActive && !m_InGameActive)
		{
			if (numPlayersAlive <= 1)
			{
				m_GameOver = true;
				m_Winner = winner;
			}
		}
	}

	if (!m_GameOver && !m_MainMenuActive && !m_PlayerSelectActive && !m_InGameActive)
	{
		m_TimeSinceStart += gameContext.pGameTime->GetElapsed();
		if (m_TimeSinceStart > m_GameTime)
		{
			m_GameOver = true;
			m_Winner = Winner::Noone;
		}
	}

	if (gameContext.pInput->IsActionTriggered(1000))
	{
		//Open in-game menu
		if (!m_MainMenuActive && !m_PlayerSelectActive && !m_InGameActive)
		{
			m_InGameActive = true;
			m_InGame = InGame::InGame;

			FMOD::Sound* sound;
			SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 01.wav", FMOD_DEFAULT, 0, &sound);
			FMOD::Channel* channel;
			SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
		}
		else if(m_InGameActive)
		{
			m_InGameActive = false;
			m_InGame = InGame::InGame;

			FMOD::Sound* sound;
			SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 01.wav", FMOD_DEFAULT, 0, &sound);
			FMOD::Channel* channel;
			SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
		}
	}

	if (gameContext.pInput->IsActionTriggered(1001))
	{
		//UP
		if (m_MainMenuActive)
		{
			switch (m_MainMenu)
			{
			case MainMenu::MainMenu:
				m_MainMenu = MainMenu::Start;
				break;
			case MainMenu::Start:
				m_MainMenu = MainMenu::Exit;
				break;
			case MainMenu::Exit:
				m_MainMenu = MainMenu::Start;
				break;
			default:
				break;
			}

			FMOD::Sound* sound;
			SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 01.wav", FMOD_DEFAULT, 0, &sound);
			FMOD::Channel* channel;
			SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
		}
		else if (m_PlayerSelectActive)
		{
			switch (m_PlayerSelect)
			{
			case PlayerSelect::PlayerSelect:
				m_PlayerSelect = PlayerSelect::Players2;
				break;
			case PlayerSelect::Players2:
				m_PlayerSelect = PlayerSelect::Players4;
				break;
			case PlayerSelect::Players3:
				m_PlayerSelect = PlayerSelect::Players2;
				break;
			case PlayerSelect::Players4:
				m_PlayerSelect = PlayerSelect::Players3;
				break;
			default:
				break;
			}

			FMOD::Sound* sound;
			SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 01.wav", FMOD_DEFAULT, 0, &sound);
			FMOD::Channel* channel;
			SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
		}
		else if (m_InGameActive)
		{
			switch (m_InGame)
			{
			case InGame::InGame:
				m_InGame = InGame::MainMenu;
				break;
			case InGame::MainMenu:
				m_InGame = InGame::Exit;
				break;
			case InGame::Restart:
				m_InGame = InGame::MainMenu;
				break;
			case InGame::Exit:
				m_InGame = InGame::Restart;
				break;
			default:
				break;
			}

			FMOD::Sound* sound;
			SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 01.wav", FMOD_DEFAULT, 0, &sound);
			FMOD::Channel* channel;
			SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
		}
	}

	if (gameContext.pInput->IsActionTriggered(1002))
	{
		//DOWN
		if (m_MainMenuActive)
		{
			switch (m_MainMenu)
			{
			case MainMenu::MainMenu:
				m_MainMenu = MainMenu::Start;
				break;
			case MainMenu::Start:
				m_MainMenu = MainMenu::Exit;
				break;
			case MainMenu::Exit:
				m_MainMenu = MainMenu::Start;
				break;
			default:
				break;
			}

			FMOD::Sound* sound;
			SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 01.wav", FMOD_DEFAULT, 0, &sound);
			FMOD::Channel* channel;
			SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
		}
		else if (m_PlayerSelectActive)
		{
			switch (m_PlayerSelect)
			{
			case PlayerSelect::PlayerSelect:
				m_PlayerSelect = PlayerSelect::Players2;
				break;
			case PlayerSelect::Players2:
				m_PlayerSelect = PlayerSelect::Players3;
				break;
			case PlayerSelect::Players3:
				m_PlayerSelect = PlayerSelect::Players4;
				break;
			case PlayerSelect::Players4:
				m_PlayerSelect = PlayerSelect::Players2;
				break;
			default:
				break;
			}

			FMOD::Sound* sound;
			SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 01.wav", FMOD_DEFAULT, 0, &sound);
			FMOD::Channel* channel;
			SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
		}
		else if (m_InGameActive)
		{
			switch (m_InGame)
			{
			case InGame::InGame:
				m_InGame = InGame::MainMenu;
				break;
			case InGame::MainMenu:
				m_InGame = InGame::Restart;
				break;
			case InGame::Restart:
				m_InGame = InGame::Exit;
				break;
			case InGame::Exit:
				m_InGame = InGame::MainMenu;
				break;
			default:
				break;
			}

			FMOD::Sound* sound;
			SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 01.wav", FMOD_DEFAULT, 0, &sound);
			FMOD::Channel* channel;
			SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
		}
	}

	if (gameContext.pInput->IsActionTriggered(1003))
	{
		//SELECT
		if (m_MainMenuActive)
		{
			switch (m_MainMenu)
			{
			case MainMenu::MainMenu:
				m_MainMenu = MainMenu::Start;
				break;
			case MainMenu::Start:
				m_MainMenuActive = false;
				m_MainMenu = MainMenu::MainMenu;
				m_PlayerSelectActive = true;
				m_PlayerSelect = PlayerSelect::PlayerSelect;
				break;
			case MainMenu::Exit:
				OverlordGame::EndGame();
				break;
			default:
				break;
			}

			FMOD::Sound* sound;
			SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 01.wav", FMOD_DEFAULT, 0, &sound);
			FMOD::Channel* channel;
			SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
		}
		else if (m_PlayerSelectActive)
		{
			FMOD::Sound* sound;
			FMOD::Channel* channel;
			switch (m_PlayerSelect)
			{
			case PlayerSelect::PlayerSelect:
				m_PlayerSelect = PlayerSelect::Players2;

				SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 01.wav", FMOD_DEFAULT, 0, &sound);
				SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
				break;
			case PlayerSelect::Players2:
				m_PlayerSelectActive = false;
				m_PlayerSelect = PlayerSelect::PlayerSelect;
				m_NumPlayers = 2;
				StartGame(m_NumPlayers);

				SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 06.wav", FMOD_DEFAULT, 0, &sound);
				SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
				break;
			case PlayerSelect::Players3:
				m_PlayerSelectActive = false;
				m_PlayerSelect = PlayerSelect::PlayerSelect;
				m_NumPlayers = 3;
				StartGame(m_NumPlayers);

				SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 06.wav", FMOD_DEFAULT, 0, &sound);
				SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
				break;
			case PlayerSelect::Players4:
				m_PlayerSelectActive = false;
				m_PlayerSelect = PlayerSelect::PlayerSelect;
				m_NumPlayers = 4;
				StartGame(m_NumPlayers);

				SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 06.wav", FMOD_DEFAULT, 0, &sound);
				SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
				break;
			default:
				break;
			}
		}
		else if (m_InGameActive)
		{
			switch (m_InGame)
			{
			case InGame::InGame:
				m_InGame = InGame::MainMenu;
				break;
			case InGame::MainMenu:
				EndGame();
				m_InGameActive = false;
				m_InGame = InGame::InGame;
				m_MainMenuActive = true;
				m_MainMenu = MainMenu::MainMenu;
				break;
			case InGame::Restart:
				m_InGameActive = false;
				m_InGame = InGame::InGame;
				EndGame();
				StartGame(m_NumPlayers);
				break;
			case InGame::Exit:
				EndGame();
				OverlordGame::EndGame();
				break;
			default:
				break;
			}

			FMOD::Sound* sound;
			SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 01.wav", FMOD_DEFAULT, 0, &sound);
			FMOD::Channel* channel;
			SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
		}
		if (m_GameOver)
		{
			EndGame();
			m_GameOver = false;
			m_Winner = Winner::Noone;
			m_MainMenuActive = true;
			m_MainMenu = MainMenu::MainMenu;

			FMOD::Sound* sound;
			SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 01.wav", FMOD_DEFAULT, 0, &sound);
			FMOD::Channel* channel;
			SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);
		}
	}
}

void BombermanScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pP1->GetTransform()->Scale(0, 0, 0);
	m_pP2->GetTransform()->Scale(0, 0, 0);
	m_pP3->GetTransform()->Scale(0, 0, 0);
	m_pP4->GetTransform()->Scale(0, 0, 0);
	m_pTimer->GetTransform()->Scale(0, 0, 0);
	m_pMenu->GetTransform()->Translate(0, 0, 0.0f);

	if (!m_GameOver && m_pPlayers && !m_MainMenuActive && !m_PlayerSelectActive && !m_InGameActive)
	{
		wstringstream sText;
		sText << (int(m_GameTime - m_TimeSinceStart) / 60);
		sText << ":";
		sText << (int(m_GameTime - m_TimeSinceStart) % 60);
		TextRenderer::GetInstance()->DrawText(m_pFont, sText.str(), XMFLOAT2(175, 15.0f), XMFLOAT4(Colors::Blue));
		m_pTimer->GetTransform()->Scale(1.0f, 1.0f, 1.0f);
	}

	if (m_pPlayers)
	{
		for (size_t i = 0; i < (*m_pPlayers).size(); i++)
		{
			if (!m_MainMenuActive && !m_PlayerSelectActive && !m_InGameActive && !m_GameOver)
			{
				int id = (*m_pPlayers)[i]->GetId();

				float yOffset = 15.0f;
				float xOffset = 235;
				float xIncrement = 225.0f;

				wstringstream sText;
				if ((*m_pPlayers)[i]->IsAlive())
				{
					sText << 1;
				}
				else
				{
					sText << 0;
				}
				float x = xOffset + xIncrement * id;
				auto color = Colors::White;
				switch (id)
				{
				case 1:
					m_pP1->GetTransform()->Scale(1.0f, 0.565f, 1.0f);
					color = Colors::Red;
					TextRenderer::GetInstance()->DrawText(m_pFont, sText.str(), XMFLOAT2(x, yOffset), XMFLOAT4(color));
					break;
				case 2:
					m_pP2->GetTransform()->Scale(1.0f, 0.565f, 1.0f);
					TextRenderer::GetInstance()->DrawText(m_pFont, sText.str(), XMFLOAT2(x, yOffset), XMFLOAT4(44.0f / 256.0f, 206.0f / 256.0f, 22.0f / 256.0f, 1.0f));
					break;
				case 3:
					m_pP3->GetTransform()->Scale(1.0f, 0.565f, 1.0f);
					color = Colors::Blue;
					TextRenderer::GetInstance()->DrawText(m_pFont, sText.str(), XMFLOAT2(x, yOffset), XMFLOAT4(color));
					break;
				case 4:
					m_pP4->GetTransform()->Scale(1.0f, 0.565f, 1.0f);
					TextRenderer::GetInstance()->DrawText(m_pFont, sText.str(), XMFLOAT2(x, yOffset), XMFLOAT4(226.0f/256.0f, 216.0f/256.0f, 0.0f, 1.0f));
					break;
				default:
					break;
				}
			}
		}
	}


	m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 0));
	m_pMenu->GetComponent<SpriteComponent>()->GetTransform()->Scale(1.0f, 0.565f, 0.0f);

	if (m_MainMenuActive)
	{
		switch (m_MainMenu)
		{
		case MainMenu::MainMenu:
			m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/MainMenu.png");
			m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
			break;
		case MainMenu::Start:
			m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/Start.png");
			m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
			break;
		case MainMenu::Exit:
			m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/Exit.png");
			m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
			break;
		default:
			break;
		}
	}
	else if (m_PlayerSelectActive)
	{
		switch (m_PlayerSelect)
		{
		case PlayerSelect::PlayerSelect:
			m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/PlayerSelect.png");
			m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
			break;
		case PlayerSelect::Players2:
			m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/2.png");
			m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
			break;
		case PlayerSelect::Players3:
			m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/3.png");
			m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
			break;
		case PlayerSelect::Players4:
			m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/4.png");
			m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
			break;
		default:
			break;
		}
	}
	else if (m_InGameActive)
	{
		m_pMenu->GetTransform()->Translate(0, 0, .95f);
		switch (m_InGame)
		{
		case InGame::InGame:
			m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/InGame.png");
			m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
			break;
		case InGame::MainMenu:
			m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/InGameMainMenu.png");
			m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
			break;
		case InGame::Restart:
			m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/InGameRestart.png");
			m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
			break;
		case InGame::Exit:
			m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/InGameExit.png");
			m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
			break;
		default:
			break;
		}
	}
	else
	{
		if (m_GameOver)
		{
			m_pMenu->GetTransform()->Translate(0, 0, .95f);
			switch (m_Winner)
			{
			case Winner::Noone:
				m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/GameOver.png");
				m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
				break;
			case Winner::Player1:
				m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/Orange.png");
				m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
				break;
			case Winner::Player2:
				m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/Green.png");
				m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
				break;
			case Winner::Player3:
				m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/Orange.png");
				m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
				break;
			case Winner::Player4:
				m_pMenu->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/Menu/Yellow.png");
				m_pMenu->GetComponent<SpriteComponent>()->SetColor(XMFLOAT4(1, 1, 1, 1));
				break;
			default:
				break;
			}
		}
		else
		{
			m_pMenu->GetComponent<SpriteComponent>()->GetTransform()->Scale(0.0f, 0.0, 0.0f);
		}
	}

}

void BombermanScene::SceneActivated()
{
}

void BombermanScene::SceneDeactivated()
{
}

void BombermanScene::CreateLevelLayout()
{
	m_pLevelLayout = std::make_shared<std::vector<std::vector<int>>>();

	for (int collumn = 0; collumn < m_GridWidth; collumn++)
	{
		(*m_pLevelLayout).push_back(vector<int>());
		for (int row = 0; row < m_GridHeight; row++)
		{
			if (row % 2 == 1 && collumn % 2 == 1)
			{
				(*m_pLevelLayout)[collumn].push_back(1);
			}
			else
			{
				(*m_pLevelLayout)[collumn].push_back(2);
			}
		}
	}

	//Row 0
	(*m_pLevelLayout)[3][0] = 0;
	(*m_pLevelLayout)[4][0] = 0;
	(*m_pLevelLayout)[5][0] = 0;
	(*m_pLevelLayout)[6][0] = 0;
	(*m_pLevelLayout)[7][0] = 0;
	(*m_pLevelLayout)[8][0] = 0;
	(*m_pLevelLayout)[9][0] = 0;
	//Row 1
	(*m_pLevelLayout)[6][1] = 0;
	(*m_pLevelLayout)[8][1] = 0;
	//Row 2	
	(*m_pLevelLayout)[3][2] = 0;
	(*m_pLevelLayout)[4][2] = 0;
	(*m_pLevelLayout)[5][2] = 0;
	(*m_pLevelLayout)[8][2] = 0;
	(*m_pLevelLayout)[9][2] = 0;
	//Row 3
	(*m_pLevelLayout)[0][3] = 0;
	(*m_pLevelLayout)[2][3] = 0;
	(*m_pLevelLayout)[4][3] = 0;
	(*m_pLevelLayout)[8][3] = 0;
	(*m_pLevelLayout)[12][3] = 0;
	//Row 4
	(*m_pLevelLayout)[1][4] = 0;
	(*m_pLevelLayout)[4][4] = 0;
	(*m_pLevelLayout)[5][4] = 0;
	(*m_pLevelLayout)[8][4] = 0;
	(*m_pLevelLayout)[9][4] = 0;
	(*m_pLevelLayout)[10][4] = 0;
	(*m_pLevelLayout)[11][4] = 0;
	//Row 5
	(*m_pLevelLayout)[0][5] = 0;
	(*m_pLevelLayout)[2][5] = 0;
	(*m_pLevelLayout)[4][5] = 0;
	(*m_pLevelLayout)[8][5] = 0;
	(*m_pLevelLayout)[12][5] = 0;
	//Row 6
	(*m_pLevelLayout)[1][6] = 0;
	(*m_pLevelLayout)[2][6] = 0;
	(*m_pLevelLayout)[7][6] = 0;
	(*m_pLevelLayout)[8][6] = 0;
	(*m_pLevelLayout)[9][6] = 0;
	(*m_pLevelLayout)[10][6] = 0;
	(*m_pLevelLayout)[11][6] = 0;
	(*m_pLevelLayout)[12][6] = 0;
	//Row 7
	(*m_pLevelLayout)[10][7] = 0;
	(*m_pLevelLayout)[12][7] = 0;
	//Row 8
	(*m_pLevelLayout)[4][8] = 0;
	(*m_pLevelLayout)[5][8] = 0;
	(*m_pLevelLayout)[6][8] = 0;
	(*m_pLevelLayout)[7][8] = 0;
	//Row 9
	(*m_pLevelLayout)[4][9] = 0;
	(*m_pLevelLayout)[8][9] = 0;
	//Row 10
	(*m_pLevelLayout)[5][10] = 0;
	(*m_pLevelLayout)[7][10] = 0;
	(*m_pLevelLayout)[8][10] = 0;
}

void BombermanScene::StartGame(int numPlayers)
{
	//Level
	m_pGrid = std::make_shared<std::vector<std::vector<CubeTile*>>>();
	for (int collumn = 0; collumn < m_GridWidth; collumn++)
	{
		(*m_pGrid).push_back(vector<CubeTile*>());
		for (int row = 0; row < m_GridHeight; row++)
		{
			int tileType = (*m_pLevelLayout)[collumn][row];
			CubeTile* gridTile = new CubeTile(XMFLOAT3(0, 0, 0), m_GridSpacing, m_GridWidth, m_GridHeight, collumn, row, TileType(tileType));
			(*m_pGrid)[collumn].push_back(gridTile);
			AddChild(gridTile);
		}
	}

	m_pPlayers = std::make_shared<std::vector<Bomberman*>>();
	//Players
	Bomberman* player1 = new Bomberman(1, XMFLOAT3(0, 0, 0), m_GridSpacing, m_GridWidth, m_GridHeight, 0, m_GridHeight - 1, m_pGrid, m_pPlayers);
	AddChild(player1);
	(*m_pPlayers).push_back(player1);
	if (numPlayers > 1)
	{
		Bomberman* player2 = new Bomberman(2, XMFLOAT3(0, 0, 0), m_GridSpacing, m_GridWidth, m_GridHeight, m_GridWidth - 1, 0, m_pGrid, m_pPlayers);
		AddChild(player2);
		(*m_pPlayers).push_back(player2);
	}
	if (numPlayers > 2)
	{
		Bomberman* player3 = new Bomberman(3, XMFLOAT3(0, 0, 0), m_GridSpacing, m_GridWidth, m_GridHeight, 0, 0, m_pGrid, m_pPlayers);
		AddChild(player3);
		(*m_pPlayers).push_back(player3);
	}
	if (numPlayers > 3)
	{
		Bomberman* player4 = new Bomberman(4, XMFLOAT3(0, 0, 0), m_GridSpacing, m_GridWidth, m_GridHeight, m_GridWidth - 1, m_GridHeight - 1, m_pGrid, m_pPlayers);
		AddChild(player4);
		(*m_pPlayers).push_back(player4);
	}

	m_TimeSinceStart = 0;
	m_GameOver = false;
	m_Winner = Winner::Noone;
}

void BombermanScene::EndGame()
{
	while (!(*m_pPlayers).empty())
	{
		auto it = find(m_pPlayers->begin(), m_pPlayers->end(), (*m_pPlayers)[0]);
		RemoveChild(*it);
		m_pPlayers->erase(it);
	}

	for (int collumn = 0; collumn < m_GridWidth; collumn++)
	{
		for (int row = 0; row < m_GridHeight; row++)
		{
			RemoveChild((*m_pGrid)[collumn][row]);
		}
	}
}

