#include "GameOver.h"

#include "SceneManager//SceneManager.h"
#include "GameObject//03_UIObject//UIObject.h"
#include "Sprite2D//Sprite2D.h"

#include "DirectX//DirectX11.h"

#include "System//02_Singleton//00_Timer//Timer.h"

#include "Sound//SoundManager.h"

GameOver::GameOver()
	: SceneBase				()

	, m_Select				( SelectMenu::Continue )
	, m_State				( LoseState::Select )

	, m_ContinuePos			( 880.0f, 520.0f, 0.0f )
	, m_EndPos				( 880.0f, 620.0f, 0.0f )
	, m_EndSelectPos		( 920.0f, 620.0f, 0.0f )

	, m_pSpriteBack			( std::make_shared<Sprite2D>() )
	, m_upBack				( std::make_shared<UIObject>() )

	, m_pSpriteDefeat		( std::make_shared<Sprite2D>() )
	, m_upDefeat			( std::make_shared<UIObject>() )

	, m_pSpriteContinue		( std::make_shared<Sprite2D>() )
	, m_upContinue			( std::make_shared<UIObject>() )

	, m_pSpriteEnd			( std::make_shared<Sprite2D>() )
	, m_upEnd				( std::make_shared<UIObject>() )

	, m_pSpriteFade			( std::make_shared<Sprite2D>() )
	, m_upFade				( std::make_shared<UIObject>() )

	, m_pSpriteSelectBack	( std::make_shared<Sprite2D>() )
	, m_upSelectBack		( std::make_shared<UIObject>() )

	, m_pSpriteSelectFrame	( std::make_shared<Sprite2D>() )
	, m_upSelectFrame		( std::make_shared<UIObject>() )
{
}

GameOver::~GameOver()
{
}

void GameOver::Initialize()
{
}

void GameOver::Create()
{
	//背景画像サイズのローカル変数.
	const float WND_W = 1280.0f;
	const float WMD_H = 720.0f;

	//Lose画像のサイズのローカル変数.
	const float Defeat_W = 430.0f;
	const float Defeat_H = 210.0f;

	//Continue画像のサイズのローカル変数.
	const float Continue_W = 194.0f;
	const float Continue_H = 45.0f;

	//End画像のサイズのローカル変数.
	const float End_W = 84.0f;
	const float End_H = 45.0f;

	//選択肢・枠のサイズのローカル変数.
	const float Select_W = 320.0f;
	const float Select_H = 80.0f;

	//背景・フェード構造体.
	Sprite2D::SPRITE_STATE SSBack =
	{
		WND_W, WMD_H, WND_W, WMD_H, WND_W, WMD_H
	};
	//背景画面の読み込み.
	m_pSpriteBack->Init(_T("Data\\Image\\Setting\\Surface.png"), SSBack);
	//画像の設定.
	m_upBack->AttachSprite(m_pSpriteBack);
	//表示位置.
	m_upBack->SetPosition(0.0f, 0.0f, 0.0f);

	//Lose構造体.
	Sprite2D::SPRITE_STATE SSLose =
	{
		Defeat_W, Defeat_H, Defeat_W, Defeat_H, Defeat_W, Defeat_H
	};
	//Lose画像の読み込み.
	m_pSpriteDefeat->Init(_T("Data\\Image\\Setting\\Defeat.png"), SSLose);
	//画像の設定.
	m_upDefeat->AttachSprite(m_pSpriteDefeat);
	//表示位置.
	m_upDefeat->SetPosition(150.0f, 100.0f, 0.0f);

	//Continue構造体.
	Sprite2D::SPRITE_STATE SSContinue =
	{
		Continue_W, Continue_H, Continue_W, Continue_H, Continue_W, Continue_H
	};
	//Continue画像の読み込み.
	m_pSpriteContinue->Init(_T("Data\\Image\\Setting\\S_Continue.png"), SSContinue);
	//画像の設定.
	m_upContinue->AttachSprite(m_pSpriteContinue);

	//End構造体.
	Sprite2D::SPRITE_STATE SSEnd =
	{
		End_W, End_H, End_W, End_H, End_W, End_H,
	};
	//End画像の読み込み.
	m_pSpriteEnd->Init(_T("Data\\Image\\Setting\\S_End.png"), SSEnd);
	//画像の設定.
	m_upEnd->AttachSprite(m_pSpriteEnd);

	//フェード用の黒画像
	m_pSpriteFade->Init(_T("Data\\Image\\Setting\\Black.png"), SSBack);
	m_upFade->AttachSprite(m_pSpriteFade);
	m_upFade->SetPosition({ 0.0f, 0.0f, 0.0f });
	m_upFade->SetAlpha(0.0f);


	//選択肢・枠構造体.
	Sprite2D::SPRITE_STATE SSSelect =
	{
		Select_W, Select_H, Select_W, Select_H, Select_W, Select_H
	};
	//選択肢の読み込み.
	m_pSpriteSelectBack->Init(_T("Data\\Image\\Setting\\SelectBack.png"), SSSelect);
	//画像の設定.
	m_upSelectBack->AttachSprite(m_pSpriteSelectBack);

	//枠の読み込み.
	m_pSpriteSelectFrame->Init(_T("Data\\Image\\Setting\\SelectFrame.png"), SSSelect);
	//画像の設定.
	m_upSelectFrame->AttachSprite(m_pSpriteSelectFrame);
}

