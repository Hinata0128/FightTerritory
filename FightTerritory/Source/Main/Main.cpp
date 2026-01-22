#include "Main.h"
#include "DirectX\\DirectX9.h"
#include "DirectX\\DirectX11.h"
#include "SceneManager/SceneManager.h"
#include "System/00_Manager/01_StaticMeshManager/StaticMeshManager.h"
#include "System/00_Manager/00_SkinMeshManager/SkinMeshManager.h"
#include "Effect/Effect.h"
#include "Sound/SoundManager.h"

#include "System/00_Manager/03_ImGuiManager/ImGuiManager.h"
//ImGuiメッセージハンドラ.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "System/02_Singleton/00_Timer/Timer.h"

//=================================================
//	定数.
//=================================================
const TCHAR WND_TITLE[] = _T("FightTerritory");
const TCHAR APP_NAME[] = _T("FightTerritory");


//=================================================
//	コンストラクタ.
//=================================================
Main::Main()
//初期化リスト.
	: m_hWnd			( nullptr )

	, m_LastEscTime		( 0.0f )
	, m_IsPause			( false )
{
	AllocConsole(); //コマンドプロンプト表示.
}


//=================================================
//	デストラクタ.
//=================================================
Main::~Main()
{
	DeleteObject(m_hWnd);
}


//=================================================
//	構築処理.
//=================================================
HRESULT Main::Create()
{
	auto pDx9 = DirectX9::GetInstance();
	auto pDx11 = DirectX11::GetInstance();

	if (FAILED(pDx9->Create(m_hWnd)))
		return E_FAIL;
	if (FAILED(pDx11->Create(m_hWnd)))
		return E_FAIL;

	StaticMeshManager::GetInstance()->Create();
	SkinMeshManager::GetInstance()->Create();

	SceneManager::GetInstance()->SetDx9(pDx9);
	SceneManager::GetInstance()->SetDx11(pDx11);

	Effect::GetInstance()->Create(pDx11->GetDevice(), pDx11->GetContext());

	// ImGui 初期化
	if (FAILED(ImGuiManager::GetInstance()->Init(m_hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=================================================
//	更新処理.
//=================================================
void Main::Update()
{
	ImGuiManager::GetInstance()->NewFrameSetting();

	SceneManager::GetInstance()->Update();
	Timer::GetInstance().Update();

	//終了判定.
	IsExitGame();
}

//=================================================
//	描画処理.
//=================================================
void Main::Draw()
{
	auto pDx11 = DirectX11::GetInstance();

	//バックバッファをクリア
	pDx11->ClearBackBuffer();

	SceneManager::GetInstance()->Draw();

	ImGuiManager::GetInstance()->Render();

	//画面に表示.
	pDx11->Present();
}


//=================================================
//	データロード処理.
//=================================================
HRESULT Main::LoadData()
{
	if (SoundManager::GetInstance()->Load(m_hWnd) == false) {
		return E_FAIL;
	}

	if (FAILED(Effect::GetInstance()->LoadData()))
	{
		return E_FAIL;
	}

	SceneManager::GetInstance()->Create(m_hWnd);

	return S_OK;
}


//=================================================
//	解放処理.
//=================================================
void Main::Release()
{
	DirectX11::GetInstance()->Release();
	DirectX9::GetInstance()->Release();
}


//=================================================
//	メッセージループ.
//=================================================
void Main::Loop()
{
	if (FAILED(LoadData())) {
		return;
	}

	//------------------------------------------------
	//	フレームレート調整準備.
	//------------------------------------------------
	float Rate = 0.0f;
	DWORD sync_old = timeGetTime();
	DWORD sync_now;

	timeBeginPeriod(1);
	//FPS外部設定(Global).
	Rate = 1000.0f / static_cast<float>(FPS);

	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		sync_now = timeGetTime();

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (sync_now - sync_old >= Rate)
		{
			sync_old = sync_now;

			// 更新と描画を分離して呼び出し
			Update();
			Draw();
		}
	}

	timeEndPeriod(1); // timeBeginPeriodと対で呼ぶ
	Release();
}

//=================================================
//	ウィンドウ初期化関数.
//=================================================
HRESULT Main::InitWindow(
	HINSTANCE hInstance,
	INT x, INT y,
	INT width, INT height)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MsgProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = APP_NAME;
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		_ASSERT_EXPR(false, _T("ウィンドウクラスの登録に失敗"));
		return E_FAIL;
	}

	RECT	rect = { 0, 0, width, height };
	DWORD	dwStyle = WS_OVERLAPPEDWINDOW;

	if (AdjustWindowRect(&rect, dwStyle, FALSE) == 0)
	{
		MessageBox(nullptr, _T("ウィンドウ領域の調整に失敗"), _T("エラーメッセージ"), MB_OK);
		return E_FAIL;
	}

	INT winWidth = rect.right - rect.left;
	INT winHeight = rect.bottom - rect.top;

	//左上に表示.
	m_hWnd = CreateWindow(
		APP_NAME, WND_TITLE, dwStyle,
		0, 0,
		winWidth, winHeight,
		nullptr, nullptr, hInstance, nullptr);

	if (!m_hWnd) {
		_ASSERT_EXPR(false, _T("ウィンドウ作成失敗"));
		return E_FAIL;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}


//=================================================
//	ウィンドウ関数（メッセージ毎の処理）.
//=================================================
LRESULT CALLBACK Main::MsgProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return 1; 
	}

	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//Escを押した際の処理.
void Main::IsExitGame()
{
	//ToDo : ゲーム終了を実行する.
	//		 一回目はポーズ状態でImGuiを調整する.
	//		 ダブルタップをした際に終了メッセージを表示させている.
	static bool isPushed = false;

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		if (!isPushed)
		{
			float currentTime = Timer::GetInstance().ElapsedTime();
			float diff = currentTime - m_LastEscTime;

			//ダブルタップ.
			if (diff < 0.3f)
			{
				if (MessageBox(m_hWnd,
					_T("ゲームを終了しますか？"),
					_T("終了確認"),
					MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
					DestroyWindow(m_hWnd);
				}
				m_LastEscTime = 0.0f;
			}
			else
			{
				m_LastEscTime = currentTime;
				m_IsPause = !m_IsPause;

				SceneManager::GetInstance()->SetPause(m_IsPause);

				//マウス解放／ロック切り替え
				ShowCursor(m_IsPause);

				if (m_IsPause)
				{
					ClipCursor(nullptr); //マウス自由
				}
				//マウスをゲーム内に表示させておく.
				else
				{
					RECT rc;
					GetClientRect(m_hWnd, &rc);
					POINT ul = { rc.left, rc.top };
					POINT lr = { rc.right, rc.bottom };
					ClientToScreen(m_hWnd, &ul);
					ClientToScreen(m_hWnd, &lr);
					rc = { ul.x, ul.y, lr.x, lr.y };
					ClipCursor(&rc); //ウィンドウ内固定
				}
			}
			isPushed = true;
		}
	}
	else
	{
		isPushed = false;
	}
}