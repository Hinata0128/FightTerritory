#include "Com.h"

#include "GameObject//02_StaticMeshObject//01_Portal//Portal.h"

#include "GameObject/00_SkinMeshObject/Character//00_Boss/Boss.h"

#include "System/00_Manager/03_ImGuiManager/ImGuiManager.h"

#include "System/02_Singleton/00_Timer/Timer.h"

#include "..//00_BossContext/BossContext.h"

Com::Com(
	Boss* pOwner,
	std::shared_ptr<Portal> pPortal)
	: m_pPortal	(pPortal)
	, m_pOwner(pOwner)

	, m_Difficulty(BossDifficulty::Easy)

	, m_MoveSpeed(1.5f)

	, m_ShotInterval(0.0f)
	, m_DefenseRadius(0.0f)

	, m_PressureShotInterval(0.0f)

	, m_IsCapturingPortal(false)
{
	ApplyDifficultyParam();
}

Com::~Com()
{
}

void Com::Update()
{
	DecideAction();
}

void Com::DrawDebugImGui()
{
#ifdef _DEBUG
	ImGui::Begin(JAPANESE("Com: 内部ステータス"));

	//ポータルの認識状態
	Portal::PortalPriority PortalState = m_pPortal->GetPortalState();
	const char* stateStr = "Unknown";
	ImVec4 color = ImVec4(1, 1, 1, 1); // デフォルト白

	if (PortalState == Portal::PortalPriority::None) {
		stateStr = "None";
		color = ImVec4(0, 1, 0, 1); // 緑
	}
	else if (PortalState == Portal::PortalPriority::Player) {
		stateStr = "Player (Attack!)";
		color = ImVec4(0, 0, 1, 1); // 青
	}
	else if (PortalState == Portal::PortalPriority::Enemy) {
		stateStr = "Enemy (Defense)";
		color = ImVec4(1, 0, 0, 1); // 赤
	}

	ImGui::Text(JAPANESE("AIが見ている状態: "));
	ImGui::SameLine();
	ImGui::TextColored(color, stateStr);

	//距離情報の表示
	D3DXVECTOR3 BossPos = m_pOwner->GetPosition();
	D3DXVECTOR3 PortalPos = m_pPortal->GetPosition();
	D3DXVECTOR3 Length = PortalPos - BossPos;
	float Pos_Lenght = D3DXVec3Length(&Length);

	ImGui::Text(JAPANESE("ポータルへの距離: %.2f"), Pos_Lenght);
	ImGui::Text(JAPANESE("占拠可能距離: 5.0f"));

	//実行中の行動推測
	if (PortalState == Portal::PortalPriority::None && Pos_Lenght > 5.0f) {
		ImGui::BulletText(JAPANESE("現在: MoveToPortal (移動中)"));
	}
	else if (PortalState == Portal::PortalPriority::Enemy) {
		ImGui::BulletText(JAPANESE("現在: Defense (防衛/AutoShot)"));
	}
	else if (PortalState == Portal::PortalPriority::Player) {
		ImGui::BulletText(JAPANESE("現在: PlayerAttack (攻撃)"));
	}
	else {
		ImGui::BulletText(JAPANESE("現在: 待機/占拠中"));
	}

	ImGui::End();
#endif
}

void Com::DecideDifficultyByRound(int raund)
{
	int Raund = rand() % 100;
#if 1
	if (raund == 1)
	{
		//9割りEasyモード.
		if (Raund < 90)
		{
			SetDifficulty(BossDifficulty::Easy);
		}
		//残りHardモード.
		else
		{
			SetDifficulty(BossDifficulty::Hard);
		}
	}
	else if (raund == 2)
	{
		//6割りHardモード.
		if (Raund < 60)
		{
			SetDifficulty(BossDifficulty::Hard);
		}
		//残りEasyモード.
		else
		{
			SetDifficulty(BossDifficulty::Easy);
		}
	}
	//最終ラウンドは、固定.
	else if (raund == 3)
	{
		SetDifficulty(BossDifficulty::Final);
	}

#else
	//ToDo : いったんEasyでしていく
	SetDifficulty(BossDifficulty::Easy);
#endif // 0

}

