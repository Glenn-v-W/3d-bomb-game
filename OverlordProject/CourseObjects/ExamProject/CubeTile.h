#pragma once
#include "Scenegraph/GameObject.h"

class ParticleEmitterComponent;
class Bomb;

enum class TileType : int
{
	Destructible = 0,
	Stationary = 1,
	Empty = 2
};

class CubeTile : public GameObject
{
public:
	CubeTile(XMFLOAT3 gridMiddle, float gridSpacing, int xGridMax, int yGridMax, int xPos, int yPos, TileType type);
	virtual ~CubeTile();
	virtual void Initialize(const GameContext& gameContext);
	virtual void PostInitialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);

	bool IsOccupied();
	void SetHasBomb(Bomb* bomb);
	bool Explode();

protected:
	void ExplosionParticleTimer();
	void HandleShrinking();

	XMFLOAT3 m_Position;
	GameObject* m_pTileObject = nullptr;
	TileType m_Type;
	bool m_HasBomb = false;
	Bomb* m_pBomb;

	//GameObject* m_pParticleObject = nullptr;
	//ParticleEmitterComponent* m_pParticleEmitter = nullptr;
	float m_Lifetime = 0.5f;
	float m_TimeAlive = 0;

	bool m_Shrinking = false;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	CubeTile(const CubeTile& t);
	CubeTile& operator=(const CubeTile& t);
};


