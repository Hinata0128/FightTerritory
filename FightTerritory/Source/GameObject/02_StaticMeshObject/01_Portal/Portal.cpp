#include "Portal.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"
#include "System//00_Manager//03_ImGuiManager//ImGuiManager.h"
#include "System/02_Singleton/00_Timer/Timer.h"
#include "GameObject/00_SkinMeshObject/Character/01_Player/Player.h"
#include "GameObject/00_SkinMeshObject/Character//00_Boss/Boss.h"

#include "GameObject/03_UIObject/03_PortalGauge/PortalGauge.h"

#include "SceneManager/SceneManager.h"

#include "System/00_Manager/02_PShotManager/PShotManager.h"
#include "System/00_Manager/04_BossShotManager/BossShotManager.h"

#include "Sound/SoundManager.h"

Portal::Portal()
	: StaticMeshObject()
	, m_PortalIncreaseF(0.0f)
	, m_PortalIncrease(0)
	, m_pPortalState(PortalPriority::None)

	, m_spPortalGauge(std::make_shared<PortalGauge>())

	, m_IsPlayerPriority(false)
	, m_IsEnemyPriority(false)

	, m_IsRoundFinished(false)

	, WAIT_TIME(2.0f)
	, m_TransitionTimer(0.0f)
	, m_IsTransitionStarted(false)

	, m_IsReadyToLoad(false)

	, m_NextScene(-1)

	, m_BulletKillRadius(4.0f)
{
	AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Portal));

	//ポータルの位置を変更.
	D3DXVECTOR3 Pos = { 0.0f, 0.0f, 5.0f };
	//ポータルのサイズ変更.
	D3DXVECTOR3 Scale = { 1.5f, 1.5f, 1.5f };

	SetPosition(Pos);
	SetScale(D3DXVECTOR3(Scale));

	Init();
}

Portal::~Portal()
{
}

void Portal::Update()
{
	StaticMeshObject::Update();

	PShotManager::GetInstance()->ChackPortalKill(*this);
	BossShotManager::GetInstance()->ChackPortalKill(*this);



	RestrictEntry();

	//100%に達した後の遷移待ち処理
	if (m_IsTransitionStarted)
	{
		float deltaTime = Timer::GetInstance().DeltaTime();
		m_TransitionTimer += deltaTime;

		//2秒待機して演出を見せる
		if (m_TransitionTimer >= 2.0f)
		{
			// 最新のスコアを取得
			int pScore = SceneManager::GetInstance()->GetPlayerScore();
			int eScore = SceneManager::GetInstance()->GetEnemyScore();

			//プレイヤーが2勝した（2-0 または 2-1）
			if (pScore >= 2)
			{
				SceneManager::GetInstance()->ResetScore();
				Init();
				m_NextScene = SceneManager::Win;
			}
			//敵（ボス）が2勝した（0-2 または 1-2）
			else if (eScore >= 2)
			{
				SceneManager::GetInstance()->ResetScore();
				Init();
				m_NextScene = SceneManager::Lose;
			}
			//まだ決着がついていない場合（1-0, 0-1, 1-1）
			else
			{
				//次のラウンドを計算してボスに通知.
				int nextRound = pScore + eScore + 1;
				if (auto enemy = m_pEnemy.lock())
				{
					enemy->StartNextRound(nextRound);
				}

				//1対1になったときの特殊判定
				if (pScore == 1 && eScore == 1)
				{
					Init();
					//最後に100%にした人」の判定（PortalStateに現在の占有者を入れている）
					if (m_pPortalState == PortalPriority::Player) {
						//プレイヤーが追いついて1-1にした場合
						//ここにはFinalRoundに入る処理.
						m_NextScene = SceneManager::Final;
					}
					else {
						//ボスが追いついて1-1にした場合
						//ここにはFinalRoundに入る処理.
						m_NextScene = SceneManager::Final;
					}
				}
				else
				{
					//初めての1ポイント取得時 (1-0 または 0-1)
					Init();
					//ここではSecondRoundに入る処理.
					m_NextScene = SceneManager::Second;
				}
			}
			m_IsReadyToLoad = true;
			return;
		}
	}
	else
	{
		//通常の占有チェック（100%になるまで）
		ChackPriority();

		switch (m_pPortalState)
		{
		case Portal::PortalPriority::Player:
			PlayerToPortal();
			break;
		case Portal::PortalPriority::Enemy:
			EnemyToPortal();
			break;
		default:
			break;
		}
	}

	if (m_spPortalGauge)
	{
		//進捗率を渡す
		m_spPortalGauge->SetPercent(m_PortalIncreaseF / 100.0f);

		//現在の占有状態を見て、UIの表示モード（色）を切り替える
		if (m_pPortalState == PortalPriority::Player)
		{
			m_spPortalGauge->SetGaugeMode(PortalGauge::GaugeMode::Player);
		}
		else if (m_pPortalState == PortalPriority::Enemy)
		{
			m_spPortalGauge->SetGaugeMode(PortalGauge::GaugeMode::Boss);
		}
		else
		{
			m_spPortalGauge->SetGaugeMode(PortalGauge::GaugeMode::None);
		}

		//UIの更新
		m_spPortalGauge->Update();
	}
}

