#pragma once

#include "..//SpriteObject.h"

class Sprite3D;
class DirectX11;

class ShadowSmall
	: public SpriteObject
{
public:
	ShadowSmall();
	~ShadowSmall() override;

	void Update() override;
	void Draw() override;
	//小さい影の表示用.
	void Create() override;

public:
	//弾のオブジェクトにセット.
	void SetTargetShadowSmallPos(GameObject* smalltarget) { m_pTragetSmall = smalltarget; };

private:
	GameObject* m_pTragetSmall;

	std::shared_ptr<Sprite3D> m_pShadowSmallSprite;
};