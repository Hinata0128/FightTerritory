#pragma once

#include "StaticMash//StaticMesh.h"
#include <cfloat>        //ToDo : FLT_MAX,-FLT_MAXの使用のため書いている

/**************************************************
*	バウンディングボックス.
*  ToDo : 球とボックス、ボックスとボックスの当たり判定用.
**/

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	//モデルに合わせたバウンディングボックスを作成.
	//メッシュの外接AABBを計算する.
	HRESULT CreateBoxForMesh(const StaticMesh& pMesh);

	void Draw();

	//他のボックスとの当たり判定(AABB vs AABB).
	bool IsHit(const BoundingBox& pBBox) const;

	//現在のAABBの最小座標を取得する.
	const D3DXVECTOR3& GetMinPosition() const { return m_MinPosition; }
	//現在のAABBの最大座標を取得する.
	const D3DXVECTOR3& GetMaxPosition() const { return m_MaxPosition; }

	//現在のAABBの最小座標を設定する.
	void SetMinPosition(const D3DXVECTOR3& MinPos) { m_MinPosition = MinPos; }
	//現在のAABBの最大座標を設定する.
	void SetMaxPosition(const D3DXVECTOR3& MaxPos) { m_MaxPosition = MaxPos; }

	//BoundingBoxの中心座標を取得.
	D3DXVECTOR3 GetCenter() const;

	//BoundingBoxのサイズ（幅、高さ、奥行き）を取得する.
	D3DXVECTOR3 GetSize() const;

	//オリジナルのローカルAABBの最小・最大座標を保存/取得する.
	void SetOriginalLocalMinMax(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos);
	D3DXVECTOR3 GetOriginalLocalMin() const { return m_OriginalLocalMin; }
	D3DXVECTOR3 GetOriginalLocalMax() const { return m_OriginalLocalMax; }

	//現在のAABBの最小座標を取得する.
	const D3DXVECTOR3& GetMin() const { return m_MinPosition; }
	//現在のAABBの最大座標を取得する.
	const D3DXVECTOR3& GetMax() const { return m_MaxPosition; }

private:
	D3DXVECTOR3		m_MinPosition;	//AABBの最小座標.
	D3DXVECTOR3		m_MaxPosition;	//AABBの最大座標.
	D3DXVECTOR3		m_OriginalLocalMin; //ローカルAABBの最小座標.
	D3DXVECTOR3		m_OriginalLocalMax; //ローカルAABBの最大座標.

	StaticMesh*		m_pMesh;
	D3DXVECTOR3		m_Position;	//中心座標.
	float			m_Radius;	//半径(長さ).

};