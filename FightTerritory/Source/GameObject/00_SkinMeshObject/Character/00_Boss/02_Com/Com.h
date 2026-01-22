#pragma once

/************************************************************************************
*	ボスの行動(AIの制御クラス)
*	優先度を作成してボスの攻撃を実装していきます.
*	優先度順番.
*	1. ボスがポータルの方に移動する.
*	2. ポータルを取得したら防衛状態に入る(防衛の時も攻撃はする).
*	2. ポータルがPlayerに取得されたら攻撃状態に入る(自分が取得していたいから攻撃をする).
*	3. 普通の攻撃.
*	こんな感じの順番で実装していく.
**/

class Portal;
class Boss;

class Timer;

class BossContext;

class Com
{
public:
	//ボスの難易度の列挙.
	enum class BossDifficulty : byte
	{
		Easy,	//1.2ラウンド目.
		Hard,	//1.2ラウンド目.
		Final	//3ラウンド目限定.
	};
public:
	Com(
		Boss* pOwner,
		std::shared_ptr<Portal> pPortal);
	~Com();

	//ボスのAIを毎フレーム実装.
	void Update();

	//デバッグで表示する.
	void DrawDebugImGui();
	//ラウンドごとに難易度を設定.
	void DecideDifficultyByRound(int raund);

public:
	//難易度用Set関数.
	void SetDifficulty(BossDifficulty diff);

	//難易度を反映させる.
	void ApplyDifficultyParam();

private:
	//決定アクション.
	void DecideAction();

	//優先度1: ポータルへの移動関数.
	void MoveToPortal();
	//優先度2: 攻撃(通常時).
	void PlayerAttack();
	//攻撃(プレイヤーが取得している).
	//ToDo : プレイヤーがポータルを取得している時に入れる攻撃用の関数.
	void PlayerPressureAttack();
	//優先度2: 防衛(Easyモードの時).
	void DefenseEasy();
	//優先度2: 防衛(Hardモードの時).
	void DefenseHard();


private:
	std::shared_ptr<Portal> m_pPortal;

	Boss* m_pOwner;

	BossDifficulty m_Difficulty;

	//ボスの歩く速度.
	float m_MoveSpeed;

	//難易度依存のパラメーター.
	//弾の発射間隔.
	float m_ShotInterval;
	//防衛時の許容半径.
	float m_DefenseRadius;
	//プレイヤーがポータル主の時の発射速度.
	float m_PressureShotInterval;
	//ポータル取得中か？.
	bool m_IsCapturingPortal;
};