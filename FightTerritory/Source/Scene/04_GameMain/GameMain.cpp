#include "GameMain.h"
#include "Sound//SoundManager.h"
#include "Effect//Effect.h"

#include "System//02_Singleton//00_Timer//Timer.h"
#include "System//00_Manager//03_ImGuiManager//ImGuiManager.h"

#include "System/00_Manager/02_PShotManager/PShotManager.h"
#include "System/00_Manager/04_BossShotManager/BossShotManager.h"
#include "System/00_Manager/06_CollisionManager/CollisionManager.h" 
#include "GameObject//00_SkinMeshObject//Character//01_Player//Player.h" 

#include "System/02_Singleton/01_Camera/Camera.h"

GameMain::GameMain()
	: SceneBase()
	, m_pStcMeshObj(std::make_unique<StaticMeshObject>())

	, m_pGround(std::make_unique<Ground>())

	, m_pPlayer(std::make_shared<Player>())

	, m_pPortal(std::make_unique<Portal>())

	, m_pEnemyNomal(std::make_shared<Boss>(m_pPortal))

	, m_pCollisionManager(std::make_shared<CollisionManager>())

	, m_pHpBar(std::make_unique<HPBar>())

	, m_pBossHpBar(std::make_unique<BossHPBar>())

	, m_pSkyBox(std::make_unique<BackGround>())

	, m_pPointUI(std::make_unique<Point>())

	, m_pPortalGauge(std::make_shared<PortalGauge>())

	, m_pLimitTime(std::make_unique<LimitTime>())
	, m_Font(std::make_unique<TestFont>())

	, m_PortalFrame(std::make_unique<PortalFrame>())

	, m_PlayerRespawnTimer(0.0f)
	, m_BossRespawnTimer(0.0f)
{
	m_pDx11 = DirectX11::GetInstance();
	m_pDx9 = DirectX9::GetInstance();

	//カメラの位置を変更できるところ.
	m_Camera.vPosition = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	//カメラを見ているところを変更できるところ.
	m_Camera.vLook = D3DXVECTOR3(0.0f, 2.0f, 5.0f);

	//ライト情報
	m_Light.vDirection = D3DXVECTOR3(1.5f, 1.f, -1.f);		//ライト方向.
	Initialize();
}

GameMain::~GameMain()
{
	//外部で作成しているので、ここでは破棄しない.
	m_hWnd = nullptr;
	m_pDx11 = nullptr;
	m_pDx9 = nullptr;
}

void GameMain::Initialize()
{
	//カメラの位置を初期化.
	m_Camera.vPosition = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	m_Camera.vLook = D3DXVECTOR3(0.0f, 2.0f, 5.0f);
}

void GameMain::Create()
{
	//これでGameMainにあるPlayerの位置を取得してポータルの増加を作成している.
	m_pPortal->SetPlayer(m_pPlayer);
	//これでGameMainにあるEnemyNomalの位置を取得してポータルの増加を作成している.
	m_pPortal->SetEnemyNomal(m_pEnemyNomal);

	m_pPortal->SetPortalGauge(m_pPortalGauge);

	m_pHpBar->SetTargetPlayer(m_pPlayer.get());
	m_pHpBar->Create();

	m_pBossHpBar->SetTragetBoss(m_pEnemyNomal.get());
	m_pBossHpBar->Create();

	m_pPointUI->Create();
	m_pPortalGauge->Create();

	m_pLimitTime->Create();
	m_Font->Create();


	BossShotManager::GetInstance()->Init();
	PShotManager::GetInstance()->Init();

	//ポータルの初期化.
	m_pPortal->Init();

}

