#pragma once
#include "Scene/SceneBase.h"

class UIObject;
class Sprite2D;
class SoundManager;

/*********************************************
*   タイトルシーンクラス
*********************************************/
class Title : public SceneBase
{
public:
    enum class SelectMenu : byte
    {
        Start,
        End
    };

    enum class TitleState : byte
    {
        Select,
        FadeOut,
        First
    };

public:
    Title();
    ~Title() override;

    void Initialize() override;
    void Create() override;
    void Update() override;
    void Draw() override;

private:
    void UpdateSelect();
    void UpdateFadeOut();

private:
    SelectMenu m_Select;
    TitleState m_State;

    //選択画面の表示位置.
    D3DXVECTOR3 m_StartPos;
    D3DXVECTOR3 m_EndPos;

    //背景画像.
    std::shared_ptr<Sprite2D> m_pSpriteBack;
    std::shared_ptr<UIObject> m_upBack;

    std::shared_ptr<Sprite2D> m_pSpriteTitle;
    std::shared_ptr<UIObject> m_upTitle;

    std::shared_ptr<Sprite2D> m_pSpriteStart;
    std::shared_ptr<UIObject> m_upStart;

    std::shared_ptr<Sprite2D> m_pSpriteEnd;
    std::shared_ptr<UIObject> m_upEnd;

    std::shared_ptr<Sprite2D> m_pSpriteFade;
    std::shared_ptr<UIObject> m_upFade;

    std::shared_ptr<Sprite2D> m_pSpriteSelectBack;
    std::shared_ptr<UIObject> m_upSelectBack;

    std::shared_ptr<Sprite2D> m_pSpriteSelectFrame;
    std::shared_ptr<UIObject> m_upSelectFrame;

};
