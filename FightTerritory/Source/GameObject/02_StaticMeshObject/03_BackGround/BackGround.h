#pragma once

/******************************************************************************
*	背景画像表示用クラス.
* 
*	背景に画像を表示させている.
* 
**/

#include "GameObject//02_StaticMeshObject//StaticMeshObject.h"	//基底クラス.

class StaticMeshManager;

class BackGround final
	: public StaticMeshObject
{
public:
	BackGround();
	~BackGround() override;

	void Update() override;
	void Draw() override;
private:
	
};