#include "PShotManager.h"
#include "GameObject/00_SkinMeshObject/Character/01_Player/Player.h"

#include "GameObject/02_StaticMeshObject/01_Portal/Portal.h"

#include "Sound/SoundManager.h"

PShotManager::PShotManager()
{
}

PShotManager::~PShotManager()
{
}

PShotManager* PShotManager::GetInstance()
{
	static PShotManager s_Instance;
	return &s_Instance;
}

void PShotManager::Update()
{
	for (auto it = m_PlayerShot.begin(); it != m_PlayerShot.end();)
	{
		(*it)->Update();	

		if (!(*it)->IsActive())	
		{
			it = m_PlayerShot.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void PShotManager::Draw(const D3DXMATRIX& view, const D3DXMATRIX& proj)
{
	for (auto& shot : m_PlayerShot)
	{
		shot->SetMatrices(view, proj);
		shot->Draw();	
	}
}

void PShotManager::Init()
{
	m_PlayerShot.clear();
}

void PShotManager::AddPlayerShot(const D3DXVECTOR3& Pos, const D3DXVECTOR3& InitDirection)
{
	constexpr float Speed = 0.1f;	//’e‘¬.

	auto PlayerShot = std::make_unique<PShot>();	//V‹K’e¶¬.
	PlayerShot->Reload(Pos, InitDirection, Speed);	//ˆÊ’uE•ûŒüE‘¬“x‚ðÝ’è.
	PlayerShot->SetDisplay(true);	//•`‰æON.

	PlayerShot->Update();

	m_PlayerShot.push_back(std::move(PlayerShot));	//ƒŠƒXƒg‚É’Ç‰Á.

	SoundManager::GetInstance()->PlaySE(SoundManager::SE_Shot);
}

void PShotManager::ReMovePlayerShot(size_t index)
{
	if (index < m_PlayerShot.size())
	{
		m_PlayerShot.erase(m_PlayerShot.begin() + index);	//Žw’è’e‚ðíœ.
	}
}

void PShotManager::ClearPlayerShot()
{
	m_PlayerShot.clear();
}

const std::vector<std::unique_ptr<PShot>>& PShotManager::GetPlayerShot()const
{
	return m_PlayerShot;	//’eƒŠƒXƒg‚ð•Ô‚·.
}

PShot* PShotManager::GetPlayerShot(size_t No)
{
	if (No < m_PlayerShot.size())
	{
		return m_PlayerShot[No].get();	//Žw’è’e‚ðŽæ“¾.
	}
	return nullptr;	//”ÍˆÍŠO.
}

size_t PShotManager::GetPlayerShotCount()const
{
	return m_PlayerShot.size();	//’e‚Ì‘”‚ð•Ô‚·.
}

std::vector<PShot*> PShotManager::GetShots()
{
	std::vector<PShot*> rawPointers;
	for (const auto& shot : m_PlayerShot)
	{
		if (shot)
		{
			rawPointers.push_back(shot.get());
		}
	}
	return rawPointers;
}

void PShotManager::ChackPortalKill(const Portal& portal)
{
	const float killRadius = portal.GetPlayerKillRadius();
	const D3DXVECTOR3 portalPos = portal.GetPosition();

	for (auto& shot : m_PlayerShot)
	{
		if (!shot) continue;
		if (!shot->IsActive()) continue;

		D3DXVECTOR3 diff = shot->GetPosition() - portalPos;
		float dist = D3DXVec3Length(&diff);

		if (dist < killRadius)
		{
			shot->SetActive(false); 
		}
	}
}
