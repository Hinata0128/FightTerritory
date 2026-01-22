#pragma once
#include "GameObject//02_StaticMeshObject//02_ShotBase//ShotBase.h"

class StaticMeshManager;
class ShadowSmall;

/**********************************************************************
*	“G’Êí’eƒNƒ‰ƒX.
**/

class BossShot
	: public ShotBase
{
public:
	BossShot();
	~BossShot() override;

	void Update() override;
	void Draw() override;

	//’e‚Ì‰Šú‰».
	void Init() override;

	void Reload(const D3DXVECTOR3& pos, const D3DXVECTOR3& direction, float speed) override;
public:
	void SetMatrices(const D3DXMATRIX& view, const D3DXMATRIX& proj);
private:
	std::shared_ptr<ShadowSmall> m_pShadowSmall;
};