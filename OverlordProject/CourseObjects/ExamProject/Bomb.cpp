//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"

#include "Bomberman.h"
#include "Bomb.h"
#include "CubeTile.h"
#include "Components/Components.h"
#include "..\..\Materials\DiffuseMaterial.h"
#include "Components\ParticleEmitterComponent.h"
#include "Base/SoundManager.h"

Bomb::Bomb(XMFLOAT3 gridMiddle, float gridSpacing, int xGridMax, int yGridMax, int xPos, int yPos, std::shared_ptr<vector<vector<CubeTile*>>> pGrid, std::shared_ptr<vector<Bomberman *>> pPlayers):
	m_pPlayers(pPlayers),
	m_pGrid(pGrid),
	m_XGridPos(xPos),
	m_YGridPos(yPos),
	m_XGridMax(xGridMax),
	m_YGridMax(yGridMax)
{
	m_Position = gridMiddle;
	m_Position.x -= xGridMax / 2 * gridSpacing;
	m_Position.z -= yGridMax / 2 * gridSpacing;


	m_Position.x += xPos * gridSpacing;
	m_Position.z += yPos * gridSpacing;
}

Bomb::~Bomb(void)
{
}

void Bomb::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_pBomb = new GameObject();
	m_pBomb->AddComponent(new ModelComponent(L"./Resources/Meshes/Bomb.ovm")); //Change Model!

	m_pBomb->GetComponent<ModelComponent>()->SetMaterial(104); //Change Mat!

	AddChild(m_pBomb);
	float scale = 10;
	m_pBomb->GetTransform()->Scale(scale, scale, scale);
	m_pBomb->GetTransform()->Translate(m_Position.x, m_Position.y, m_Position.z);

	GameObject* pParticleObject = new GameObject();
	ParticleEmitterComponent* pParticleEmitter = new ParticleEmitterComponent(L"./Resources/Textures/Wick.png", 60);
	pParticleEmitter->SetVelocity(XMFLOAT3(0, 0.0f, 0));
	pParticleEmitter->SetMinSize(1.0f);
	pParticleEmitter->SetMaxSize(1.25f);
	pParticleEmitter->SetMinEnergy(.25f);
	pParticleEmitter->SetMaxEnergy(.5f);
	pParticleEmitter->SetMinSizeGrow(3.5f);
	pParticleEmitter->SetMaxSizeGrow(6.5f);
	pParticleEmitter->SetMinEmitterRange(0.2f);
	pParticleEmitter->SetMaxEmitterRange(0.5f);
	pParticleEmitter->SetColor(XMFLOAT4(1.f, 0.96f, 0.f, 0.6f));
	pParticleObject->AddComponent(pParticleEmitter);
	AddChild(pParticleObject);

	auto pos = m_Position;
	pos.y += 15;
	pos.z -= 2.5f;
	pParticleEmitter->GetTransform()->Translate(pos);

	m_TimeAlive = 0;
}

void Bomb::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

}

void Bomb::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (!m_InputDisabled)
	{
		m_TimeAlive += gameContext.pGameTime->GetElapsed();
		if (m_TimeAlive > m_Lifetime)
		{
			Explode();
		}

		m_TimeAlive += gameContext.pGameTime->GetElapsed();
		if (m_TimeAlive > m_Lifetime)
		{
			auto children = GetChildren<GameObject>();
			for (size_t i = 0; i < children.size(); i++)
			{
				auto emitter = (children[i])->GetComponent<ParticleEmitterComponent>();
				if (emitter)
				{
					RemoveChild((children[i]));
					SafeDelete((children[i]));
				}
			}
		}
	}
}

void Bomb::DisableInput()
{
	m_InputDisabled = true;
}

void Bomb::EnableInput()
{
	m_InputDisabled = false;
}

void Bomb::Explode()
{
	m_TimeAlive = m_Lifetime * 2;
	if (m_pBomb)
	{
		FMOD::Sound* sound;
		SoundManager::GetInstance()->GetSystem()->createSound("./Resources/SoundEffects/WAVE_SE_BATTLE - 08.wav", FMOD_DEFAULT, 0, &sound);
		FMOD::Channel* channel;
		SoundManager::GetInstance()->GetSystem()->playSound(sound, NULL, false, &channel);

		RemoveChild(m_pBomb);
		SafeDelete(m_pBomb);

		ExplodeTile(m_XGridPos, m_YGridPos);
		if (!ExplodeTile(m_XGridPos, m_YGridPos + 1))
		{
			ExplodeTile(m_XGridPos, m_YGridPos + 2);
		}
		if (!ExplodeTile(m_XGridPos, m_YGridPos - 1))
		{
			ExplodeTile(m_XGridPos, m_YGridPos - 2);
		}
		if (!ExplodeTile(m_XGridPos + 1, m_YGridPos))
		{
			ExplodeTile(m_XGridPos + 2, m_YGridPos);
		}
		if (!ExplodeTile(m_XGridPos - 1, m_YGridPos))
		{
			ExplodeTile(m_XGridPos - 2, m_YGridPos);
		}
	}
}

bool Bomb::ExplodeTile(int x, int y)
{
	for (size_t id = 0; id < (*m_pPlayers).size(); id++)
	{
		(*m_pPlayers)[id]->Explode(x, y);
	}
	if (x < m_XGridMax && x >= 0 && y < m_YGridMax && y >= 0)
	{
		return (*m_pGrid)[x][y]->Explode();
	}
	else
	{
		return false;
	}
}