void GameOver::Update()
{
	switch (m_State)
	{
	case GameOver::LoseState::Select:
		UpdateSelect();
		break;
	case GameOver::LoseState::FadeOut:
		UpdateFadeOut();
		break;
	case GameOver::LoseState::First:
		SceneManager::GetInstance()->LoadScene(SceneManager::First);
		break;
	default:
		break;
	}
}

void GameOver::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);
	DirectX11::GetInstance()->SetAlphaBlend(true);
	//描画順番調整.
	m_upBack->Draw();

	D3DXVECTOR3 currentSelectPos = (m_Select == SelectMenu::Continue) ? m_ContinuePos : m_EndPos;

	D3DXVECTOR3 backPos = currentSelectPos;
	backPos.x -= 60.0f; //背景を左にずらして、文字を背景の中央に合わせる
	backPos.y -= 15.0f; //背景を少し上にずらして上下の中央を合わせる



	m_upDefeat->Draw();

	//青い背景
	m_upSelectBack->SetPosition(backPos);
	m_upSelectBack->Draw();

	//選択枠も同じ位置に
	m_upSelectFrame->SetPosition(backPos);
	m_upSelectFrame->Draw();

	m_upContinue->Draw();
	//表示位置.
	m_upContinue->SetPosition(m_ContinuePos);

	m_upEnd->Draw();

	//表示位置.
	m_upEnd->SetPosition(m_EndSelectPos);

	if (m_FadeAlpha > 0.0f)
	{
		m_upFade->SetAlpha(m_FadeAlpha);
		m_upFade->Draw();
	}

	DirectX11::GetInstance()->SetAlphaBlend(false);
	DirectX11::GetInstance()->SetDepth(true);
}

void GameOver::UpdateSelect()
{
	m_InputTimer += Timer::GetInstance().DeltaTime();

	SelectMenu oldSelect = m_Select;

	if (GetAsyncKeyState(VK_UP) & 0x0001)
	{
		if (m_InputTimer >= 0.2f)
		{
			m_Select = SelectMenu::Continue;
		}
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x0001)
	{
		if (m_InputTimer >= 0.2f)
		{
			m_Select = SelectMenu::End;
		}
	}

	if (m_Select != oldSelect)
	{
		SoundManager::GetInstance()->PlaySE(SoundManager::SE_Select);

		//選択が変わったのでタイマーを初期化.
		m_InputTimer = 0.0f;
	}

	if (m_Select == SelectMenu::Continue)
	{
		m_ContinuePos;
		m_EndPos;
	}
	else
	{
		m_ContinuePos;
		m_EndPos;
	}
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		if (m_InputTimer >= 0.2f)
		{
			SoundManager::GetInstance()->PlaySE(SoundManager::SE_Enter);
			if (m_Select == SelectMenu::Continue)
			{
				m_State = LoseState::FadeOut;
				m_FadeAlpha = 0.0f;
			}
			else
			{
				//タイトルへ遷移.
				SceneManager::GetInstance()->LoadScene(SceneManager::OP);
			}
			m_InputTimer = 0.0f;
		}
	}
}

void GameOver::UpdateFadeOut()
{
	m_FadeAlpha += m_FadeSpeed * Timer::GetInstance().DeltaTime();

	if (m_FadeAlpha >= 1.0f)
	{
		m_State = LoseState::First;
	}
}
