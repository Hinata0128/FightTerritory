#include "SecondRound.h"

#include "SceneManager//SceneManager.h"
#include "System//02_Singleton//00_Timer//Timer.h"
#include "GameObject//03_UIObject//UIObject.h"
#include "Sprite2D//Sprite2D.h"

SecondRound::SecondRound()
	: SceneBase				()
	, m_pSpriteSecondRound	( std::make_shared<Sprite2D>() )
	, m_upSecondRound		( std::make_shared<UIObject>() )
{
}

SecondRound::~SecondRound()
{
}

void SecondRound::Initialize()
{
}

void SecondRound::Create()
{
	//画像サイズのローカル変数.
	const float WND_W = 1280.0f;
	const float WMD_H = 720.0f;

	//SecondRound構造体.
	Sprite2D::SPRITE_STATE SSSecomnd =
	{ WND_W, WMD_H, WND_W, WMD_H, WND_W , WMD_H };

	//SecondRound読み込み.
	m_pSpriteSecondRound->Init(_T("Data\\Texture\\SecondRoundImg.png"), SSSecomnd);
	//画像の設定.
	m_upSecondRound->AttachSprite(m_pSpriteSecondRound);
	//表示位置.
	m_upSecondRound->SetPosition(0.0f, 0.0f, 0.0f);
}

void SecondRound::Update()
{
	float deltaTime = Timer::GetInstance().DeltaTime();

	m_Timer += deltaTime;

	if (m_Timer >= 0.5f)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::Main);
		return;
	}
}

void SecondRound::Draw()
{
	m_upSecondRound->Draw();
}
