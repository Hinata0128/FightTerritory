#include "Title.h"

#include "SceneManager/SceneManager.h"
#include "GameObject/03_UIObject/UIObject.h"
#include "Sprite2D/Sprite2D.h"
#include "DirectX/DirectX11.h"
#include "System/02_Singleton/00_Timer/Timer.h"

#include "Sound/SoundManager.h"


Title::Title()
    : m_Select              ( SelectMenu::Start )
    , m_State               ( TitleState::Select )
    , m_StartPos            ( 550.f, 550.f, 0.f )
    , m_EndPos              ( 550.f, 650.f, 0.f )

    , m_pSpriteBack         ( std::make_shared<Sprite2D>() )
    , m_upBack              ( std::make_shared<UIObject>() )

    , m_pSpriteTitle        ( std::make_shared<Sprite2D>() )
    , m_upTitle             ( std::make_shared<UIObject>() )

    ,m_pSpriteStart         ( std::make_shared<Sprite2D>() )
    ,m_upStart              ( std::make_shared<UIObject>() )

    ,m_pSpriteEnd           ( std::make_shared<Sprite2D>() )
    ,m_upEnd                ( std::make_shared<UIObject>() )

    ,m_pSpriteFade          ( std::make_shared<Sprite2D>() )
    ,m_upFade               ( std::make_shared<UIObject>() )

    ,m_pSpriteSelectBack    ( std::make_shared<Sprite2D>() )
    ,m_upSelectBack         ( std::make_shared<UIObject>() )

    ,m_pSpriteSelectFrame   ( std::make_shared<Sprite2D>() )
    ,m_upSelectFrame        ( std::make_shared<UIObject>() )

{
}

Title::~Title()
{
}

void Title::Initialize()
{
}

void Title::Create()
{
    //背景・フェード画像のサイズ.
    const float WND_W = 1280.0f;
    const float WND_H = 720.0f;

    //Lose画像のサイズのローカル変数.
    const float Defeat_W = 430.0f;
    const float Defeat_H = 210.0f;

    //スタート画像のサイズ.
    const float Start_W = 135.0f;
    const float Staet_H = 45.0f;

    //エンド画像のサイズ.
    const float End_W = 84.0f;
    const float End_H = 45.0f;

    //背景画像の設定
    Sprite2D::SPRITE_STATE ssTitle{ WND_W, WND_H, WND_W, WND_H, WND_W, WND_H };
    //背景画像の読み込み.
    m_pSpriteBack->Init(_T("Data\\Image\\Setting\\Surface.png"), ssTitle);
    m_upBack->AttachSprite(m_pSpriteBack);
    m_upBack->SetPosition( 0.0f, 0.0f, 0.0f );

    //タイトル画像の読み込み.
    Sprite2D::SPRITE_STATE SSTitle =
    {
        Defeat_W, Defeat_H, Defeat_W, Defeat_H, Defeat_W, Defeat_H
    };
    m_pSpriteTitle->Init(_T("Data\\Image\\Setting\\Title.png"), SSTitle);
    m_upTitle->AttachSprite(m_pSpriteTitle);
    m_upTitle->SetPosition(450.0f, 200.0, 0.0f);

    //スタートボタン
    Sprite2D::SPRITE_STATE ssStart{ Start_W, Staet_H, Start_W, Staet_H, Start_W, Staet_H };
    //スタート画像の読み込み.
    m_pSpriteStart->Init(_T("Data\\Image\\Setting\\S_Start.png"), ssStart);
    m_upStart->AttachSprite(m_pSpriteStart);

    //エンドボタン
    Sprite2D::SPRITE_STATE ssEnd{ End_W, End_H, End_W, End_H, End_W, End_H };
    //エンド画像の読み込み.
    m_pSpriteEnd->Init(_T("Data\\Image\\Setting\\S_End.png"), ssEnd);
    m_upEnd->AttachSprite(m_pSpriteEnd);

    //フェード用の黒画像
    Sprite2D::SPRITE_STATE ssFade{ WND_W, WND_H, WND_W, WND_H, WND_W, WND_H };
    m_pSpriteFade->Init(_T("Data\\Image\\Setting\\Black.png"), ssFade);
    m_upFade->AttachSprite(m_pSpriteFade);
    m_upFade->SetPosition({ 0.0f, 0.0f, 0.0f });
    m_upFade->SetAlpha(0.0f);

    Sprite2D::SPRITE_STATE ssBack{ 320,80,320,80,320,80 };
    m_pSpriteSelectBack->Init(_T("Data\\Image\\Setting\\SelectBack.png"), ssBack);
    m_upSelectBack->AttachSprite(m_pSpriteSelectBack);
    m_upSelectBack->SetPosition(m_StartPos);

    m_pSpriteSelectFrame->Init(_T("Data\\Image\\Setting\\SelectFrame.png"), ssBack);
    m_upSelectFrame->AttachSprite(m_pSpriteSelectFrame);
    m_upSelectFrame->SetPosition(m_StartPos);

}