void Portal::Draw()
{
	StaticMeshObject::Draw();
#ifdef _DEBUG
	ImGui::Begin(JAPANESE("Portal System"));
	ImGui::Text(JAPANESE("Player Score: %d / 2"), SceneManager::GetInstance()->GetPlayerScore());
	ImGui::Text(JAPANESE("Enemy Score: %d / 2"), SceneManager::GetInstance()->GetEnemyScore());
	ImGui::Separator();
	ImGui::SliderFloat(JAPANESE("Portal %"), &m_PortalIncreaseF, 0.0f, 100.0f);

	if (m_IsTransitionStarted) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), JAPANESE("決着！シーン遷移まで: %.1f"), 2.0f - m_TransitionTimer);
	}
	ImGui::End();
#endif

}

void Portal::Init()
{
	m_PortalIncreaseF = 0.0f;
	m_PortalIncrease = 0;
	m_pPortalState = PortalPriority::None;

	m_FirstEnterPriority = PortalPriority::None;

	m_IsRoundFinished = false;      // 100%判定をリセット
	m_IsTransitionStarted = false;  // 遷移中フラグをリセット
	m_TransitionTimer = 0.0f;       // タイマーを0にリセット

	m_IsReadyToLoad = false;
	m_NextScene = -1;

	if (auto player = m_pPlayer.lock())
	{
		player->SetFrozen(false);
	}

	if (auto enemy = m_pEnemy.lock())
	{
		enemy->SetFrozen(false);
	}
}

void Portal::ForceFinishByTimeUp()
{
	//すでに100%で決着がついている場合は何もしない
	if (m_IsRoundFinished) return;

	//現在のポータルの状態を見てスコアを加算
	if (m_pPortalState == PortalPriority::Player)
	{
		SceneManager::GetInstance()->AddPlayerScore();
	}
	else
	{
		SceneManager::GetInstance()->AddEnemyScore();
	}

	//終了フラグを立てて、2秒間の待機演出に入る
	m_IsRoundFinished = true;
	m_IsTransitionStarted = true;
	m_TransitionTimer = 0.0f;

	if (auto player = m_pPlayer.lock())
	{
		player->SetFrozen(true);
	}
}

void Portal::RestrictEntry()
{
	const float LIMIT_DISTANCE = 4.0f;
	if (auto player = m_pPlayer.lock())
	{

		if (player->IsDead()) return;

		D3DXVECTOR3 pos = player->GetPosition();
		D3DXVECTOR3 portalPos = GetPosition();

		D3DXVECTOR3 diff = pos - portalPos;
		float distance = D3DXVec3Length(&diff);

		if (distance < LIMIT_DISTANCE)
		{
			//中心から外側に向かう方向ベクトルを正規化
			D3DXVECTOR3 direction;
			D3DXVec3Normalize(&direction, &diff);

			//距離がゼロにいる場合は、適当な方向に逃がす
			if (distance < 0.001f) direction = D3DXVECTOR3(0, 0, 1);

			//制限距離の場所まで強制的に押し出す
			D3DXVECTOR3 newPos = portalPos + direction * LIMIT_DISTANCE;

			//Y軸（高さ）はプレイヤーの現在の高さを維持
			newPos.y = pos.y;

			player->SetPosition(newPos);
		}
	}
	if (auto enemy = m_pEnemy.lock())
	{
		D3DXVECTOR3 pos = enemy->GetPosition();
		D3DXVECTOR3 portalPos = GetPosition();
		D3DXVECTOR3 diff = pos - portalPos;
		float distance = D3DXVec3Length(&diff);

		if (distance < LIMIT_DISTANCE)
		{
			D3DXVECTOR3 direction;
			D3DXVec3Normalize(&direction, &diff);
			if (distance < 0.001f) direction = D3DXVECTOR3(0, 0, 1);
			D3DXVECTOR3 newPos = portalPos + direction * LIMIT_DISTANCE;
			newPos.y = pos.y;
			enemy->SetPosition(newPos);
		}
	}
}

