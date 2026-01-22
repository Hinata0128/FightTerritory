#pragma once
#include "Scene/SceneBase.h"

class SceneManager;
class UIObject;
class Sprite2D;
class DirectX11;
class Timer;

/*********************************************
*	ゲームオーバー画像クラス.
*	ToDo : Lose画面の作成.
**/

class GameOver
	: public SceneBase
{
public:
	enum class SelectMenu : byte
	{
		Continue,	//Firstシーンへ遷移.
		End			//Titleに遷移.
	};

	enum class LoseState : byte
	{
		Select,		//コンティニューかタイトルへ戻すか.
		FadeOut,	//Firstシーンに入る前に再生.
		First,		//FadeOut後にFirstRoundに遷移させる.
	};
public:
	GameOver();
	~GameOver() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

private:
	void UpdateSelect();
	void UpdateFadeOut();

private:
	SelectMenu	m_Select;
	LoseState	m_State;

	//コンティニューの位置.
	D3DXVECTOR3 m_ContinuePos;
	//ずらす用のEndの位置
	D3DXVECTOR3 m_EndPos;
	//End画像の表示位置
	//ToDo : コンティニューと違和感のない配置にする.
	D3DXVECTOR3 m_EndSelectPos;

//ToDo : スマートポインタをまとめている.
#pragma region smartpointer
	//背景画像.
	std::shared_ptr<Sprite2D> m_pSpriteBack;
	std::shared_ptr<UIObject> m_upBack;
	//敗北画像.
	std::shared_ptr<Sprite2D> m_pSpriteDefeat;
	std::shared_ptr<UIObject> m_upDefeat;
	//コンティニュー画像.
	std::shared_ptr<Sprite2D> m_pSpriteContinue;
	std::shared_ptr<UIObject> m_upContinue;
	//タイトルへ戻る用のEnd画像.
	std::shared_ptr<Sprite2D> m_pSpriteEnd;
	std::shared_ptr<UIObject> m_upEnd;
	//コンティニューを選んだときに流すフェード画像.
	std::shared_ptr<Sprite2D> m_pSpriteFade;
	std::shared_ptr<UIObject> m_upFade;
	//選択肢.
	std::shared_ptr<Sprite2D> m_pSpriteSelectBack;
	std::shared_ptr<UIObject> m_upSelectBack;
	//選択肢の枠.
	std::shared_ptr<Sprite2D> m_pSpriteSelectFrame;
	std::shared_ptr<UIObject> m_upSelectFrame;
#pragma endregion

};