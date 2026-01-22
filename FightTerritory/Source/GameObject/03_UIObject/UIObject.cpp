#include "UIObject.h"

UIObject::UIObject()
	: m_pSprite		( nullptr )
	, m_PatternNo	()

	, m_Alpha(1.0f)
{
}

UIObject::~UIObject()
{
	DetachSprite();
}

void UIObject::Update()
{
	if( m_pSprite == nullptr ){
		return;
	}
}

//引数なしのDraw()関数
void UIObject::Draw()
{
	if (!m_pSprite) 
	{
		return;
	}

	// 描画直前で座標・回転・スケールを反映
	m_pSprite->SetPosition(m_Position);
	m_pSprite->SetRotation(m_Rotation);
	m_pSprite->SetScale(m_Scale);

	// パターン番号設定
	m_pSprite->SetPatternNo(m_PatternNo.x, m_PatternNo.y);


	m_pSprite->SetAlpha(m_Alpha);

	// レンダリング
	m_pSprite->Render();
}

void UIObject::Create()
{
}

void UIObject::SetAlpha(float alpha)
{
	if (alpha < 0.0f) alpha = 0.0f;
	if (alpha > 1.0f) alpha = 1.0f;

	m_Alpha = alpha;
}
