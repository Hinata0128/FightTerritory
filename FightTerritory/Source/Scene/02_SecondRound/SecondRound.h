#pragma once
#include "Scene//SceneBase.h"	//基底クラス.

class SceneManager;
class Timer;
class UIObject;
class Sprite2D;

/********************************************************************************
*	セカンドラウンド開始時に表示させる画像のクラス.
* 
*	SEを鳴らしてわかりやすくする.
**/

class SecondRound final
	: public SceneBase
{
public:
	SecondRound();
	~SecondRound() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;
private:
	std::shared_ptr<Sprite2D> m_pSpriteSecondRound;
	std::shared_ptr<UIObject> m_upSecondRound;

};