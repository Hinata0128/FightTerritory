#include "LimitTime.h"

#include "Sprite2D/Sprite2D.h"
#include "DirectX/DirectX11.h"
#include "System/02_Singleton/00_Timer/Timer.h"

LimitTime::LimitTime()
	: UIObject				()

	, m_pClockBackSprite	( std::make_shared<Sprite2D>() )
	, m_pClockFrameSprite	( std::make_shared<Sprite2D>() )
	, m_pClockFrontSprite	( std::make_shared<Sprite2D>() )
	, m_pClockMinSprite		( std::make_shared<Sprite2D>() )

	, m_upClockBack			( std::make_shared<UIObject>() )
	, m_upClockFrame		( std::make_shared<UIObject>() )
	, m_upClockFront		( std::make_shared<UIObject>() )
	, m_upClockMin			( std::make_shared<UIObject>() )

#if 0
	, m_LimitTime			( 90.0f )
	, m_RemainTime			( 90.0f )
#else
	//デバッグ確認用
	, m_LimitTime			( 500.0f )
	, m_RemainTime			( 500.0f )
#endif
{
	Create();
}

LimitTime::~LimitTime()
{
}

void LimitTime::Create()
{
	//画像の高さ.
	const float Clock = 158.0f;

	//制限時間の表示位置.
	const float posX = static_cast<float>(WND_W - 200);
	const float posY = 30.0f;

	//表示サイズの設定.
	//ToDo : 全部の画像に使用する. 
	Sprite2D::SPRITE_STATE ssAllClock = { Clock ,Clock ,Clock ,Clock ,Clock ,Clock };

	//制限時間の表示.
	//灰色の枠.
	m_pClockBackSprite->Init(_T("Data\\Image\\Buttle\\ClockBack.png"), ssAllClock);
	m_upClockBack->AttachSprite(m_pClockBackSprite);
	m_upClockBack->SetPosition(posX, posY, 0.0f);
	//黒色の枠.
	m_pClockFrameSprite->Init(_T("Data\\Image\\Buttle\\ClockFrame.png"), ssAllClock);
	m_upClockFrame->AttachSprite(m_pClockFrameSprite);
	m_upClockFrame->SetPosition(posX, posY, 0.0f);
	//白色の枠.
	m_pClockFrontSprite->Init(_T("Data\\Image\\Buttle\\ClockFront.png"), ssAllClock);
	m_upClockFront->AttachSprite(m_pClockFrontSprite);
	m_upClockFront->SetPosition(posX, posY, 0.0f);
	//時計の針.
	m_pClockMinSprite->Init(_T("Data\\Image\\Buttle\\ClockMin.png"), ssAllClock);
	m_upClockMin->AttachSprite(m_pClockMinSprite);
	m_upClockMin->SetPosition(posX, posY, 0.0f);
}

void LimitTime::Update()
{
	float deltaTime = Timer::GetInstance().DeltaTime();
	m_RemainTime -= deltaTime;
	if (m_RemainTime < 0.0f) m_RemainTime = 0.0f;

	float Rate = m_RemainTime / m_LimitTime;

	float AngleDeg = (1.0f - Rate) * 360.0f;
	float m_RotateZ = D3DXToRadian(AngleDeg);

	m_upClockMin->SetRotation(D3DXVECTOR3(0.0f, 0.0f, m_RotateZ));

	float halfW = 158.0f / 2.0f;
	float halfH = 158.0f / 2.0f;

	float posX = static_cast<float>(WND_W - 200.f);
	float posY = 30.0f;

	float targetX = posX + halfW;
	float targetY = posY + halfH;

	float cosZ = cosf(m_RotateZ);
	float sinZ = sinf(m_RotateZ);

	float rotatedCenterX = (halfW * cosZ) - (halfH * sinZ);
	float rotatedCenterY = (halfW * sinZ) + (halfH * cosZ);

	m_upClockMin->SetPosition(targetX - rotatedCenterX, targetY - rotatedCenterY, 0.0f);
}

void LimitTime::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);
	//表示.
	m_upClockBack->Draw();
	m_upClockFront->Draw();
	m_upClockFrame->Draw();
	m_upClockMin->Draw();
	DirectX11::GetInstance()->SetDepth(true);
}