void GameMain::Update()
{

	if (SceneManager::GetInstance()->IsPause())
	{
		return;
	}

	//Timer更新
	Timer::GetInstance().Update();

	static POINT prevMouse;
	POINT currMouse;
	GetCursorPos(&currMouse);
	float dx = (float)(currMouse.x - (WND_W / 2));
	float dy = (float)(currMouse.y - (WND_H / 2));
	if (!SceneManager::GetInstance()->IsPause())
	{
		SetCursorPos(WND_W / 2, WND_H / 2);
	}

	if (!SceneManager::GetInstance()->IsPause())
	{
		Camera::GetInstance().Update(dx, dy, m_pPlayer->GetPosition());
	}

	{
		static ::EsHandle hEffect = -1;

		//エフェクト確認用のコード.
		if (GetAsyncKeyState('Y') & 0x0001) {
			hEffect = Effect::Play(Effect::LookPortal, D3DXVECTOR3(0.f, 1.f, 0.f));

			//拡大縮小
			Effect::SetScale(hEffect, D3DXVECTOR3(0.8f, 0.8f, 0.8f));

			//回転(Y軸回転)
			Effect::SetRotation(hEffect, D3DXVECTOR3(0.f, D3DXToRadian(90.0f), 0.f));

			//位置を再設定
			Effect::SetLocation(hEffect, D3DXVECTOR3(0.0f,5.0f,5.0f));
		}
		if (GetAsyncKeyState('T') & 0x0001) {
			Effect::Stop(hEffect);
		}
	}

	//地面.
	m_pGround->Update();
	m_pPlayer->Update();

	// リスポーン管理
	if (m_pPlayer->IsDead()) 
	{
		m_PlayerRespawnTimer += Timer::GetInstance().DeltaTime();

		//5秒経過したら復活
		if (m_PlayerRespawnTimer >= 5.0f)
		{
			m_pPlayer->Respawn();
			m_PlayerRespawnTimer = 0.0f;
		}
	}

	m_pEnemyNomal->SetTargetDead(m_pPlayer->IsDead());
	m_pEnemyNomal->Update();

	m_pEnemyNomal->SetTargetPos(m_pPlayer->GetPosition());

	if (m_pEnemyNomal->IsDaed())
	{
		// メンバ変数 m_BossRespawnTimer を使用（GameMain.h で定義してください）
		m_BossRespawnTimer += Timer::GetInstance().DeltaTime();

		// 5秒後に復活させる
		if (m_BossRespawnTimer >= 5.0f)
		{
			m_pEnemyNomal->Respawn();
			m_BossRespawnTimer = 0.0f; // タイマーリセット
		}
	}




	auto playerShotMgr = PShotManager::GetInstance();
	auto enemyShotMgr = BossShotManager::GetInstance();

	if (m_pPlayer && m_pPlayer->IsVisible())
	{
		m_pCollisionManager->SetPlayer(m_pPlayer); // shared_ptr を渡す
	}
	else
	{
		m_pCollisionManager->SetPlayer(nullptr);
	}

	if (m_pEnemyNomal && m_pEnemyNomal->IsVisible())
	{
		m_pCollisionManager->SetEnemies({ m_pEnemyNomal.get() }); // EnemyNomalManagerにGetEnemies()が必要です
	}
	else
	{
		m_pCollisionManager->SetEnemies({});
	}
	m_pCollisionManager->SetPlayerShots(playerShotMgr->GetShots()); // PShotManagerにGetShots()が必要です
	m_pCollisionManager->SetEnemyShots(enemyShotMgr->GetShots()); // EnemyNomalShotManagerにGetShots()が必要です

	m_pCollisionManager->Update();


	m_pHpBar->Update();
	m_pBossHpBar->Update();


	int totalPlayerScore = SceneManager::GetInstance()->GetPlayerScore();

	//累計スコアが1以上なら1つ目を点灯
	if (totalPlayerScore >= 1) {
		m_pPointUI->SetPlayerPointActive(0, true);
	}
	//累計スコアが2以上なら2つ目を点灯
	if (totalPlayerScore >= 2) {
		m_pPointUI->SetPlayerPointActive(1, true);
	}

	//敵のスコアも同様に反映させる場合
	int totalEnemyScore = SceneManager::GetInstance()->GetEnemyScore();

	//累計スコアが1以上なら1つ目を点灯
	if (totalEnemyScore >= 1) {
		m_pPointUI->SetBossPointActive(0, true);
	}
	//累計スコアが2以上なら2つ目を点灯
	if (totalEnemyScore >= 2) {
		m_pPointUI->SetBossPointActive(1, true);
	}


	m_pPointUI->Update();

	//ポータルの更新（ここで100%判定とシーン遷移が行われる）
	m_pPortal->Update();

	if (m_pPortal->IsReadyToLoad())
	{
		int nextID = m_pPortal->GetNextScene();

		SceneManager::GetInstance()->LoadScene(static_cast<SceneManager::List>(nextID));
		return;
	}

	//タイムアップ判定（ポータルが決着していない時のみ実行）
	if (!m_pPortal->IsTransitionStarted())
	{
		m_pLimitTime->Update();
		if (m_pLimitTime->IsTimeUp())
		{
			m_pPortal->ForceFinishByTimeUp();
		}
	}
}