//難易度用Set関数.
void Com::SetDifficulty(BossDifficulty diff)
{
	//引数代入.
	m_Difficulty = diff;
	ApplyDifficultyParam();
}

void Com::ApplyDifficultyParam()
{
#if 1
	if (m_Difficulty == BossDifficulty::Easy)
	{
		m_MoveSpeed = 1.2f;
		m_ShotInterval = 5.0f;
		m_PressureShotInterval = 2.5f;
		m_DefenseRadius = 6.0f;
	}
	else if (m_Difficulty == BossDifficulty::Hard)
	{
		m_MoveSpeed = 3.5f;
		m_ShotInterval = 2.0f;
		m_PressureShotInterval = 1.5f;
		m_DefenseRadius = 8.0f;
	}
	else if (m_Difficulty == BossDifficulty::Final)
	{
		m_MoveSpeed = 5.0f;
		m_ShotInterval = 1.0f;
		m_PressureShotInterval = 0.5f;
		m_DefenseRadius = 10.0f;
	}
#else
	if (m_Difficulty == BossDifficulty::Easy)
	{
		m_MoveSpeed = 1.2f;
		m_ShotInterval = 5.0f;
		m_PressureShotInterval = 2.5f;
		m_DefenseRadius = 6.0f;
	}
#endif

	//Boss側に発射間隔を反映.
	m_pOwner->SetShotInterval(m_ShotInterval);

}


//優先度1の処理を書く関数.
void Com::DecideAction()
{
	//敵の位置を取得する
	D3DXVECTOR3 BossPos = m_pOwner->GetPosition();
	//ポータルの位置を取得する
	D3DXVECTOR3 PortalPos = m_pPortal->GetPosition();

	//ポータルから敵への距離を計算
	D3DXVECTOR3 Diff = PortalPos - BossPos;
	float Dist = D3DXVec3Length(&Diff);

	//現在のポータルの占有状態を取得
	Portal::PortalPriority state = m_pPortal->GetPortalState();

	//ターゲットの死亡状態を取得
	bool isPlayerDead = m_pOwner->IsTargetDead();


	if (state == Portal::PortalPriority::Player && isPlayerDead)
	{
		m_IsCapturingPortal = true;
	}

	if (m_IsCapturingPortal)
	{
		// 奪取完了
		if (state == Portal::PortalPriority::Enemy)
		{
			m_IsCapturingPortal = false;
		}
		else
		{
			m_pOwner->SetShotInterval(m_ShotInterval);
			MoveToPortal();
			m_pOwner->RequestShot();
			return;
		}
	}

	if (state == Portal::PortalPriority::Player && !isPlayerDead)
	{
		// プレイヤー占有中専用の連射速度に設定
		m_pOwner->SetShotInterval(m_PressureShotInterval);
		// プレイヤーを攻撃しながら間合いを調整する
		PlayerPressureAttack();
		return;
	}

	// 邪魔者がいないので、ポータルを奪取するために中心へ移動する
	if (state == Portal::PortalPriority::None || (state == Portal::PortalPriority::Player && isPlayerDead))
	{
		m_pOwner->SetShotInterval(m_ShotInterval);
		MoveToPortal();
		m_pOwner->RequestShot();
		return;
	}

	// 自分の陣地を守るための防衛行動に切り替える
	if (state == Portal::PortalPriority::Enemy)
	{
		m_pOwner->SetShotInterval(m_ShotInterval);
		DefenseFinal();
		return;
	}
}

