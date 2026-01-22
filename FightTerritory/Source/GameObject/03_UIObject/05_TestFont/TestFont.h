#pragma once

#include "..//UIObject.h"

class Sprite2D;
class DirectX11;
class Timer;


class TestFont
	: public UIObject
{
public:
	TestFont();
	~TestFont() override;

	void Create() override;
	void Update() override;
	void Draw() override;

private:


	std::shared_ptr<Sprite2D> m_FontSprite_0;

	std::shared_ptr<UIObject> m_FontUI_0;
};