void Title::Update()
{
    switch (m_State)
    {
    case TitleState::Select:
        UpdateSelect();
        break;

    case TitleState::FadeOut:
        UpdateFadeOut();
        break;

    case TitleState::First:
        SceneManager::GetInstance()->LoadScene(SceneManager::First);
        break;
    }

    if (GetAsyncKeyState(VK_SPACE) & 0x0001)
    {
        SceneManager::GetInstance()->LoadScene(SceneManager::Lose);
    }
    if (GetAsyncKeyState('A') & 0x0001)
    {
        SceneManager::GetInstance()->LoadScene(SceneManager::Win);
    }
}

void Title::Draw()
{
    auto dx = DirectX11::GetInstance();

    dx->SetDepth(false);
    dx->SetAlphaBlend(true);

    m_upBack->Draw();
    m_upTitle->Draw();

    D3DXVECTOR3 currentSelectPos = (m_Select == SelectMenu::Start) ? m_StartPos : m_EndPos;

    D3DXVECTOR3 backPos = currentSelectPos;
    backPos.x -= 100.0f; //背景を左にずらして、文字を背景の中央に合わせる
    backPos.y -= 15.0f; //背景を少し上にずらして上下の中央を合わせる

    //青い背景
    m_upSelectBack->SetPosition(backPos);
    m_upSelectBack->Draw();

    //選択枠も同じ位置に
    m_upSelectFrame->SetPosition(backPos);
    m_upSelectFrame->Draw();

    //ボタンの座標更新と描画
    m_upStart->SetPosition(m_StartPos);
    m_upEnd->SetPosition(m_EndPos);
    m_upStart->Draw();
    m_upEnd->Draw();

    //フェード
    //ステートに関わらず、アルファ値が0より大きければ描画する
    if (m_FadeAlpha > 0.0f)
    {
        m_upFade->SetAlpha(m_FadeAlpha);
        m_upFade->Draw();
    }

    dx->SetAlphaBlend(false);
    dx->SetDepth(true);
}

void Title::UpdateSelect()
{
    m_InputTimer += Timer::GetInstance().DeltaTime();

    SelectMenu oldSelect = m_Select;

    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        if (m_InputTimer >= 0.2f)
        {
            m_Select = SelectMenu::Start;
        }
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
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

    if (GetAsyncKeyState(VK_RETURN) & 0x0001)
    {
        if (m_InputTimer >= 0.2f)
        {
            SoundManager::GetInstance()->PlaySE(SoundManager::SE_Enter);
            if (m_Select == SelectMenu::Start)
            {
                m_State = TitleState::FadeOut;
                m_FadeAlpha = 0.0f;
            }
            else
            {
                PostQuitMessage(0);
            }
            m_InputTimer = 0.0f;
        }
    }


    if (m_Select == SelectMenu::Start)
    {
        m_StartPos = { 550.f, 550.f, 0.f };
        m_EndPos = { 550.f, 650.f, 0.f };
    }
    else
    {
        m_StartPos = { 550.f, 550.f, 0.f };
        m_EndPos = { 550.f, 650.f, 0.f };
    }

}

void Title::UpdateFadeOut()
{
    m_FadeAlpha += m_FadeSpeed * Timer::GetInstance().DeltaTime();

    if (m_FadeAlpha >= 1.0f)
    {
        m_FadeAlpha = 1.0f;
        m_State = TitleState::First;
    }
}
