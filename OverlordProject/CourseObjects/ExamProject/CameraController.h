#pragma once
#include "Scenegraph/GameObject.h"

class CameraComponent;

class CameraController : public GameObject
{
public:
	CameraController();
	virtual ~CameraController();
	virtual void Initialize(const GameContext& gameContext);
	virtual void PostInitialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);


protected:
	CameraComponent * m_pCamera;


private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	CameraController(const CameraController& t);
	CameraController& operator=(const CameraController& t);
};


