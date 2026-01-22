#include "AttackShort.h"

#include "..//..//Player.h"
#include "..//..//PlayerContext/PlayerContext.h"
#include "System/02_Singleton/00_Timer/Timer.h"

#include "Sound/SoundManager.h"

AttackShort::AttackShort()
	: PlayerAttckStateBase	()
	, m_ShortCoolDown		(0.0f)
	, m_CoolTime			(0.5f)
{
}

AttackShort::~AttackShort()
{
}

void AttackShort::Enter(Player* player)
{
	m_ShortCoolDown = 0.0f;
	m_IsHitActive = false;
	m_HasHit = false;
	// 当たり判定の半径を設定
	m_SwordHitBox.SetRadius(Radius);
}

void AttackShort::ExecuteAttack(Player* player)
{
	// deltaTimeを取得する.
	float deltaTime = Timer::GetInstance().DeltaTime();

	if (m_ShortCoolDown > 0.0f)
	{
		m_ShortCoolDown -= deltaTime;
		m_IsHitActive = true; //クールタイム中は判定を有効化
	}
	else
	{
		m_ShortCoolDown = 0.0f;
		m_IsHitActive = false; //クールタイム終了で無効化
	}

	// 近距離攻撃動作
	if (m_ShortCoolDown <= 0.0f)
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//クールタイム設定
			m_ShortCoolDown = m_CoolTime;
			m_IsHitActive = true;
			m_HasHit = false;

			SoundManager::GetInstance()->PlaySE(SoundManager::SE_Slash);
		}
	}
	std::string BoneName = "blade_r_head";

	//剣先の位置を毎フレーム更新していく.
	D3DXVECTOR3 bonePos;
	//剣先のボーン位置をPlayer経由で取得
	if (player->GetBonePosition(BoneName.c_str(), &bonePos))
	{
		//剣の当たり判定の位置を毎フレーム更新
		m_SwordHitBox.SetPosition(bonePos);
	}
}

void AttackShort::Exit(Player* player)
{
}

void AttackShort::Draw(Player* player)
{
#ifdef _DEBUG
	// 1. 描画条件のチェック
	if (m_IsHitActive && player)
	{
		D3DXVECTOR3 bonePos;

		//当たり判定の位置なんだけどいったん剣先にしています
		//最終的には剣の中心とかかなと思っている.
		std::string BoneName = "blade_r_head";

		//剣先のボーンの位置をPlayer経由で取得.
		if (player->GetBonePosition(BoneName.c_str(), &bonePos))
		{
			//BoundingSphereの位置と半径を設定
			m_SwordHitBox.SetPosition(bonePos);
			m_SwordHitBox.SetRadius(Radius);

			//BoundingSphere::Draw()を呼び出してデバッグ描画
			m_SwordHitBox.Draw();
		}
	}
#endif

}

bool AttackShort::IsFinished() const
{
	return false;
}
