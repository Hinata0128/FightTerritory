#include "FirstRound.h"

#include "SceneManager//SceneManager.h"
#include "System//02_Singleton//00_Timer//Timer.h"
#include "GameObject//03_UIObject//UIObject.h"
#include "Sprite2D//Sprite2D.h"


FirstRound::FirstRound()
	: SceneBase			()
	, m_pSpriteFirstRound(std::make_shared<Sprite2D>())
	, m_upFirstRound(std::make_shared<UIObject>())
{
}

FirstRound::~FirstRound()
{
}

void FirstRound::Initialize()
{
}

void FirstRound::Create()
{
	//画像サイズのローカル変数.
	const float WND_W = 1280.0f;
	const float WMD_H = 720.0f;

	//FirstRound構造体.
	Sprite2D::SPRITE_STATE SSFirst =
	{ WND_W, WMD_H, WND_W, WMD_H, WND_W , WMD_H };

	//FirstRound読み込み.
	m_pSpriteFirstRound->Init(_T("Data\\Texture\\FirstRoundImg.png"), SSFirst);
	//画像を設定.
	m_upFirstRound->AttachSprite(m_pSpriteFirstRound);
	//表示位置.
	m_upFirstRound->SetPosition(D3DXVECTOR3(0, 0, 0));
}

void FirstRound::Update()
{
	float deltaTime = Timer::GetInstance().DeltaTime();

	m_Timer += deltaTime;

	if (m_Timer >= 0.5f)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::Main);
		return;
	}

}

void FirstRound::Draw()
{
	m_upFirstRound->Draw();
}
