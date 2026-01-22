#include "PortalGauge.h"
#include "Sprite2D/Sprite2D.h"
#include "DirectX/DirectX11.h"
#include <algorithm>

PortalGauge::PortalGauge()
	: UIObject()
	, m_spBaseSprite(std::make_shared<Sprite2D>())
	, m_spGaugeSprite(std::make_shared<Sprite2D>())
	, m_spBossGaugeSprite(std::make_shared<Sprite2D>())

	, m_upBase(std::make_shared<UIObject>())
	, m_upGauge(std::make_shared<UIObject>())
	, m_upBossGauge(std::make_shared<UIObject>())

	, m_maxBarScaleX(0.0f)
	, m_percent(0.0f)
	, m_currentScaleX(0.0f)
	, m_barWidth(0.0f)

	, m_mode(GaugeMode::None)
{
	Create();
}

PortalGauge::~PortalGauge()
{
}

void PortalGauge::Create()
{
	// -----------------------------------------------------------
	//各画像のサイズ定義
	// -----------------------------------------------------------
	const float baseW = 460.0f;  // HPBase.png の横幅
	const float baseH = 64.0f;   // HPBase.png の縦幅
	const float barW = 1000.0f; // PortalGauge.png の本来の横幅
	const float barH = 34.0f;   // PortalGauge.png の本来の縦幅

	const float paddingX = 8.0f; 
	const float innerBarW = baseW - (paddingX * 2.0f);

	const float posX = 420.0f;
	const float posY = 10.0f;

	m_barWidth = barW;
	m_maxBarScaleX = innerBarW / barW; 
	SetPosition(posX, posY, 0.0f);

	Sprite2D::SPRITE_STATE ssBase = { baseW, baseH, baseW, baseH, baseW, baseH };
	m_spBaseSprite->Init(_T("Data\\Image\\Buttle\\HPBase.png"), ssBase);
	m_upBase->AttachSprite(m_spBaseSprite);
	m_upBase->SetPosition(posX, posY, 0.0f);

	float offsetY = (baseH - barH) * 0.5f;
	Sprite2D::SPRITE_STATE ssBar = { barW, barH, barW, barH, barW, barH };

	m_spGaugeSprite->Init(_T("Data\\Image\\Buttle\\PortalGauge.png"), ssBar);
	m_upGauge->AttachSprite(m_spGaugeSprite);

	m_upGauge->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upGauge->SetScale(D3DXVECTOR3(0.0f, 1.0f, 1.0f));

	m_spBossGaugeSprite->Init(_T("Data\\Image\\Buttle\\PortalBossGauge.png"), ssBar);
	m_upBossGauge->AttachSprite(m_spBossGaugeSprite);

	m_upBossGauge->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upBossGauge->SetScale(D3DXVECTOR3(0.0f, 1.0f, 1.0f));


}

void PortalGauge::Update()
{
	float targetScaleX = m_maxBarScaleX * m_percent;

	float interpolationSpeed = 0.05f;
	m_currentScaleX += (targetScaleX - m_currentScaleX) * interpolationSpeed;
	
	if (m_upGauge)
	{
		m_upGauge->SetScale(D3DXVECTOR3(m_currentScaleX, 1.0f, 1.0f));
	}

	D3DXVECTOR3 scale(m_currentScaleX, 1.0f, 1.0f);
	if (m_upGauge) { m_upGauge->SetScale(scale);     m_upGauge->Update(); }
	if (m_upBossGauge) { m_upBossGauge->SetScale(scale); m_upBossGauge->Update(); }
	if (m_upBase) { m_upBase->Update(); }
}

void PortalGauge::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);

	if (m_upBossGauge && m_currentScaleX > 0.001f)
	{
		if (m_mode == GaugeMode::Player && m_upGauge) 
		{
			m_upGauge->Draw();
		}
		else if (m_mode == GaugeMode::Boss && m_upBossGauge) 
		{
			m_upBossGauge->Draw();
		}
	}
	if (m_upBase)
	{
		m_upBase->Draw();
	}

	DirectX11::GetInstance()->SetDepth(true);
}