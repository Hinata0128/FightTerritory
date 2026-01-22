#include "TestFont.h"
#include "Sprite2D/Sprite2D.h"
#include "DirectX/DirectX11.h"

TestFont::TestFont()
	: UIObject()
	, m_FontSprite_0(std::make_shared<Sprite2D>())

	, m_FontUI_0(std::make_shared<UIObject>())
{
	Create();
}

TestFont::~TestFont()
{
}

void TestFont::Create()
{
	const float charSize = 32.0f;
	const float texW = 224.0f;
	const float texH = 32.0f;

	Sprite2D::SPRITE_STATE ssFont =
	{
		texW, texH, texW, texH, texW, texH
	};

	const float posX = 50.0f;
	const float posY = static_cast<float>(WND_H - 150.0f);

	m_FontSprite_0->Init(_T("Data\\Image\\Setting\\PlayerHPFont.png"), ssFont);
	m_FontUI_0->AttachSprite(m_FontSprite_0);
	m_FontUI_0->SetPosition(posX, posY, 0.0f);
}

void TestFont::Update()
{
}

void TestFont::Draw()
{
	DirectX11::GetInstance()->SetDepth(false);
	m_FontUI_0->Draw();
	DirectX11::GetInstance()->SetDepth(true);
}
