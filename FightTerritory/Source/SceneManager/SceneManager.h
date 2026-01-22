#pragma once
#include "Scene/SceneBase.h"
#include "Scene/00_Title/Title.h"
#include "Scene/01_FirstRound/FirstRound.h"
#include "Scene/02_SecondRound/SecondRound.h"
#include "Scene/03_FinalRound/FinalRound.h"
#include "Scene/04_GameMain/GameMain.h"
#include "Scene/06_GameOver/GameOver.h"
#include "Scene/05_Ending/Ending.h"

#include "GameObject/02_StaticMeshObject/01_Portal/Portal.h"


/********************************************
*	シーンマネージャークラス.
**/

class SceneManager
{
public:
	//シーンリストの列挙型.
	enum List
	{
		OP,		//タイトル.
		First,	//第一ラウンド開始画像.
		Second,	//第二ラウンド開始画像.
		Final,	//第三ラウンド開始画像.
		Main,	//メイン.
		Lose,	//敗北.
		Win,	//勝利.

		max,	//何も書かない.
	};

public:
	//シングルトンパターン
	static SceneManager* GetInstance()
	{
		static SceneManager s_Instance;
		return &s_Instance;
	}
	~SceneManager();

	HRESULT Create(HWND hWnd);

	void Update();
	void Draw();

	//シーン読み込み.
	void LoadScene(List Scene);

	//HWNDを取得.
	HWND GetHWND() const;

	void SetPause(bool pause);
	bool IsPause() const;
public:
	//============================================================================
	// ポータル周りの関数.
	// PortalのGetSet関数.
	// SetPortalでは、Portal = portalで代入させています.
	// GetPortalでは、return Portalで返しています.
	// PlayerとEnemyNomalの今の取得ポイント関数.
	// PlayerPointとEnemyNomalPointのGet関数(pointをreturnで返す).
	//============================================================================
	
	void SetPortal(Portal* portal);
	Portal* GetPortal() const;

	//Playerのポータル取得ポイント関数.
	void AddPlayerScore();
	//EnemyNomalのポータル取得ポイント関数.
	void AddEnemyScore();

	//PlayerとEnemyNomalのGet関数.
	int GetPlayerScore() const;
	int GetEnemyScore() const;
	//2点とった時に入る関数.
	void ResetRound();
	//試合が終了したかの確認.
	bool IsGameFinished() const;
	//PlayerとenemyNomalのポイントを初期化する.
	void ResetScore();



	//遷移の準備ができたか.
	bool IsReadyToLoad() const { return m_IsReadyToLoad; }
	//次のシーンを取得.
	SceneManager::List GetNextScene() const { return m_NextScene; }


public:
	void SetDx11(DirectX11* Dx11) { m_pDx11 = Dx11; }
	void SetDx9(DirectX9* pDx9) { m_pDx9 = pDx9; }
private:
	//シーン作成.
	void MakeScene(List Scene);
private:
	SceneManager();
	SceneManager(const SceneManager& rhs) = delete;
	SceneManager& operator = (const SceneManager& rhs) = delete;

private:
	std::unique_ptr<SceneBase> m_pScene;
	HWND m_hWnd;

	DirectX11* m_pDx11;
	DirectX9*	m_pDx9;

	Portal* m_pPortal;

	int m_PlayerPoint = 0;
	int m_EnemyNomalPoint = 0;

	bool m_IsReadyToLoad = false;
	SceneManager::List m_NextScene = SceneManager::max;

	bool m_IsPause = false;
};
