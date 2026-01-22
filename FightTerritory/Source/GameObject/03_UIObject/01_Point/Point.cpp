#include "Point.h"

#include "DirectX/DirectX11.h"

Point::Point()
    : UIObject()
    , m_spBaseSprite(std::make_shared<Sprite2D>())
    , m_spActiveSprite(std::make_shared<Sprite2D>()) 
{
    for (int i = 0; i < 2; ++i) {
        m_upPlayerBase[i] = std::make_shared<UIObject>();
        m_upPlayerActive[i] = std::make_shared<UIObject>();
        m_upBossBase[i] = std::make_shared<UIObject>();
        m_upBossActive[i] = std::make_shared<UIObject>();

        m_isPlayerActive[i] = false;
        m_isBossActive[i] = false;
    }
}

Point::~Point()
{
}

void Point::Create()
{
	const float size = 32.0f;
	const float margin = 5.0f;
	const float groupSpacing = 100.0f;

	const float startX = 530.0f;
	const float posY = 80.0f;

	Sprite2D::SPRITE_STATE ss = { size, size, size, size, size, size };

	m_spBaseSprite->Init(_T("Data\\Texture\\PointBase.png"), ss);
	m_spActiveSprite->Init(_T("Data\\Texture\\PointActive.png"), ss);

	auto SetupIcons = [&](std::shared_ptr<UIObject> base[], std::shared_ptr<UIObject> active[], float xOffset) {
		for (int i = 0; i < 2; ++i) {
			float x = xOffset + (i * (size + margin));

			base[i]->AttachSprite(m_spBaseSprite);
			base[i]->SetPosition(x, posY, 0.0f);
			base[i]->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f)); // 土台は等倍

			active[i]->AttachSprite(m_spActiveSprite);
			active[i]->SetPosition(x, posY, 0.0f);
			active[i]->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); // 最初は非表示
		}
		};

	SetupIcons(m_upPlayerBase, m_upPlayerActive, startX);
	float bossStartX = startX + (size * 2 + margin) + groupSpacing;
	SetupIcons(m_upBossBase, m_upBossActive, bossStartX);
}

void Point::Update()
{
	// 獲得状況に応じてスケールをアニメーションさせる（0.0 <-> 1.0）
	auto UpdateScale = [](std::shared_ptr<UIObject> obj, bool active) {
		D3DXVECTOR3 currentScale = obj->GetScale();
		float target = active ? 1.0f : 0.0f;
		currentScale.x += (target - currentScale.x) * 0.1f;
		currentScale.y += (target - currentScale.y) * 0.1f;
		obj->SetScale(currentScale);
		};

	for (int i = 0; i < 2; ++i) {
		UpdateScale(m_upPlayerActive[i], m_isPlayerActive[i]);
		UpdateScale(m_upBossActive[i], m_isBossActive[i]);

		// UIObject自体の更新処理（もしあれば）
		m_upPlayerBase[i]->Update();
		m_upPlayerActive[i]->Update();
		m_upBossBase[i]->Update();
		m_upBossActive[i]->Update();
	}
}

void Point::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);

	for (int i = 0; i < 2; ++i) {
		m_upPlayerBase[i]->Draw();
		m_upBossBase[i]->Draw();

		m_upPlayerActive[i]->Draw();
		m_upBossActive[i]->Draw();
	}

	DirectX11::GetInstance()->SetDepth(true);
}

