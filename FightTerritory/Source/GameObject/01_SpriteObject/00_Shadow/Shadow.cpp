#include "Shadow.h"

#include "DirectX/DirectX11.h"
#include "Sprite3D/Sprite3D.h"

Shadow::Shadow()
	: SpriteObject	()
	, m_pTarget		(nullptr)
	, m_pShadowSprite(std::make_shared<Sprite3D>())

{
	Create();
}

Shadow::~Shadow()
{
}

void Shadow::Update()
{
	if (m_pTarget == nullptr) 
	{ 
		return; 
	}

	D3DXVECTOR3 targetPos = m_pTarget->GetPosition();

	m_Position.x = targetPos.x;
	m_Position.z = targetPos.z;
	//高さ調整.
	m_Position.y = 0.001f;
	//影の画像のサイズ変更.
	//ToDo : 影のサイズが小さかったので、2倍の大きさに設定.
	m_Scale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);

	m_pShadowSprite->SetPosition(m_Position);
	m_pShadowSprite->SetRotation(m_Rotation);
	m_pShadowSprite->SetScale(m_Scale);

	SpriteObject::Update();
}

void Shadow::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);
	SpriteObject::Draw(); 
	DirectX11::GetInstance()->SetDepth(true);

	DirectX11::GetInstance()->SetAlphaBlend(false);
}

void Shadow::Create()
{
	Sprite3D::SPRITE_STATE SSShadow =
	{
		1.0f, 1.0f,
		128.0, 128.0,
		128.0, 128.0,
	};
	//画像の読み込み.
	m_pShadowSprite->Init(_T("Data\\Texture\\shadow.png"), SSShadow);

	this->AttachSprite(*m_pShadowSprite);

	//影を地面に寝かせる.
	m_Rotation.x = D3DXToRadian(90.0f);
}
