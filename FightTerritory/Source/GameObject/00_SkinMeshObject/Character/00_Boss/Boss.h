#pragma once
#include "GameObject//00_SkinMeshObject//Character//Character.h"
#include "Collision//BoundingSphere//BoundingSphere.h" 

#include "..//00_Boss//01_BossStateBase//00_BossIdol//BossIdol.h"
#include "..//00_Boss//01_BossStateBase//02_BossDead//BossDead.h"
#include "..//00_Boss//01_BossStateBase//03_BossPortalAnim//BossPortalAnim.h"

class BossShotManager;
class Timer;
class BossContext;
class BossStateBase;

class Com;

class Shadow;

#include "GameObject//02_StaticMeshObject//01_Portal//Portal.h"



class Boss final
	: public Character
{
public:
	friend BossContext;
	friend Com;
public:
	enum class BossDifficulty : byte
	{
		Easy,
		Hard,
		Final
	};
public:
	Boss(std::shared_ptr<Portal> pPortal);
	~Boss() override;

	void Update() override;
	void Draw() override;
	void Init() override;

	void Hit();

	//弾のクールタイムを管理.
	bool CanShot() const;
	//弾の発射準備確認.
	//ToDO : 発射可能になったら飛ばす.
	void RequestShot();

	void SetShotInterval(float time);

	void StartNextRound(int nextround);

	void Respawn();
	
public:

	BoundingSphere& GetBoundingSphere() { return m_BSphere; }

	D3DXVECTOR3 GetHitCenter() const;

	void SetEnemyPosition(const D3DXVECTOR3& pos)
	{
		GameObject::SetPosition(pos);
	}

	void SetTargetPos(const D3DXVECTOR3& pos)
	{
		m_pPlayerPos = pos;
	}

	//NomalMoveClassがプレイヤーの位置を取得するためのGet関数.
	const D3DXVECTOR3& GetPlayerPos() const { return m_pPlayerPos; }

	//ポータルの位置を取得する.
	void SetPortalPos(const D3DXVECTOR3& portalpos)
	{
		m_pPortalPos = portalpos;
	}

	//AIの処理で必要になる.
	const D3DXVECTOR3& GetPortalPos() const { return m_pPortalPos; }

	//HPを取得する関数.
	float GetEnemyHitPoint() const { return m_HitPoint; }

	//死亡しているかどうか.
	bool IsDaed() const { return m_pCurrentState == (BossStateBase*)m_pDead.get(); }
	
	//今のラウンドを確認する用の関数.
	int GetCurrentRound() const { return m_CurrentRound; }

public:
	void ChangeState(BossStateBase* state);

	void AutoShot();
	//publicでStateの遷移をしている.
	std::unique_ptr<BossIdol> m_pIdol;
	std::unique_ptr<BossDead> m_pDead;
	std::unique_ptr<BossPortalAnim> m_pBossAnim;

	void SetTargetDead(bool isDead)		{ m_IsTargetDead = isDead; }
	bool IsTargetDead() const			{ return m_IsTargetDead; }

	void SetVisible(bool visible)		{ m_IsVisible = visible; }
	bool IsVisible() const				{ return m_IsVisible; }

	void SetFrozen(bool frozen)			{ m_IsFrozen = frozen; }
	bool IsFrozen() const				{ return m_IsFrozen; }

	Shadow* GetShadow() const { return m_pShadow.get(); }

	//ポータルアニメーション用関数.
	//ToDo : ボスのポータル取得アニメーションを再生する.
	void SetCaptureState(float duration);
	bool IsCapturingState() const;
public:
	//ラウンドごとに難易度を設定.
	//ToDo : ボスの強さ設定をする.
	void DecideDifficltyByRound(float raudo);

public:
	//難易度用のSet関数.
	void SetDifficulty(BossDifficulty diff);
	//難易度を反映させる.
	void ApplyDifficultyParam();

private:
	//決定アクション.
	void DecideAction();

	//ポータルへボスが進む.
	void MoveToPortal();

private:
	BossShotManager* m_pENShotManager;

	float m_ShotCoolDown;	//弾の発射クールダウンタイマー
	float m_CoolTime;		//弾の発射間隔(1.0f)※コンストラクタで設定している.

	BoundingSphere m_BSphere;//敵用バウンディングスフィア

	D3DXVECTOR3 m_HitCenterOffset;

	//Stateで使用する.
	//時間がなかったのでここで初期化をしている.
	BossStateBase* m_pCurrentState = nullptr;

	std::unique_ptr<Com> m_pCom;

	D3DXVECTOR3 m_InitialPosition;
	//現在のラウンド数(開始は1から).
	int m_CurrentRound;

	bool m_IsTargetDead = false;

	//ボスの死んだときの表示・非表示フラグ.
	bool m_IsVisible;

	bool m_IsFrozen;

	D3DXVECTOR3 m_pPlayerPos;
	//ポータルの位置の取得.
	D3DXVECTOR3 m_pPortalPos;

	std::shared_ptr<Shadow> m_pShadow;
	std::unique_ptr<Player> m_pPlayer;

	//ポータル取得演出の残り時間.
	float m_CaptureTimer;

};