#include "PShot.h"
#include "System/00_Manager/01_StaticMeshManager/StaticMeshManager.h"

PShot::PShot()
	: ShotBase()

	, m_pShadowSmall(std::make_unique<ShadowSmall>())
{
	AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Bullet));

	m_pShadowSmall->Create();
	m_pShadowSmall->SetTargetShadowSmallPos(this);

	Init();
}

PShot::~PShot()
{

}

void PShot::Update()
{
	constexpr float MaxRange = 15000.0f;

	if (!m_IsActive) return;

	if (m_Disp)
	{
		//位置更新
		m_Position += m_Direction * m_Speed;

		//一定距離を超えたらリセット
		if (D3DXVec3Length(&m_Position) >= MaxRange)
		{
			Init();
			return;
		}
		//バウンディングスフィアの位置同期
		UpdateBPosition();

		//影の実装.
		m_pShadowSmall->Update();
	}
}

void PShot::Draw()
{
	if (m_Disp)
	{
		ShotBase::Draw();
		m_pShadowSmall->Draw();
	}
}

void PShot::Init()
{
	//親クラスの基本初期化
	ShotBase::Init();

	//見た目の大きさ（プレイヤーの弾）
	m_Scale = D3DXVECTOR3(5.0f, 5.0f, 5.0f);

	//当たり判定の大きさ（スケールに合わせて）
	if (m_BSphere)
	{
		m_BSphere->SetRadius(1.0f); 
	}
}

void PShot::Reload(
	const D3DXVECTOR3& pos,
	const D3DXVECTOR3& direction,
	float speed)
{
	m_Position = pos;
	m_Direction = direction;
	m_Speed		= speed;

	m_Disp		= true;
	m_IsActive	= true;

	if (m_BSphere)
	{
		m_BSphere->SetPosition(m_Position);
		m_BSphere->SetRadius(0.5f);
	}

	m_pShadowSmall->SetTargetShadowSmallPos(this);
	if (m_pShadowSmall)
	{
		m_pShadowSmall->Update();
	}
}
