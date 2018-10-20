#pragma once
#include "Scenegraph/GameScene.h"
#include "Components/TerrainComponent.h"

class SkyBoxTestScene : public GameScene
{
public:
	SkyBoxTestScene(void);
	virtual ~SkyBoxTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	GameObject* m_pSkyBox;
	GameObject* m_pTerrain;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkyBoxTestScene(const SkyBoxTestScene &obj);
	SkyBoxTestScene& operator=(const SkyBoxTestScene& obj);
};