void Com::MoveToPortal()
{
	float dt = Timer::GetInstance().DeltaTime();

	D3DXVECTOR3 bossPos = m_pOwner->GetPosition();
	D3DXVECTOR3 portalPos = m_pPortal->GetPosition();
	D3DXVECTOR3 dir = portalPos - bossPos;

	float len = D3DXVec3Length(&dir);
	BossContext ctx(m_pOwner);

	if (len > 0.01f)
	{
		D3DXVec3Normalize(&dir, &dir);
		m_pOwner->AddPosition(dir * m_MoveSpeed * dt);

		float angle = std::atan2f(-dir.x, -dir.z);
		m_pOwner->SetRotationY(angle);

		const int WALK_ANIM = 2;
		if (ctx.AnimNo != WALK_ANIM)
		{
			ctx.AnimNo = WALK_ANIM;
			ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
		}
	}
	else
	{
		const int IDLE_ANIM = 0;
		if (ctx.AnimNo != IDLE_ANIM)
		{
			ctx.AnimNo = IDLE_ANIM;
			ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
		}
	}
	m_pOwner->RequestShot();
}

void Com::PlayerAttack()
{

	BossContext ctx(m_pOwner);

	//歩きアニメ
	const int WALK_ANIM = 2;
	if (ctx.AnimNo != WALK_ANIM)
	{
		ctx.AnimNo = WALK_ANIM;
		ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
	}

	m_pOwner->RequestShot();
}

void Com::PlayerPressureAttack()
{
	const float IDEAL_DISTANCE = 10.0f;

	float dt = Timer::GetInstance().DeltaTime();

	D3DXVECTOR3 bossPos = m_pOwner->GetPosition();
	D3DXVECTOR3 playerPos = m_pOwner->GetPlayerPos();

	D3DXVECTOR3 toPlayer = playerPos - bossPos;
	float dist = D3DXVec3Length(&toPlayer);
	if (dist < 0.01f) return;

	D3DXVec3Normalize(&toPlayer, &toPlayer);

	//プレイヤーを見る
	float angle = std::atan2f(-toPlayer.x, -toPlayer.z);
	m_pOwner->SetRotationY(angle);

	BossContext ctx(m_pOwner);
	const int WALK_ANIM = 2; //歩きアニメーション
	if (ctx.AnimNo != WALK_ANIM)
	{
		ctx.AnimNo = WALK_ANIM;
		ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
	}

	D3DXVECTOR3 move(0, 0, 0);

	//距離調整（近すぎるなら離れ、遠すぎるなら近づく）
	if (dist < IDEAL_DISTANCE - 1.0f)
	{
		move -= toPlayer;
	}
	else if (dist > IDEAL_DISTANCE + 1.0f)
	{
		move += toPlayer;
	}

	//横移動
	D3DXVECTOR3 up(0, 1, 0);
	D3DXVECTOR3 side;
	D3DXVec3Cross(&side, &up, &toPlayer);
	move += side * 0.7f;

	D3DXVec3Normalize(&move, &move);
	m_pOwner->AddPosition(move * m_MoveSpeed * dt);

	//攻撃
	m_pOwner->RequestShot();
}

void Com::DefenseEasy()
{
	float deltaTime = Timer::GetInstance().DeltaTime();
	//ボスの位置.
	D3DXVECTOR3 BossPos_v = m_pOwner->GetPosition();
	//ポータルの位置.
	D3DXVECTOR3 PortalPos_v = m_pPortal->GetPosition();
	//プレイヤーの位置.
	D3DXVECTOR3 PlayerPos_v = m_pOwner->GetPlayerPos();
	
	BossContext ctx(m_pOwner);

	//ボスがプレイヤーを注視する.
	D3DXVECTOR3 look = PlayerPos_v - BossPos_v;
	look.y = 0.0f;
	D3DXVec3Normalize(&look, &look);
	float Angle = std::atan2f(-look.x, -look.z);
	m_pOwner->SetRotationY(Angle);

	//ポータルの中心基準の方向.
	D3DXVECTOR3 ToBoss = BossPos_v - PortalPos_v;
	ToBoss.y = 0.0f;

	float Dist = D3DXVec3Length(&ToBoss);
	if (Dist < 0.01f)
	{
		return;
	}

	D3DXVec3Normalize(&ToBoss, &ToBoss);

	//円運動の接線ベクトル.
	D3DXVECTOR3 Up(0, 1, 0);
	D3DXVECTOR3 Tangent;
	D3DXVec3Cross(&Tangent, &Up, &ToBoss);

	//円運動.
	D3DXVECTOR3 Velocity = Tangent * m_MoveSpeed;

	//半径制御.
	if (Dist > m_DefenseRadius)
	{
		Velocity -= ToBoss * m_MoveSpeed;
	}
	m_pOwner->AddPosition(Velocity * deltaTime);

	//歩きアニメーション.
	const int WALK_ANIMATION = 2;
	if (ctx.AnimNo != WALK_ANIMATION)
	{
		ctx.AnimNo = WALK_ANIMATION;
		ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
	}
	//攻撃.
	m_pOwner->RequestShot();
}