void Portal::SetPortalState(PortalPriority state)
{
	if (m_pPortalState == state) return;
	m_pPortalState = state;
}

void Portal::ChackPriority()
{
	constexpr float PORTAL_DISTANCE = 5.0f;
	float playerDistance = 1000.0f;
	float enemyDistance = 1000.0f;

	m_IsPlayerPriority = false;
	if (auto player = m_pPlayer.lock())
	{
		if (!player->IsDead())
		{
			D3DXVECTOR3 diff = player->GetPosition() - GetPosition();
			playerDistance = D3DXVec3Length(&diff);
			if (playerDistance <= PORTAL_DISTANCE) m_IsPlayerPriority = true;
		}
	}

	m_IsEnemyPriority = false;
	if (auto enemy = m_pEnemy.lock())
	{
		D3DXVECTOR3 Diff = enemy->GetPosition() - GetPosition();
		enemyDistance = D3DXVec3Length(&Diff);
		if (enemyDistance <= PORTAL_DISTANCE) m_IsEnemyPriority = true;
	}

	if (m_IsPlayerPriority && !m_IsEnemyPriority)
	{
		if (m_pPortalState != PortalPriority::Player)
		{
			if (auto player = m_pPlayer.lock()) player->SetCaptureState(1.0f);
			m_FirstEnterPriority = PortalPriority::Player;

			SoundManager::GetInstance()->PlaySE(SoundManager::SE_PortalGet);
		}
		m_pPortalState = PortalPriority::Player;
	}
	else if (!m_IsPlayerPriority && m_IsEnemyPriority)
	{
		if (m_pPortalState != PortalPriority::Enemy)
		{
			m_FirstEnterPriority = PortalPriority::Enemy;
			SoundManager::GetInstance()->PlaySE(SoundManager::SE_PortalGet);
		}
		m_pPortalState = PortalPriority::Enemy;
	}
	else if (m_IsPlayerPriority && m_IsEnemyPriority)
	{
		m_pPortalState = m_FirstEnterPriority;
	}
}

void Portal::PlayerToPortal()
{
	if (m_IsRoundFinished) return;

	float deltaTime = Timer::GetInstance().DeltaTime();

	float SpeedMultiplier = (m_PortalIncreaseF <= 50.0f) ? 2.0f : 1.0f;

	m_PortalIncreaseF += deltaTime * SpeedMultiplier;
	m_PortalIncrease = static_cast<int>(m_PortalIncreaseF);

	if (m_PortalIncreaseF >= 100.0f)
	{
		m_PortalIncreaseF = 100.0f;
		m_PortalIncrease = 100;

		m_IsRoundFinished = true;
		m_IsTransitionStarted = true;
		m_TransitionTimer = 0.0f;

		if (auto player = m_pPlayer.lock())
		{
			player->SetFrozen(true);
		}

		if (auto enemy = m_pEnemy.lock())
		{
			enemy->SetFrozen(true);
		}

		// スコアを加算
		SceneManager::GetInstance()->AddPlayerScore();
	}
}

void Portal::EnemyToPortal()
{
	if (m_IsRoundFinished) return;

	float deltaTime = Timer::GetInstance().DeltaTime();
	m_PortalIncreaseF += deltaTime * 2.0f;
	m_PortalIncrease = static_cast<int>(m_PortalIncreaseF);

	if (m_PortalIncreaseF >= 100.0f)
	{
		m_PortalIncreaseF = 100.0f;
		m_PortalIncrease = 100;

		m_IsRoundFinished = true;
		m_IsTransitionStarted = true;
		m_TransitionTimer = 0.0f;

		if (auto player = m_pPlayer.lock())
		{
			player->SetFrozen(true);
		}

		if (auto enemy = m_pEnemy.lock())
		{
			enemy->SetFrozen(true);
		}

		//スコアを加算
		SceneManager::GetInstance()->AddEnemyScore();
	}
}