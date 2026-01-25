#pragma once

//ここも継承させているわけではないのでSceneBaseだけインクルードして
//他は前方宣言をしておく.
#include "Scene//SceneBase.h"
#include "GameObject//01_SpriteObject//SpriteObject.h"
#include "SceneManager//SceneManager.h"

#include "GameObject//02_StaticMeshObject//00_Ground//Ground.h"
#include "GameObject//00_SkinMeshObject//Character//01_Player//Player.h"

#include "GameObject/00_SkinMeshObject/Character//00_Boss/Boss.h"

#include "System/00_Manager/02_PShotManager/PShotManager.h"
#include "System/00_Manager/04_BossShotManager/BossShotManager.h"

#include "System/00_Manager/06_CollisionManager/CollisionManager.h"

#include "Sprite3D//Sprite3D.h"

#include "GameObject/02_StaticMeshObject/01_Portal/Portal.h"

#include "GameObject//03_UIObject//00_HPBar//HPBar.h"

#include "GameObject/03_UIObject/02_BossHPBar/BossHPBar.h"

#include "GameObject/02_StaticMeshObject/03_BackGround/BackGround.h"

#include "GameObject/03_UIObject/01_Point/Point.h"

#include "GameObject/03_UIObject/03_PortalGauge/PortalGauge.h"

#include "GameObject/03_UIObject/04_LimitTime/LimitTime.h"

#include "GameObject/03_UIObject/05_TestFont/TestFont.h"

#include "GameObject/02_StaticMeshObject/04_PortalFrame/PortalFrame.h"

class Timer;



/*********************************************
*	ゲームメイン画像クラス.
**/

class GameMain
	: public SceneBase
{
public:
	GameMain();
	~GameMain() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

	HRESULT LoadData();

	//カメラ関数.
	void Camera();
	//プロジェクション関数.
	void Projection();

private:
	//ゲームで扱うスプライトデータ(使いまわす資源).
	std::unique_ptr<Sprite3D> m_pSp3D;

	//スタティックメッシュオブジェクトクラス.
	std::unique_ptr<StaticMeshObject> m_pStcMeshObj;

	//地面クラス.
	std::unique_ptr<Ground>	m_pGround;

	//プレイヤークラス.
	std::shared_ptr<Player> m_pPlayer;

	std::shared_ptr<Portal> m_pPortal;

	//ボスをポータルの後に宣言する
	std::shared_ptr<Boss> m_pEnemyNomal;

	std::shared_ptr<CollisionManager> m_pCollisionManager;


	//HPBarクラス(Test).
	std::unique_ptr<HPBar> m_pHpBar;
	std::unique_ptr<BossHPBar> m_pBossHpBar;

	//背景クラス.
	std::unique_ptr<BackGround> m_pSkyBox;

	std::unique_ptr<Point> m_pPointUI;

	std::shared_ptr<PortalGauge> m_pPortalGauge;

	std::unique_ptr<LimitTime> m_pLimitTime;

	std::unique_ptr<TestFont> m_Font;

	std::unique_ptr<PortalFrame> m_PortalFrame;

	//プレイヤーとボスのリスポーン時間の設定.
	float m_PlayerRespawnTimer;
	float m_BossRespawnTimer;
};