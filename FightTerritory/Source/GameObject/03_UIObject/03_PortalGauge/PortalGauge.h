#pragma once

#include "..//UIObject.h"

class Sprite2D;
class DirectX11;

/*************************************************************
*	ポータルゲージUIクラス.
*	ToDo : このクラスでは、ポータルのUIの増加をするクラス.
*          基本的なことはプレイヤーのHPクラスを参考に作成.
**/

class PortalGauge
	: public UIObject
{
public:

	// Portalクラスの列挙型と同じか、それに準ずる状態を定義
	enum class GaugeMode : byte
	{
		None,
		Player,
		Boss
	};
public:
	PortalGauge();
	~PortalGauge() override;

	void Update() override;
	void Draw() override;
	void Create() override;

	void SetPercent(float percent) { m_percent = percent; }

	//どちらのゲージを表示するか設定する関数
	void SetGaugeMode(GaugeMode mode) { m_mode = mode; }

private:
	//ゲージのメンバ変数.
	std::shared_ptr<Sprite2D> m_spBaseSprite;  //外枠
	std::shared_ptr<Sprite2D> m_spGaugeSprite; //プレイヤーゲージ本体
	std::shared_ptr<Sprite2D> m_spBossGaugeSprite;	//ボスのゲージ本体

	// UIオブジェクト
	std::shared_ptr<UIObject> m_upBase;
	std::shared_ptr<UIObject> m_upGauge;
	std::shared_ptr<UIObject> m_upBossGauge;

	float m_maxBarScaleX; //枠に収まる最大スケール
	float m_percent;      //目標進捗率
	float m_currentScaleX; //現在の表示スケール (アニメーション用)
	float m_barWidth;     //ゲージテクスチャの元幅

	GaugeMode m_mode = GaugeMode::None; //現在のモード
};