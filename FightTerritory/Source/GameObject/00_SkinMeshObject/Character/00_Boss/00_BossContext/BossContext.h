#pragma once

class Boss;
class SkinMesh;

/************************************************************************
*	NomalStateと攻撃用のステートを使用する時に必要なコードを書く.
**/

class BossContext
{
public:
	D3DXVECTOR3&					Position;
	D3DXVECTOR3&					Rotation;
	D3DXVECTOR3&					Scale;
	int&							AnimNo;
	double&							AnimTime;
	double&							AnimSpeed;
	SkinMesh*						Mesh;
	LPD3DXANIMATIONCONTROLLER		AnimCtrl;
	D3DXVECTOR3&					BonePos;

	BossContext(Boss* nomal);
private:
};