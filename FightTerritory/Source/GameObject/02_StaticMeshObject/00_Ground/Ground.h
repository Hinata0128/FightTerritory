#pragma once
#include "GameObject\02_StaticMeshObject\StaticMeshObject.h"	//基底クラス

/******************************************
*	地面クラス.
**/

class Ground
	: public StaticMeshObject	//スタティックメッシュオブジェクトクラス
{
public:
	Ground();
	virtual ~Ground();

	virtual void Update() override;

private:
};