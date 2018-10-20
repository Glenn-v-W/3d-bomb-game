#pragma once
#include <Scenegraph/GameScene.h>

class Bomberman;
class CameraController;
class CubeTile;
class GameObject;
class SpriteFont;

enum class MainMenu : int
{
	MainMenu = 0,
	Start = 1,
	Exit = 2
};

enum class PlayerSelect : int
{
	PlayerSelect = 0,
	Players2 = 1,
	Players3 = 2,
	Players4 = 3
};

enum class InGame : int
{
	InGame = 0,
	MainMenu = 1,
	Restart = 2,
	Exit = 3
};

enum class Winner : int
{
	Noone = 0,
	Player1 = 1,
	Player2 = 2,
	Player3 = 3,
	Player4
};

class BombermanScene : public GameScene
{
public:
	BombermanScene();
	~BombermanScene();

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void SceneActivated() override;
	void SceneDeactivated() override;

private:
	void CreateLevelLayout();
	void StartGame(int numPlayers);
	void EndGame();

	std::shared_ptr<vector<Bomberman *>> m_pPlayers;

	CameraController* m_pCamera = nullptr;

	GameObject* m_pOuterWall = nullptr;
	GameObject* m_pFloor = nullptr;
	GameObject* m_pBackground = nullptr;
	GameObject* m_pTile = nullptr;

	std::shared_ptr<vector<vector<int>>> m_pLevelLayout;
	std::shared_ptr<vector<vector<CubeTile*>>> m_pGrid;
	int m_GridWidth = 13;
	int m_GridHeight = 11;
	float m_GridSpacing = 15.0f;

	//MENU
	GameObject* m_pMenu;

	bool m_MainMenuActive = true;
	MainMenu m_MainMenu = MainMenu::MainMenu;
	
	bool m_PlayerSelectActive = false;
	PlayerSelect m_PlayerSelect = PlayerSelect::PlayerSelect;
	
	bool m_InGameActive = false;
	InGame m_InGame = InGame::InGame;
	
	int m_NumPlayers;

	bool m_GameOver = false;
	Winner m_Winner = Winner::Noone;

	float m_TimeSinceStart = 0;
	float m_GameTime = 120.0f;

	SpriteFont* m_pFont = nullptr;
	GameObject* m_pTimer;
	GameObject* m_pP1;
	GameObject* m_pP2;
	GameObject* m_pP3;
	GameObject* m_pP4;


private:
	BombermanScene(const BombermanScene &obj) = delete;
	BombermanScene& operator=(const BombermanScene& obj) = delete;
};

