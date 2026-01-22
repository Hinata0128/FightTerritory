#pragma once
#include "GameObject/02_StaticMeshObject/StaticMeshObject.h"
#include "GameObject//02_StaticMeshObject//02_ShotBase//ShotBase.h"

#include "GameObject/01_SpriteObject/00_Shadow/Shadow.h"

#include "GameObject/01_SpriteObject/01_ShadowSmall/ShadowSmall.h"

class StaticMeshManager;

/******************************************************************
*	ショットクラス.
**/

class PShot
	: public ShotBase
{
public:
	PShot();
	~PShot() override;

	void Update() override;
	void Draw() override;

	//弾の初期化.
	void Init();

	// ShotBase で純粋仮想にした Reload を実装
	void Reload(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& direction,
		float speed) override;

public:
	void SetMatrices(const D3DXMATRIX& view, const D3DXMATRIX& proj) {
		if (m_pShadowSmall) {
			m_pShadowSmall->SetViewMatrix(view);
			m_pShadowSmall->SetProjMatrix(proj);
		}
	}

private:
	std::unique_ptr<ShadowSmall> m_pShadowSmall;
};