#pragma once

#include "..//SpriteObject.h"

class Sprite3D;
class DirectX11;

/***********************************************************
*	影クラス.
*	ToDo : プレイヤー・ボス・弾の影を実装.
**/

class Shadow
	: public SpriteObject
{
public:
	Shadow();
	~Shadow() override;

	void Update() override;
	void Draw() override;
	//画像の命名.
	void Create() override;

public:
	//どのオブジェクトにつけるか.
	void SetTargetShadowPos(GameObject* target) { m_pTarget = target; }
private:
	GameObject* m_pTarget;

	std::shared_ptr<Sprite3D> m_pShadowSprite;
};