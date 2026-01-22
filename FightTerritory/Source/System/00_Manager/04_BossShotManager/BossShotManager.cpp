#include "BossShotManager.h"

#include "GameObject/02_StaticMeshObject/01_Portal/Portal.h"

BossShotManager* BossShotManager::m_pInstance = nullptr;

BossShotManager::BossShotManager()
{
}

BossShotManager::~BossShotManager()
{
}

BossShotManager* BossShotManager::GetInstance()
{
	static BossShotManager s_Instance;	
	return &s_Instance;
}

void BossShotManager::Update()
{
	for (auto it = m_pBossShot.begin(); it != m_pBossShot.end();)
	{
		(*it)->Update();	//’e‚ÌXVˆ—.

		if (!(*it)->IsActive())	//”ñƒAƒNƒeƒBƒu‚È’e‚Ííœ.
		{
			it = m_pBossShot.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void BossShotManager::Draw(const D3DXMATRIX& view, const D3DXMATRIX& proj)
{
	for (std::unique_ptr<BossShot>& ENomalShot : m_pBossShot)
	{
		ENomalShot->SetMatrices(view, proj);
		ENomalShot->Draw();	//’e‚Ì•`‰æˆ—.

	}
}

void BossShotManager::Init()
{
	m_pBossShot.clear();
}

void BossShotManager::AddEnemyNomalShot(const D3DXVECTOR3& Pos, const D3DXVECTOR3& InitDirecton)
{
	constexpr float Speed = 0.05f;	//’e‚Ì‘¬“xİ’è.

	auto ENomalShot = std::make_unique<BossShot>();	//’e‚Ì¶¬.

	ENomalShot->Reload(Pos, InitDirecton, Speed);	//’e‚Ì‰Šú‰».

	ENomalShot->SetDisplay(true);	//•\¦—LŒø‰».

	m_pBossShot.push_back(std::move(ENomalShot));	//ƒŠƒXƒg‚É’Ç‰Á.
}

void BossShotManager::Clear()
{
	m_pBossShot.clear();
}

void BossShotManager::ReMoveEnemyNomalShot(size_t index)
{
	if (index < m_pBossShot.size())
	{
		m_pBossShot.erase(m_pBossShot.begin() + index);	//w’è’eíœ.
	}
}

const std::vector<std::unique_ptr<BossShot>>& BossShotManager::GetEnemyNomalShot() const
{
	return m_pBossShot;	//’eƒŠƒXƒg•Ô‹p.
}

BossShot* BossShotManager::GetEnemyNomalShot(size_t No)
{
	if (No < m_pBossShot.size())
	{
		return m_pBossShot[No].get();	//w’è’e•Ô‹p.
	}
	return nullptr;
}

size_t BossShotManager::GetEnemyNomalShotCount() const
{
	return m_pBossShot.size();	//’e”•Ô‹p.
}

std::vector<BossShot*> BossShotManager::GetShots()
{
	std::vector<BossShot*> rawPointers;

	for (const auto& shot : m_pBossShot)
	{
		if (shot)
		{
			rawPointers.push_back(shot.get());
		}
	}
	return rawPointers; // ’l‚Æ‚µ‚Ä•Ô‚·
}

void BossShotManager::ChackPortalKill(const Portal& portal)
{
	const float killRadius = portal.GetBulletKillRadius();
	const D3DXVECTOR3 portalPos = portal.GetPosition();

	for (auto& shot : m_pBossShot)
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