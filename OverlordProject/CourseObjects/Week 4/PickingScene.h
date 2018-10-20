#pragma once
#include "Scenegraph/GameScene.h"
#include "Helpers\EffectHelper.h"
#include "Components\CameraComponent.h"

class Material;

class PickingScene : public GameScene
{
public:
	PickingScene(void);
	virtual ~PickingScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	float m_FpsInterval;
	GameObject * m_pBox;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PickingScene(const PickingScene &obj);
	PickingScene& operator=(const PickingScene& obj);
};

