#pragma once

#include "GameObject//GameObject.h"
#include "SkinMesh//SkinMesh.h"

/************************************************************
*	スキンメッシュオブジェクトクラス.
*	ToDo : プレイヤー・ボスのメッシュを使用するためのクラス.
**/
class SkinMeshObject
	: public GameObject
{
public:
	SkinMeshObject();
	virtual ~SkinMeshObject() override;

	virtual void Update() override;
	virtual void Draw() override;

	//メッシュを接続する.
	void AttachMesh(std::shared_ptr<SkinMesh> pMesh);
	//メッシュを切り離す.
	void DetachMesh();

public:
	//セット・ゲット関数.
	//接続メッシュ名を取得.
	const std::weak_ptr<SkinMesh> GetAttachMesh() const
	{
		return m_pMesh;
	}

protected:
	std::shared_ptr<SkinMesh>	m_pMesh;
	LPD3DXANIMATIONCONTROLLER	m_pAnimCtrl;	//アニメーションコントローラ

	int				m_AnimNo;			//アニメーション番号
	double			m_AnimTime;			//アニメーション経過時間
	double			m_AnimSpeed;		//アニメーション速度
	D3DXVECTOR3		m_BonePos;			//ボーン座標

	bool			m_Loop;	//アニメーションのループ再生.
};
