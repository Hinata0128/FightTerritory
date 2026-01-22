#pragma once
#include "StaticMash/StaticMesh.h"
#include "DirectX/DirectX11.h"
#include "DirectX/DirectX9.h"
#include "Sprite2D/Sprite2D.h"
#include "GameObject/03_UIObject/UIObject.h"


/**********************************************************
*	基底クラス.
**/

class SceneBase	
{
public:
	SceneBase();
	virtual ~SceneBase() {}

	virtual void Initialize() = 0;
	virtual void Create() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//メッシュを接続する.
	void AttachMesh(StaticMesh& pMesh) {
		m_pMesh = &pMesh;
	}

	void PreDraw();

protected:
	StaticMesh* m_pMesh;
	DirectX9* m_pDx9;
	DirectX11* m_pDx11;

	//ウィンドウハンドル.
	HWND				m_hWnd;

	//カメラ情報.
	CAMERA				m_Camera;
	//ライト情報
	LIGHT				m_Light;

	//行列.
	D3DXMATRIX			m_mView;	//ビュー(カメラ)行列.
	D3DXMATRIX			m_mProj;	//射影（プロジェクション）行列.
	
	
	//スコア.
	int m_Score;
	//カウント.
	int m_Count;
	//時間経過でシーンを遷移する.
	//タイマーのメンバ変数.
	float m_Timer;

	//フェードで使用する変数.
	float m_InputTimer;
	float m_FadeAlpha;
	float m_FadeSpeed;

};