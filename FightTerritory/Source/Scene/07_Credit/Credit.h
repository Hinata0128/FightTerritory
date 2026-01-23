#pragma once
#include "Scene//SceneBase.h"

class SceneManager;
class UIObject;
class Sprite2D;
class DirectX11;
class Timer;

/**************************************************************************
*	クレジット画面クラス.
*	ToDo : 使用したフォント・BGM・SEを書いておく.
*		   クレジットからはタイトルへしか移動できない用にする.
**/

class Credit final
	: public SceneBase
{
public:
	//遷移メニューの列挙.
	enum class SelectMenu : byte
	{
		End	//タイトルシーンに遷移.
	};

	//フェードを使用してタイトルに戻るようにする.
	enum class CreditState : byte
	{
		Select,		//フェードに入るため.
		FadeOut,	//Titleシーンに戻る前にフェードを入れる.
		Title		//FadeOut後にTitleに遷移させる.
	};

public:
	Credit();
	~Credit() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

private:
	void UpdateSelect();
	void UpdateFadeOut();
private:
	SelectMenu	m_Select;
	CreditState m_State;

	//文字画像の表示位置.
	D3DXVECTOR3 m_EndPos;
	D3DXVECTOR3 m_EndSelectPos;

#pragma region SmartPointer
	//背景画像.
	std::shared_ptr<Sprite2D> m_pSpriteBack;
	std::shared_ptr<UIObject> m_upBack;
	//フェード画像.
	//ToDo : タイトルに戻る時にフェードを入れる.
	std::shared_ptr<Sprite2D> m_pSpriteFade;
	std::shared_ptr<UIObject> m_upFade;
	//End画像.
	std::shared_ptr<Sprite2D> m_pSpriteEnd;
	std::shared_ptr<UIObject> m_upEnd;
	//選択肢.
	std::shared_ptr<Sprite2D> m_pSpriteSelectBack;
	std::shared_ptr<UIObject> m_upSelectBack;
	//選択肢の枠.
	std::shared_ptr<Sprite2D> m_pSpriteSelectFrame;
	std::shared_ptr<UIObject> m_upSelectFrame;
#pragma endregion
};