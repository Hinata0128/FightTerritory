#include "SceneManager.h"
#include "Sound/SoundManager.h"

#include "..//System/00_Manager/03_ImGuiManager/ImGuiManager.h"

SceneManager::SceneManager()
	: m_pScene			( nullptr )
	, m_hWnd			()
	, m_pDx11			()
	, m_pDx9			()

	, m_pPortal			( nullptr )
	, m_PlayerPoint		( 0 )
	, m_EnemyNomalPoint	( 0 )
{
}

SceneManager::~SceneManager()
{
}

HRESULT SceneManager::Create(HWND hWnd)
{
	m_hWnd = hWnd;
	LoadScene(List::OP);
	return S_OK;
}

void SceneManager::Update()
{

#ifdef _DEBUG
	ImGui::Begin("Game Score");

	ImGui::Text("Player Score : %d", m_PlayerPoint);
	ImGui::Text("Enemy  Score : %d", m_EnemyNomalPoint);

	//ポータルが存在していれば、進行状況も確認可能
	if (m_pPortal)
	{
		ImGui::Separator();
		ImGui::Text("Portal Progress : %d %%", m_pPortal->GetPortalPercent());
	}

	ImGui::End();
#endif

#ifdef _DEBUG
	//それぞれのシーンへの移動ができる
	ImGui::Begin("Scene Debug");

	ImGui::Text("Current Scene Control");
	ImGui::Separator();

	if (ImGui::Button("Title"))
	{
		LoadScene(SceneManager::OP);
	}

	if (ImGui::Button("Game Main"))
	{
		LoadScene(SceneManager::Main);
	}

	if (ImGui::Button("First Round"))
	{
		LoadScene(SceneManager::First);
	}

	if (ImGui::Button("Second Round"))
	{
		LoadScene(SceneManager::Second);
	}
	
	if (ImGui::Button("Final Round"))
	{
		LoadScene(SceneManager::Final);
	}

	if (ImGui::Button("Win"))
	{
		LoadScene(SceneManager::Win);
	}

	if (ImGui::Button("Lose"))
	{
		LoadScene(SceneManager::Lose);
	}

	ImGui::Separator();
	//ポイントのUIの確認のため.
	if (ImGui::Button("Player +1")) m_PlayerPoint++;
	if (ImGui::Button("Enemy +1"))  m_EnemyNomalPoint++;
	ImGui::End();
#endif
	if (m_IsPause)
	{
		return;
	}
	m_pScene->Update();

}

void SceneManager::Draw()
{
	m_pScene->Draw();
}

void SceneManager::LoadScene(List Scene)
{
	m_pScene.reset();

	//シーン作成.
	MakeScene(Scene);
	m_pScene->Create();
}

HWND SceneManager::GetHWND() const
{
	return m_hWnd;
}

void SceneManager::SetPause(bool pause)
{
	m_IsPause = pause;
}

bool SceneManager::IsPause() const
{
	return m_IsPause;
}

void SceneManager::SetPortal(Portal* portal)
{
	m_pPortal = portal;
}

Portal* SceneManager::GetPortal() const
{
	return m_pPortal;
}

//Playerのポータル取得ポイント関数.
void SceneManager::AddPlayerScore()
{
	m_PlayerPoint++;


	ResetRound();
}

//EnemyNomalのポータル取得ポイント関数.
void SceneManager::AddEnemyScore()
{
	m_EnemyNomalPoint++;


	ResetRound();
}

int SceneManager::GetPlayerScore() const
{
	return m_PlayerPoint;
}

int SceneManager::GetEnemyScore() const
{
	return m_EnemyNomalPoint;
}

void SceneManager::ResetRound()
{
	auto portal = SceneManager::GetInstance()->GetPortal();
	if (portal)
	{
		portal->Init();
	}
}

bool SceneManager::IsGameFinished() const
{
	return (m_PlayerPoint >= 2 || m_EnemyNomalPoint >= 2);

}
void SceneManager::ResetScore()
{
	m_PlayerPoint = 0;
	m_EnemyNomalPoint = 0;
}

//シーン作成.
void SceneManager::MakeScene(List Scene)
{
	switch (Scene)
	{
	case SceneManager::OP:
		SoundManager::GetInstance()->PlayLoop(SoundManager::BGM_Title);
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Ending);
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Over);
		m_pScene = std::make_unique<Title>();
		break;
	case SceneManager::First:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Title);
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Ending);
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Over);
		SoundManager::GetInstance()->PlaySE(SoundManager::SE_Round);
		m_pScene = std::make_unique<FirstRound>();
		break;
	case SceneManager::Second:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Main);
		SoundManager::GetInstance()->PlaySE(SoundManager::SE_Round);
		m_pScene = std::make_unique<SecondRound>();
		break;
	case SceneManager::Final:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Main);
		SoundManager::GetInstance()->PlaySE(SoundManager::SE_Round);
		m_pScene = std::make_unique<FinalRound>();
		break;
	case SceneManager::Main:
		SoundManager::GetInstance()->PlayLoop(SoundManager::BGM_Main);
		m_pScene = std::make_unique<GameMain>();
		break;
	case SceneManager::Lose:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Main);
		SoundManager::GetInstance()->PlayLoop(SoundManager::BGM_Over);
		m_pScene = std::make_unique<GameOver>();
		break;
	case SceneManager::Win:
		SoundManager::GetInstance()->Stop(SoundManager::BGM_Main);
		SoundManager::GetInstance()->PlayLoop(SoundManager::BGM_Ending);
		m_pScene = std::make_unique<Ending>();
		break;
	default:
		break;
	}
}
