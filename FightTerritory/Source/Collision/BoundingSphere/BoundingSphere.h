#pragma once

#include "StaticMash//StaticMesh.h"

/**********************************************************************************
*	バウンディングスフィアクラス(当たり判定クラス).
**/

class BoundingSphere
{
public:
	BoundingSphere();
	~BoundingSphere();

	//モデルに合わせたバウンディングスフィアを作成.
	//引数に&がある時はその時のアドレスを手に入れることが確実にできる.
	HRESULT CreateSphereForMesh(const StaticMesh& pMesh);

	//他の球体との当たり判定.
	bool IsHit(const BoundingSphere& pBSphere) const;

	//中心座標を取得する.
	const D3DXVECTOR3& GetPostion() const { return m_Position; }
	//半径(長さ)を取得する.
	float GetRadius() const { return m_Radius; }

	//中心座標を設定する.
	void SetPosition(const D3DXVECTOR3& Pos) { m_Position = Pos; }
	//半径(長さ)を設定する.
	void SetRadius(float Radius) { m_Radius = Radius; }
	
	void Update();
	void Draw();


private:
	StaticMesh*		m_pMesh;
	D3DXVECTOR3		m_Position;	//中心座標.
	float			m_Radius;	//半径(長さ).
};