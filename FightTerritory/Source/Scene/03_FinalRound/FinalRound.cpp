#include "FinalRound.h"

#include "SceneManager//SceneManager.h"
#include "System//02_Singleton//00_Timer//Timer.h"
#include "GameObject//03_UIObject//UIObject.h"
#include "Sprite2D//Sprite2D.h"

FinalRound::FinalRound()
	: SceneBase				()
	, m_pSpriteFinalRound	( std::make_shared<Sprite2D>() )
	, m_upFinalRound		( std::make_shared<UIObject>() )
{
}

FinalRound::~FinalRound()
{
}

void FinalRound::Initialize()
{
}

void FinalRound::Create()
{
	//画像サイズのローカル変数.
	const float WND_W = 1280.0f;
	const float WMD_H = 720.0f;

	//FinalRound構造体.
	Sprite2D::SPRITE_STATE SSFinal =
	{ WND_W, WMD_H, WND_W, WMD_H, WND_W , WMD_H };
	
	//FinalRound読み込み.
	m_pSpriteFinalRound->Init(_T("Data\\Texture\\FinalRoundImg.png"), SSFinal);
	//画像の設定.
	m_upFinalRound->AttachSprite(m_pSpriteFinalRound);
	//表示位置.
	m_upFinalRound->SetPosition(0.0f, 0.0f, 0.0f);
}

void FinalRound::Update()
{
	float deltaTime = Timer::GetInstance().DeltaTime();

	m_Timer += deltaTime;

	if (m_Timer >= 0.5f)
	{
		SceneManager::GetInstance()->LoadScene(SceneManager::Main);
		return;
	}

}

void FinalRound::Draw()
{
	m_upFinalRound->Draw();
}
