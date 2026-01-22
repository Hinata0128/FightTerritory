#pragma once
#include "GameObject//02_StaticMeshObject//02_ShotBase//ShotBase.h"

#include "GameObject//02_StaticMeshObject//StaticMeshObject.h"
#include "Collision//BoundingSphere/BoundingSphere.h"


/************************************************************
*	弾のベースクラス.
**/

class ShotBase
	: public StaticMeshObject
{
public:
	ShotBase();
	virtual ~ShotBase() override;

	virtual void Update() override;
	virtual void Draw() override;
	virtual void Init();

	//弾の再設定（位置・方向・スピード）
	virtual void Reload(const D3DXVECTOR3& pos, const D3DXVECTOR3& direction, float speed) = 0;

	//バウンディングスフィアを位置に同期
	virtual void UpdateBPosition();

	//バウンディングスフィアを取得
	virtual BoundingSphere& GetBoundingSphere() const;

	//表示フラグ設定
	virtual void SetDisplay(bool disp) { m_Disp = disp; }

	//表示フラグ取得
	virtual bool IsDisplay() const { return m_Disp; }

	//アクティブ状態取得
	virtual bool IsActive() const { return m_IsActive; }

	//アクティブ状態を設定する関数 
	virtual void SetActive(bool active) { m_IsActive = active; }

protected:
	//子クラスに必要になるメンバー変数をここに書いておく.
	D3DXVECTOR3 m_Direction;

	float		m_Speed;	//スピード.
	bool		m_Disp;		//弾を表示するかしないか.
	bool		m_IsActive;	//弾が生存しているか?

	std::shared_ptr<BoundingSphere> m_BSphere;
};