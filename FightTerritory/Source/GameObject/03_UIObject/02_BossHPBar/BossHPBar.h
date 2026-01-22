#pragma once

#include "..//UIObject.h"
#include <vector>
#include "GameObject/00_SkinMeshObject/Character//00_Boss/Boss.h"

class Sprite2D;

/***********************************************************
*	BossHPBarの表示クラス.
*	ToDo : プレイヤーの体力と同じ書き方.
**/

class BossHPBar
	: public UIObject
{
public:
	BossHPBar();
	~BossHPBar() override;

	void Update() override;
	void Draw() override;
	void Create() override;

	//ボスの体力減少.
	void SetTragetBoss(Boss* boss) { m_pTargetBoss = boss; }

private:
	Boss* m_pTargetBoss;
	float m_MaxBarScaleX;

	//各パーツ用のSprite
	std::shared_ptr<Sprite2D> m_spBaseSprite;   // HPBase (枠)
	std::shared_ptr<Sprite2D> m_spBackSprite;   // GaugeBack (背景)
	std::shared_ptr<Sprite2D> m_spDamageSprite; // HPDamage (赤)
	std::shared_ptr<Sprite2D> m_spGaugeSprite;  // HPGauge (緑)

	//各パーツ用のUIオブジェクト
	std::shared_ptr<UIObject> m_upBase;
	std::shared_ptr<UIObject> m_upBack;
	std::shared_ptr<UIObject> m_upDamage;
	std::shared_ptr<UIObject> m_upGauge;
};