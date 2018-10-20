//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "./base/stdafx.h"

#include "CubeTile.h"
#include "Bomb.h"
#include "Components/Components.h"
#include "..\..\Materials\DiffuseMaterial.h"
#include "Components\ParticleEmitterComponent.h"

CubeTile::CubeTile(XMFLOAT3 gridMiddle, float gridSpacing, int xGridMax, int yGridMax, int xPos, int yPos, TileType type):
	m_Type(type)
{
	m_Position = gridMiddle;
	m_Position.x -= xGridMax / 2 * gridSpacing;
	m_Position.z -= yGridMax / 2 * gridSpacing;


	m_Position.x += xPos * gridSpacing;
	m_Position.z += yPos * gridSpacing;
}

CubeTile::~CubeTile(void)
{
}

void CubeTile::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (m_Type != TileType::Empty)
	{
		m_pTileObject = new GameObject();
		m_pTileObject->AddComponent(new ModelComponent(L"./Resources/Meshes/Tile.ovm"));

		switch (m_Type)
		{
		case TileType::Destructible:
			m_pTileObject->GetComponent<ModelComponent>()->SetMaterial(102);
			break;
		case TileType::Stationary:
			m_pTileObject->GetComponent<ModelComponent>()->SetMaterial(103);
			break;
		default:
			break;
		}

		AddChild(m_pTileObject);
		m_pTileObject->GetTransform()->Rotate(90, 0, 0);
		m_pTileObject->GetTransform()->Translate(m_Position.x, m_Position.y, m_Position.z);
	}
}

void CubeTile::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

}

void CubeTile::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_TimeAlive += gameContext.pGameTime->GetElapsed();
	ExplosionParticleTimer();
	HandleShrinking();
}

bool CubeTile::IsOccupied()
{
	return (m_Type == TileType::Destructible || m_Type == TileType::Stationary || m_HasBomb);
}

void CubeTile::SetHasBomb(Bomb* bomb)
{
	m_HasBomb = true;
	m_pBomb = bomb;
}

bool CubeTile::Explode()
{
	if (m_HasBomb)
	{
		m_pBomb->Explode();
	}
	if (m_Type == TileType::Stationary)
	{
		return true;
	}
	m_HasBomb = false;
	if (m_Type == TileType::Destructible || m_Type == TileType::Empty)
	{
		//Spawn Particle
		GameObject* pParticleObject = new GameObject();
		ParticleEmitterComponent* pParticleEmitter = new ParticleEmitterComponent(L"./Resources/Textures/FireBall.png", 60);
		pParticleEmitter->SetVelocity(XMFLOAT3(0, 0.0f, 0));
		pParticleEmitter->SetMinSize(4.5f);
		pParticleEmitter->SetMaxSize(5.0f);
		pParticleEmitter->SetMinEnergy(.25f);
		pParticleEmitter->SetMaxEnergy(.5f);
		pParticleEmitter->SetMinSizeGrow(3.5f);
		pParticleEmitter->SetMaxSizeGrow(6.5f);
		pParticleEmitter->SetMinEmitterRange(0.2f);
		pParticleEmitter->SetMaxEmitterRange(0.5f);
		pParticleEmitter->SetColor(XMFLOAT4(1.f, 1.f, 1.f, 0.6f));
		pParticleObject->AddComponent(pParticleEmitter);
		AddChild(pParticleObject);

		auto pos = m_Position;
		pos.y += 5;
		pParticleEmitter->GetTransform()->Translate(pos);

		m_TimeAlive = 0;
	}
	if (m_Type == TileType::Destructible)
	{
		m_pTileObject->GetComponent<ModelComponent>()->SetMaterial(105);
		m_Shrinking = true;
		return true;
	}
	return false;
}

void CubeTile::ExplosionParticleTimer()
{
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
	else
	{
		auto children = GetChildren<GameObject>();
		for (size_t i = 0; i < children.size(); i++)
		{
			auto emitter = (children[i])->GetComponent<ParticleEmitterComponent>();
			if (emitter)
			{
				emitter->SetMinSize(4.5f * (m_Lifetime - m_TimeAlive) / m_Lifetime);
				emitter->SetMaxSize(5.0f * (m_Lifetime - m_TimeAlive) / m_Lifetime);
				emitter->SetMinSizeGrow(3.5f * (m_Lifetime - m_TimeAlive) / m_Lifetime);
				emitter->SetMaxSizeGrow(6.5f * (m_Lifetime - m_TimeAlive) / m_Lifetime);
			}
		}
	}
}

void CubeTile::HandleShrinking()
{
	if (m_Shrinking)
	{
		float scale = (m_Lifetime - m_TimeAlive) / m_Lifetime;
		m_pTileObject->GetTransform()->Scale(scale, scale, scale);
		if (m_TimeAlive > m_Lifetime)
		{
			RemoveChild(m_pTileObject);
			SafeDelete(m_pTileObject);
			m_Type = TileType::Empty;
			m_Shrinking = false;
		}
	};
}