void Com::DefenseHard()
{
	float deltaTime = Timer::GetInstance().DeltaTime();
	//累計時間の取得.
	float totalTime = Timer::GetInstance().ElapsedTime();
	//ボスの位置.
	D3DXVECTOR3 BossPos_v = m_pOwner->GetPosition();
	//ポータルの位置.
	D3DXVECTOR3 PortalPos_v = m_pPortal->GetPosition();
	//プレイヤーの位置.
	D3DXVECTOR3 PlayerPos_v = m_pOwner->GetPlayerPos();

	BossContext ctx(m_pOwner);

	//ボスがプレイヤーを注視する.
	D3DXVECTOR3 Look = PlayerPos_v - BossPos_v;
	Look.y = 0.0f;
	D3DXVec3Normalize(&Look, &Look);
	float Angle = atan2f(-Look.x, -Look.z);
	m_pOwner->SetRotationY(Angle);

	//ポータルからボスへの方向.
	D3DXVECTOR3 ToBoss = BossPos_v - PortalPos_v;
	ToBoss.y = 0.0f;
	float CurrentDist = D3DXVec3Length(&ToBoss);
	if (CurrentDist < 0.01f)
	{
		return;
	}

	D3DXVECTOR3 Up(0, 1, 0);
	D3DXVECTOR3 Tangent;
	D3DXVec3Cross(&Tangent, &Up, &ToBoss);

	//回転速度の調整.
	//ボスの回転速度の調整.
	float rotationSpeedRate = 0.4f;
	D3DXVECTOR3 horizontalVelocity = Tangent * (m_MoveSpeed * rotationSpeedRate);

	//ジグザグの調整
	float zigzagFrequency = 2.0f;   //揺れる周期（速さ）
	float zigzagAmplitude = 1.5f;   //揺れる幅
	float Wave = sinf(totalTime * zigzagFrequency) * zigzagAmplitude;

	//前後移動の速度係数.
	//ジグザグの動き自体の移動スピードを抑えたい場合はここを調整
	float zigzagSpeedRate = 0.5f;
	D3DXVECTOR3 forwardVelocity = ToBoss * (Wave * zigzagSpeedRate);

	//最終的な速度ベクトルの合成.
	D3DXVECTOR3 Velocity = horizontalVelocity + forwardVelocity;

	//半径維持の引き戻し
	if (CurrentDist > m_DefenseRadius)
	{
		Velocity -= ToBoss * (m_MoveSpeed * 0.5f);
	}

	m_pOwner->AddPosition(Velocity * deltaTime);

	// アニメーション・攻撃
	const int WALK_ANIMATION = 2;
	if (ctx.AnimNo != WALK_ANIMATION)
	{
		ctx.AnimNo = WALK_ANIMATION;
		ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
	}
	m_pOwner->RequestShot();
}