void GameMain::Draw()
{
	Camera();
	Projection();

	PreDraw();

	if (m_pPlayer && m_pPlayer->GetShadow()) {
		m_pPlayer->GetShadow()->SetViewMatrix(m_mView);
		m_pPlayer->GetShadow()->SetProjMatrix(m_mProj);
	}

	if (m_pEnemyNomal && m_pEnemyNomal->GetShadow())
	{
		m_pEnemyNomal->GetShadow()->SetViewMatrix(m_mView);
		m_pEnemyNomal->GetShadow()->SetProjMatrix(m_mProj);
	}

	//背景の表示.
	//m_pSkyBox->Draw();

	//地面表示.
	m_pGround->Draw();
	if (m_pPlayer && m_pPlayer->IsVisible())
	{
		m_pPlayer->Draw();
	}
	if (m_pEnemyNomal && m_pEnemyNomal->IsVisible())
	{
		m_pEnemyNomal->Draw();
	}



	m_pPortal->Draw();

	PShotManager::GetInstance()->Draw(m_mView, m_mProj);
	BossShotManager::GetInstance()->Draw(m_mView, m_mProj);

#ifdef _DEBUG

	// バウンディングスフィアの描画（デバッグ用）
	m_pPlayer->GetBoundingSphere().Draw();

	m_pEnemyNomal->GetBoundingSphere().Draw();

	ImGui::Begin(JAPANESE("タイマー情報"));
	ImGui::Text(JAPANESE("デルタタイム: %.4f 秒"), Timer::GetInstance().DeltaTime());
	ImGui::Text(JAPANESE("総経過時間: %.2f 秒"), Timer::GetInstance().ElapsedTime());
	ImGui::End();
#endif
	//Effectクラス
	Effect::GetInstance()->Draw();
	m_PortalFrame->Draw();
	m_pHpBar->Draw();
	m_pBossHpBar->Draw();

	m_pPointUI->Draw();
	m_pPortalGauge->Draw();
	m_pLimitTime->Draw();
	m_Font->Draw();
}

HRESULT GameMain::LoadData()
{

	return S_OK;
}

void GameMain::Camera()
{
	m_mView = Camera::GetInstance().GetViewMatrix();
}

//プロジェクション関数.
void GameMain::Projection()
{
	//y方向の視野角。数値を大きくしたら視野が狭くなる.
	float fov_y = static_cast<FLOAT>(D3DXToRadian(45.0));	//ラジアン値.
	//アスペクト（幅÷高さ）.
	float aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);
	float near_z = 0.1f;
	float far_z = 999.9f;

	//プロジェクション（射影）変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,	//(out)プロジェクション計算結果.
		fov_y,		//視野角（FOV：Field of View）.
		aspect,		//アスペクト.
		near_z,		//近いビュー平面のz値.
		far_z);		//遠いビュー平面のz値.
}

