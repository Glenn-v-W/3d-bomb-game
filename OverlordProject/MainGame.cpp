//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "MainGame.h"
#include "Base\GeneralStructs.h"
#include "Scenegraph\SceneManager.h"
#include "Physx\PhysxProxy.h"
#include "Diagnostics\DebugRenderer.h"

//#define W1
//#define W2
//#define W3
//#define W4
//#define W5
//#define W6
//#define W7
//#define W8
#define ExamProject

#ifdef W1
#include "CourseObjects/Week 1/TestScene.h"
#include "CourseObjects/Week 1/ComponentTestScene.h"
#endif

#ifdef W2
#include "CourseObjects/Week 2/CharacterTest.h"
#include "CourseObjects/Week 2/ModelTestScene.h"
#endif

#ifdef W3
#include "CourseObjects/Week 3/SpikeyScene.h"
#include "CourseObjects/Week 3/UberTestScene.h"
#include "CourseObjects/Week 3/SpriteTestScene.h"
#endif

#ifdef W4
#include "CourseObjects/Week 4/PickingScene.h"
#endif

#ifdef W5
#include "CourseObjects/Week 5/SkyBoxTestScene.h"
#endif

#ifdef W6
#include "CourseObjects/Week 6/SoftwareSkinningScene_1.h"
#include "CourseObjects/Week 6/SoftwareSkinningScene_2.h"
#include "CourseObjects/Week 6/SoftwareSkinningScene_3.h"
#endif

#ifdef W7
#include "CourseObjects/Week 7/HardwareSkinningScene.h"
#endif

#ifdef W8
#include "CourseObjects/Week 8/PostProcessingScene.h"
#include "CourseObjects/Week 8/ParticleTestScene.h"
#endif

#ifdef ExamProject
#include "CourseObjects/ExamProject/BombermanScene.h"
#endif

MainGame::MainGame(void)
{
}


MainGame::~MainGame(void)
{
}

//Game is preparing
void MainGame::OnGamePreparing(GameSettings& gameSettings)
{
	UNREFERENCED_PARAMETER(gameSettings);
	//Nothing to do atm.
}

void MainGame::Initialize()
{

#ifdef W1
	SceneManager::GetInstance()->AddGameScene(new TestScene());
	SceneManager::GetInstance()->AddGameScene(new ComponentTestScene());

	//SceneManager::GetInstance()->SetActiveGameScene(L"TestScene");
#endif

#ifdef W2
	SceneManager::GetInstance()->AddGameScene(new ModelTestScene());
	SceneManager::GetInstance()->AddGameScene(new CharacterTest());

	SceneManager::GetInstance()->SetActiveGameScene(L"ModelTestScene");
#endif

#ifdef W3
	SceneManager::GetInstance()->AddGameScene(new SpikeyScene());
	SceneManager::GetInstance()->AddGameScene(new UberTestScene());
	SceneManager::GetInstance()->AddGameScene(new SpriteTestScene());

	SceneManager::GetInstance()->SetActiveGameScene(L"SpriteTestScene");
#endif

#ifdef W4
	SceneManager::GetInstance()->AddGameScene(new PickingScene());

	SceneManager::GetInstance()->SetActiveGameScene(L"PickingScene");
#endif

#ifdef W5
	SceneManager::GetInstance()->AddGameScene(new SkyBoxTestScene());

	SceneManager::GetInstance()->SetActiveGameScene(L"SkyBoxTestScene");
#endif

#ifdef W6
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_1());
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_2());
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_3());
	SceneManager::GetInstance()->SetActiveGameScene(L"SoftwareSkinningScene_3");
#endif

#ifdef W7
	SceneManager::GetInstance()->AddGameScene(new HardwareSkinningScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"HardwareSkinningScene");
#endif

#ifdef W8
	SceneManager::GetInstance()->AddGameScene(new PostProcessingScene()); 
	SceneManager::GetInstance()->AddGameScene(new ParticleTestScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"PostProcessingScene");
#endif

#ifdef ExamProject
	SceneManager::GetInstance()->AddGameScene(new BombermanScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"BombermanScene");
#endif

}

LRESULT MainGame::WindowProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_KEYUP:
		{
			if ((lParam & 0x80000000) != 0x80000000)
				return -1;

			//NextScene
			if (wParam == VK_F3)
			{
				SceneManager::GetInstance()->NextScene();
				return 0;
			}
			//PreviousScene
			else if (wParam == VK_F2)
			{
				SceneManager::GetInstance()->PreviousScene();
				return 0;
			}
			else if (wParam == VK_F4)
			{
				DebugRenderer::ToggleDebugRenderer();
				return 0;
			}
			else if (wParam == VK_F6)
			{
				auto activeScene = SceneManager::GetInstance()->GetActiveScene();
				activeScene->GetPhysxProxy()->NextPhysXFrame();
			}
		}
	}

	return -1;
}
