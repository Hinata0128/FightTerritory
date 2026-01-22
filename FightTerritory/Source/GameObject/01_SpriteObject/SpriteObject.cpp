#include "SpriteObject.h"

SpriteObject::SpriteObject()
	: m_pSprite(nullptr)
{
	D3DXMatrixIdentity(&m_View);
	D3DXMatrixIdentity(&m_Proj);
}

SpriteObject::~SpriteObject()
{
	DetachSprite();
}

void SpriteObject::Update()
{
	if (m_pSprite == nullptr) {
		return;
	}
}

void SpriteObject::Draw()
{
	if (m_pSprite == nullptr) {
		return;
	}

	//座標や回転情報などをスプライトに反映
	m_pSprite->SetPosition(m_Position);
	m_pSprite->SetRotation(m_Rotation);
	m_pSprite->SetScale(m_Scale);

	//設定済みのView・Proj行列を使用して描画
	m_pSprite->Render(m_View, m_Proj);
}
