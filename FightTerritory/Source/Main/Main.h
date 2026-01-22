#pragma once
#include <Windows.h>

//クラスの前方宣言.
class DirectX9;
class DirectX11;
class Game;
class SceneManager;
class StaticMeshManager;
class SkinMeshManager;
class ImGuiManager;

/**************************************************
*	メインクラス.
**/
class Main
{
public:
	Main();	//コンストラクタ.
	~Main();	//デストラクタ.

	void Update();		//更新処理.
	void Draw();		//描画関数.
	HRESULT Create();	//構築処理.
	HRESULT LoadData();	//データロード処理.
	void Release();		//解放処理.
	void Loop();		//メインループ.

	//ウィンドウ初期化関数.
	HRESULT InitWindow(
		HINSTANCE hInstance,
		INT x, INT y,
		INT width, INT height );

private:
	//ウィンドウ関数（メッセージ毎の処理）.
	static LRESULT CALLBACK MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam );

	//終了判定関数.
	void IsExitGame();

private:
	HWND			m_hWnd;	//ウィンドウハンドル.

	//前回Escを押した時間.
	float			m_LastEscTime;
	//ImGui調整モードフラグ.
	bool			m_IsPause;

};