void Com::DefenseFinal()
{
	float deltaTime = Timer::GetInstance().DeltaTime();
	float totalTime = Timer::GetInstance().ElapsedTime();

	D3DXVECTOR3 BossPos_v = m_pOwner->GetPosition();
	D3DXVECTOR3 PortalPos_v = m_pPortal->GetPosition();
	D3DXVECTOR3 PlayerPos_v = m_pOwner->GetPlayerPos();

	// 1. プレイヤーを注視（これは基本）
	D3DXVECTOR3 Look = PlayerPos_v - BossPos_v;
	Look.y = 0.0f;
	D3DXVec3Normalize(&Look, &Look);
	float Angle = atan2f(-Look.x, -Look.z);
	m_pOwner->SetRotationY(Angle);

	// 2. 基本ベクトル計算
	D3DXVECTOR3 ToBoss = BossPos_v - PortalPos_v;
	ToBoss.y = 0.0f;
	float CurrentDist = D3DXVec3Length(&ToBoss);
	if (CurrentDist < 0.01f) return;
	D3DXVec3Normalize(&ToBoss, &ToBoss);

	D3DXVECTOR3 Up(0, 1, 0);
	D3DXVECTOR3 Tangent;
	D3DXVec3Cross(&Tangent, &Up, &ToBoss);

	// --- ★難易度アップの仕掛け：二重サイン波とカオス移動 ---

	// A. 複雑な軌道（円運動に「うねり」を加える）
	// 二つの異なる周期のサイン波を合成して、移動パターンを読ませない
	float waveA = sinf(totalTime * 3.5f) * 2.0f;
	float waveB = cosf(totalTime * 7.0f) * 1.5f;
	float finalWave = waveA + waveB;

	// B. 4秒周期の強烈な緩急パターン
	float patternTime = fmodf(totalTime, 4.0f);
	float rotationSpeedRate = 1.0f;
	float moveSpeedMultiplier = 1.0f;
	bool isEnraged = false; // 猛攻フラグ

	if (patternTime < 1.0f) {
		// 【超高速旋回】一気に回り込む
		rotationSpeedRate = 2.5f;
		moveSpeedMultiplier = 1.5f;
	}
	else if (patternTime < 1.5f) {
		// 【急ブレーキ & 溜め】一瞬止まって連射準備（あえて隙を見せて誘う）
		rotationSpeedRate = 0.2f;
		finalWave = 0.0f;
		m_pOwner->SetShotInterval(m_PressureShotInterval * 0.5f); // 限界まで連射
	}
	else if (patternTime < 2.5f) {
		// 【カウンター・ダッシュ】プレイヤーに向かって斜め前に踏み込む
		rotationSpeedRate = -1.5f; // 逆回転しつつ
		ToBoss *= -2.5f;          // 急接近（マイナスはPortal方向＝プレイヤー接近）
		moveSpeedMultiplier = 2.0f;
		isEnraged = true;
	}
	else {
		// 【離脱】大きく距離をとって仕切り直し
		rotationSpeedRate = 1.8f;
		ToBoss *= 2.0f; // Portalから離れる
	}

	// 速度の合成
	D3DXVECTOR3 horizontalVel = Tangent * (m_MoveSpeed * rotationSpeedRate * moveSpeedMultiplier);
	D3DXVECTOR3 forwardVel = ToBoss * (finalWave * moveSpeedMultiplier);
	D3DXVECTOR3 Velocity = horizontalVel + forwardVel;

	// 半径維持（Finalは守備範囲を広くしつつ、外に出過ぎたら超高速で戻る）
	if (CurrentDist > m_DefenseRadius) {
		Velocity -= ToBoss * (m_MoveSpeed * 2.0f);
	}

	m_pOwner->AddPosition(Velocity * deltaTime);

	// --- 3. アニメーション：速度に合わせて再生速度を変えるイメージ ---
	BossContext ctx(m_pOwner);
	const int WALK_ANIM = 2;
	if (ctx.AnimNo != WALK_ANIM) {
		ctx.AnimNo = WALK_ANIM;
		ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
	}
	// 移動速度が速いときはアニメーションを少し早送りする演出も効果的
	// (ctx.AnimCtrl->SetSpeed 等があれば moveSpeedMultiplier を渡すと◎)

	m_pOwner->RequestShot();
}