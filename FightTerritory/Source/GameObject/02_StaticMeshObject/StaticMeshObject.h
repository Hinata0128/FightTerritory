#pragma once

#include "GameObject\\GameObject.h"
#include "StaticMash\StaticMesh.h"


/************************************************************
*	スタティックメッシュオブジェクトクラス.
*	3DSTGではメインの分類.
**/
class StaticMeshObject
	: public GameObject
{
public:
	StaticMeshObject();
	virtual ~StaticMeshObject() override;

	virtual void Update() override;
	virtual void Draw() override;

	// メッシュを接続する. 
	void AttachMesh(StaticMesh& pMesh) {
		m_pMesh = &pMesh;
	}
	// スプライトを切り離す.
	void DetachMesh() {
		m_pMesh = nullptr;
	}


protected:
	StaticMesh* m_pMesh;
};