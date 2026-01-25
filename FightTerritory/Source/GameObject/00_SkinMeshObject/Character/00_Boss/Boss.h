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


class Shadow;

#include "GameObject//02_StaticMeshObject//01_Portal//Portal.h"

#include <nlohmann/json.hpp>



class Boss final
	: public Character
{
public:
	friend BossContext;
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
	void DecideDifficltyByRound(float raund);

public:
	//難易度用のSet関数.
	void SetDifficulty(BossDifficulty diff);
	//ボスの行動調整関数.
	//ToDo : ボスの移動・弾の発射間隔等を難易度別に実装する関数です.
	void ApplyDifficultyParam();

private:
	//決定アクション.
	//ToDo : 今何をするべきか.
	//       Lプレイヤーがポータルを取得しているとき.
	//		 Lボスがポータルを取得しているとき.
	//		 L誰もとっていないとき.
	//		 Lプレイヤーが死亡してポータル主がプレイヤーの時取得する.
	void DecideAction();

//-----------------------------------------------------------------------
//		ボスの行動関数.
// 	    ToDo : ボスがポータルへすすむようにする関数. 
//-----------------------------------------------------------------------
	//ポータルへボスが進む.
	void MoveToPortal();

	//ポータルへの移動/Easy.
	void PortalMoveEasy();
	//ポータルへの移動/Hard.
	void PortalMoveHard();
	//ポータルへの移動/Final.
	void PortalMoveFinal();

//-----------------------------------------------------------------------
// 		攻撃関数.
//		ToDo : ボスの攻撃関数.
//			　 難易度で攻撃をする動作が変わる.
//-----------------------------------------------------------------------

	//ボスの攻撃の状態を切り替える関数.
	void Attack();

	//ボスのEasyモードのときの攻撃関数.
	void AttackEasy();
	//ボスのHardモードのときの攻撃関数.
	void AttackHard();
	//ボスのFinalモードのときの攻撃関数.
	void AttackFinal();
	//プレイヤーがポータルを取得している.
	//ToDo : この時はボスの攻撃のインターバルを早くしたりする.
	void PlayerMainPortalGetMyAttack();

//-----------------------------------------------------------------------
// 	   防衛関数.
// 	   ToDo : ボスの防衛関数
//			　難易度で防衛する動作が変わる.
//			  Easyモードの時はポータル周面を円移動する.
//			  Hardモードは円運動をしながらジグザグに動く.
//			  Finalはフェイント等を入れてボスの行動を強くする.
//-----------------------------------------------------------------------

	//ボスの防衛の状態を切り替える関数.
	void Defense();

	//ボスのEasyモードのときの防衛関数.
	void DefenseEasy();
	//ボスのHardモードのときの防衛関数.
	void DefenseHard();
	//ボスのFinalモードのときの防衛関数.
	void DefenseFinal();

//-----------------------------------------------------------------------
//		ボスの攻撃等に必要なメンバ変数.
//-----------------------------------------------------------------------
	BossDifficulty m_Difficulty;
	//ボスの速度.
	float m_MoveSpeed;
	//ボスの攻撃のインターバル.
	float m_ShotInterval;
	//防衛時の許容半径.
	float m_DefenseRadius;
	//プレイヤーがポータル主の時の発射速度.
	float m_PressureShotInterval;
	//ポータル取得中か？.
	bool m_IsCapturingPortal;


private:
	BossShotManager* m_pENShotManager;

	float m_ShotCoolDown;	//弾の発射クールダウンタイマー
	float m_CoolTime;		//弾の発射間隔(1.0f)※コンストラクタで設定している.

	BoundingSphere m_BSphere;//敵用バウンディングスフィア

	D3DXVECTOR3 m_HitCenterOffset;

	//Stateで使用する.
	//時間がなかったのでここで初期化をしている.
	BossStateBase* m_pCurrentState = nullptr;


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
	
	std::shared_ptr<Portal> m_pPortal;

};