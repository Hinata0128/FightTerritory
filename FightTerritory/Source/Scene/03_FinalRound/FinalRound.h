#pragma once
#include "Scene//SceneBase.h"	//Šî’êƒNƒ‰ƒX.

class SceneManager;
class Timer;
class UIObject;
class Sprite2D;


class FinalRound final
	: public SceneBase
{
public:
	FinalRound();
	~FinalRound() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

private:
	std::shared_ptr<Sprite2D> m_pSpriteFinalRound;
	std::shared_ptr<UIObject> m_upFinalRound;

};