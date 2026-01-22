#pragma once

#include "GameObject/GameObject.h"
#include "Sprite2D/Sprite2D.h"

/************************************************************
*	UIオブジェクトクラス.
**/
class UIObject 
	: public GameObject
{
public:
	UIObject();
	virtual ~UIObject() override;

	// Update と Draw
	virtual void Update() override;
	virtual void Draw() override;

	virtual void Create();


	// スプライトを接続する
	void AttachSprite(std::shared_ptr<Sprite2D> pSprite) { m_pSprite = std::move(pSprite); }
	void DetachSprite() { m_pSprite.reset(); }

	// パターン番号を設定
	void SetPatternNo(SHORT x, SHORT y)
	{
		m_PatternNo.x = x;
		m_PatternNo.y = y;
	}

	void SetAlpha(float alpha);
	float GetAlpha() const { return m_Alpha; }

protected:
	std::shared_ptr<Sprite2D> m_pSprite;

	POINTS		m_PatternNo;	// パターン番号(マス目)

	float  m_Alpha;
};
