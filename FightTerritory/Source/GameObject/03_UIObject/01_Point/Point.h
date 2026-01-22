#pragma once

#include "..//UIObject.h"	//基底クラス.
#include <vector>

class Sprite2D;
class DirectX11;

/******************************************************
*	PointClass.
*	内容：陣地取得時に増えるポイントを管理するクラス.
*		ToDo : ImGuiを使用してカウントを追加させる.
**/

class Point
	: public UIObject
{
public:
	Point();
	~Point() override;

	void Update() override;
	void Draw() override;
	void Create() override;

	//ポイントの表示を切り替える関数.
	//ToDo : インデックスは0もしくは1.
	//プレイヤーの取得ポイント.
	void SetPlayerPointActive(int index, bool active) { m_isPlayerActive[index] = active; }
	//ボスの取得ポイント.
	void SetBossPointActive(int index, bool active) { m_isBossActive[index] = active; }

private:
	// 画像リソース
	std::shared_ptr<Sprite2D> m_spBaseSprite;   //未取得用.
	std::shared_ptr<Sprite2D> m_spActiveSprite; //取得済み用.

	//プレイヤー用のポイント.
	std::shared_ptr<UIObject> m_upPlayerBase[2];
	std::shared_ptr<UIObject> m_upPlayerActive[2];

	//ボス用のポイント.
	std::shared_ptr<UIObject> m_upBossBase[2];
	std::shared_ptr<UIObject> m_upBossActive[2];

	//各ポイントが有効かどうかのフラグ.
	bool m_isPlayerActive[2];
	bool m_isBossActive[2];
};