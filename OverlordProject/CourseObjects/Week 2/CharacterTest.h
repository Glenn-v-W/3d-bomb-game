#pragma once
#include "Scenegraph/GameScene.h"
#include "..\..\CourseObjects\Week 2\Character.h"

class CharacterTest : public GameScene
{
public:
	CharacterTest(void);
	virtual ~CharacterTest(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	GameObject * m_pController;
	Character * m_pCharacter;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	CharacterTest(const CharacterTest &obj);
	CharacterTest& operator=(const CharacterTest& obj);
};

