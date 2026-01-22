#include "BossHPBar.h"

#include "DirectX/DirectX11.h"

BossHPBar::BossHPBar()
	: UIObject	()
	
	, m_pTargetBoss(nullptr)
	, m_MaxBarScaleX(0.0f)

	, m_spBaseSprite(std::make_shared<Sprite2D>())
	, m_spBackSprite(std::make_shared<Sprite2D>())
	, m_spDamageSprite(std::make_shared<Sprite2D>())
	, m_spGaugeSprite(std::make_shared<Sprite2D>())
	, m_upBase(std::make_shared<UIObject>())
	, m_upBack(std::make_shared<UIObject>())
	, m_upDamage(std::make_shared<UIObject>())
	, m_upGauge(std::make_shared<UIObject>())

{
	Create();
}

BossHPBar::~BossHPBar()
{
}

void BossHPBar::Update()
{
	//ターゲットが設定されていなかったら返す.
	if (!m_pTargetBoss)
	{
		return;
	}

	//最大HP100と仮定して割合(0.0～1.0)を算出
	float hpRate = m_pTargetBoss->GetEnemyHitPoint() / 100.0f;
	hpRate = max(0.0f, min(1.0f, hpRate)); 

	float targetScaleX = m_MaxBarScaleX * hpRate;

	if (m_upGauge)
	{
		D3DXVECTOR3 greenScale = m_upGauge->GetScale();
		greenScale.x = targetScaleX;
		m_upGauge->SetScale(greenScale);
	}

	if (m_upDamage)
	{
		D3DXVECTOR3 redScale = m_upDamage->GetScale();

		float interpolationSpeed = 0.02f;
		redScale.x += (targetScaleX - redScale.x) * interpolationSpeed;

		if (redScale.x < targetScaleX) {
			redScale.x = targetScaleX;
		}

		m_upDamage->SetScale(redScale);
	}

	if (m_upBase) { m_upBase->Update(); }
	if (m_upBack) { m_upBack->Update(); }
	if (m_upDamage) { m_upDamage->Update(); }
	if (m_upGauge) { m_upGauge->Update(); }

}

void BossHPBar::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);

	if (m_upBack)   m_upBack->Draw();  
	if (m_upDamage) m_upDamage->Draw(); 
	if (m_upGauge)  m_upGauge->Draw();  
	if (m_upBase)   m_upBase->Draw();  

	DirectX11::GetInstance()->SetDepth(true);

}

void BossHPBar::Create()
{
	// -----------------------------------------------------------
	// 各画像のサイズ定義
	// -----------------------------------------------------------
	const float baseW = 460.0f;  // HPBase.png の横幅
	const float baseH = 64.0f;   // HPBase.png の縦幅
	const float barW = 1000.0f; // ゲージ用画像(GaugeBack等)のテクスチャ本来の横幅
	const float barH = 34.0f;   // ゲージ用画像の縦幅

	const float paddingX = 8.0f;
	const float innerBarW = baseW - (paddingX * 2.0f); //枠の中に入るバーの表示幅

	// 表示位置
	const float posX = 800.0f;
	const float posY = static_cast<float>(WND_H - 100.0f);

	m_MaxBarScaleX = innerBarW / barW;

	Sprite2D::SPRITE_STATE ssBase = { baseW, baseH, baseW, baseH, baseW, baseH };
	m_spBaseSprite->Init(_T("Data\\Image\\Buttle\\HPBase.png"), ssBase);
	m_upBase->AttachSprite(m_spBaseSprite);
	m_upBase->SetPosition(posX, posY, 0.0f);

	float offsetY = (baseH - barH) * 0.5f;
	Sprite2D::SPRITE_STATE ssBar = { barW, barH, barW, barH, barW, barH };
	D3DXVECTOR3 initialScale(m_MaxBarScaleX, 1.0f, 1.0f);

	// 背景（灰色ゲージ）
	m_spBackSprite->Init(_T("Data\\Image\\Buttle\\GaugeBack.png"), ssBar);
	m_upBack->AttachSprite(m_spBackSprite);
	m_upBack->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upBack->SetScale(initialScale);

	// ダメージバー（赤色ゲージ）
	m_spDamageSprite->Init(_T("Data\\Image\\Buttle\\HPDamage.png"), ssBar);
	m_upDamage->AttachSprite(m_spDamageSprite);
	m_upDamage->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upDamage->SetScale(initialScale);

	// メインバー（緑色ゲージ）
	m_spGaugeSprite->Init(_T("Data\\Image\\Buttle\\HPGauge.png"), ssBar);
	m_upGauge->AttachSprite(m_spGaugeSprite);
	m_upGauge->SetPosition(posX + paddingX, posY + offsetY, 0.0f);
	m_upGauge->SetScale(initialScale);

}
