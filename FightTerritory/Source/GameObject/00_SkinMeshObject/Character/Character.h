#pragma once
#include "GameObject//00_SkinMeshObject//SkinMeshObject.h"
#include "Collision//BoundingSphere//BoundingSphere.h"

/***************************************************************
*	キャラクタークラス.
*	ToDo : このクラスを親としてプレイヤー・ボスを子クラスとして設定.
**/

class Character
	: public SkinMeshObject
{
public:
	Character();
	virtual ~Character() override;

	virtual void Update() override;
	virtual void Draw() override;

	virtual void Init();
protected:
	//当たり判定を中心天にオフセット.
	D3DXVECTOR3 m_HitCenterOffset;
	//体力用.
	float		m_HitPoint;
};