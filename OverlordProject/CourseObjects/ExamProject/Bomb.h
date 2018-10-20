#pragma once
#include "Scenegraph/GameObject.h"

class Bomberman;
class CubeTile;

class Bomb : public GameObject
{
public:
	Bomb(XMFLOAT3 gridMiddle, float gridSpacing, int xGridMax, int yGridMax, int xPos, int yPos, std::shared_ptr<vector<vector<CubeTile*>>> pGrid, std::shared_ptr<vector<Bomberman *>> pPlayers);
	virtual ~Bomb();
	virtual void Initialize(const GameContext& gameContext);
	virtual void PostInitialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);

	void DisableInput();
	void EnableInput();
	void Explode();

protected:
	bool ExplodeTile(int x, int y);

	XMFLOAT3 m_Position;
	GameObject* m_pBomb = nullptr;
	std::shared_ptr<vector<Bomberman *>> m_pPlayers;
	std::shared_ptr<vector<vector<CubeTile*>>> m_pGrid;
	int m_XGridMax;
	int m_YGridMax;
	int m_XGridPos;
	int m_YGridPos;
	float m_Lifetime = 4.0f;
	float m_TimeAlive = 0;
	bool m_InputDisabled = true;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Bomb(const Bomb& t);
	Bomb& operator=(const Bomb& t);
};


