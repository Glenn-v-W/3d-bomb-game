#pragma once
#include "Scenegraph/GameObject.h"

class ModelComponent;

class CubeTile;
class Bomb;

enum class PlayerState : int
{
	Idle = 0,
	Running = 1
};

enum class Direction : int
{
	Up = 0,
	Right = 1,
	Down = 2,
	Left = 3
};

class Bomberman : public GameObject
{
public:
	Bomberman(int id, XMFLOAT3 gridMiddle, float gridSpacing, int xGridMax, int yGridMax, int xPos, int yPos 
		, std::shared_ptr<vector<vector<CubeTile*>>> pGrid, std::shared_ptr<vector<Bomberman *>> pPlayers);
	virtual ~Bomberman();
	virtual void Initialize(const GameContext& gameContext);
	virtual void PostInitialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);

	void Explode(int x, int y);
	void DisableInput();
	void EnableInput();
	bool IsAlive();
	int GetId();

protected:
	void CheckNextTileForBlock();
	bool CheckTileForBlock(int x, int y);

	int m_PlayerId;
	int m_MaxNumInputActions = 20;
	GameObject * m_pPlayer = nullptr;
	ModelComponent * m_pModel = nullptr;
	XMFLOAT3 m_Position;
	std::shared_ptr<vector<Bomberman*>> m_pPlayers;
	std::shared_ptr<vector<Bomb*>> m_pBombs;
	std::shared_ptr<vector<vector<CubeTile*>>> m_pGrid;
	int m_XGridMax;
	int m_YGridMax;
	int m_XGridPos;
	int m_YGridPos;
	XMFLOAT3 m_GridMiddle;
	float m_GridSpacing;
	PlayerState m_PlayerState = PlayerState::Idle;
	Direction m_FacingDirection = Direction::Down;
	float m_MovementSpeed = 30.0f;
	float m_TileOffset = 0;

	bool m_IsDead = false;

	bool m_InputDisabled = true;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Bomberman(const Bomberman& t);
	Bomberman& operator=(const Bomberman& t);
};


