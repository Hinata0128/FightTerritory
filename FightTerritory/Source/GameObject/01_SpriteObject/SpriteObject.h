#pragma once

#include "GameObject/GameObject.h"
#include "Sprite3D/Sprite3D.h"

/************************************************************
*	スプライトオブジェクトクラス.
**/
class SpriteObject
	: public GameObject
{
public:
	SpriteObject();
	virtual ~SpriteObject() override;

	// GameObjectの純粋仮想関数を定義
	virtual void Update() override;
	// 引数なしDraw（オーバーライド）
	virtual void Draw() override;
	//構築関数.
	virtual void Create() {}

	// スプライトを接続する
	void AttachSprite(Sprite3D& pSprite) {
		m_pSprite = &pSprite;
	}
	// スプライトを切り離す
	void DetachSprite() {
		m_pSprite = nullptr;
	}

	// 行列を外部から設定できるようにする
	void SetViewMatrix(const D3DXMATRIX& view) { m_View = view; }
	void SetProjMatrix(const D3DXMATRIX& proj) { m_Proj = proj; }

protected:
	Sprite3D* m_pSprite;
	D3DXMATRIX m_View;
	D3DXMATRIX m_Proj;
};
