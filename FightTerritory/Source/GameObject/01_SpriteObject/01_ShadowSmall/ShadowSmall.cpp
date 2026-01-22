#include "ShadowSmall.h"

#include "Sprite3D/Sprite3D.h"
#include "DirectX/DirectX11.h"

ShadowSmall::ShadowSmall()
	: SpriteObject			()

	, m_pTragetSmall(nullptr)

	, m_pShadowSmallSprite	(std::make_shared<Sprite3D>())
{
	Create();
}

ShadowSmall::~ShadowSmall()
{
}

void ShadowSmall::Update()
{
	if (m_pTragetSmall == nullptr)
	{
		return;
	}

	D3DXVECTOR3 TargetPos = m_pTragetSmall->GetPosition();

	m_Position.x = TargetPos.x;
	m_Position.z = TargetPos.z;
	//高さ調整.
	m_Position.y = 0.01f;

	if (TargetPos.y < 0.0f)
	{
		// スケールを0にして事実上消す、あるいは描画フラグをオフにする
		m_Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		m_Position.x = TargetPos.x;
		m_Position.z = TargetPos.z;
		m_Position.y = 0.01f; // 地面スレスレに配置

		// 影のサイズ設定
		//影の画像のサイズ変更.
		//ToDo : 影のサイズを小さくして弾の表示をする.
		m_Scale = D3DXVECTOR3(0.25f, 0.25f, 0.25f);
	}
	SpriteObject::Update();
}

void ShadowSmall::Draw()
{
	//深度チェックをする.
	DirectX11::GetInstance()->SetDepth(false);
	DirectX11::GetInstance()->SetDepth(true);
	SpriteObject::Draw();

	DirectX11::GetInstance()->SetAlphaBlend(false);
}

void ShadowSmall::Create()
{
	const float ShadowSmallSize = 32.0f;
	//影の小さいものサイズ表示.
	Sprite3D::SPRITE_STATE SSShadowSmall =
	{
		1.0f,1.0f,
		ShadowSmallSize,ShadowSmallSize,
		ShadowSmallSize,ShadowSmallSize,
	};
	//画像の読み込み.
	m_pShadowSmallSprite->Init(_T("Data\\Texture\\shadow.png"), SSShadowSmall);

	this->AttachSprite(*m_pShadowSmallSprite);

	//影を地面に寝かせる.
	m_Rotation.x = D3DXToRadian(90.0f);
}
