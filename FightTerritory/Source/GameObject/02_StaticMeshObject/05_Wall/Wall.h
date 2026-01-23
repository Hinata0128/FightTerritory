#pragma once
#include "GameObject//02_StaticMeshObject//StaticMeshObject.h"

class StaticMeshManager;

/***************************************************************************
*	壁クラス.
*	ToDo : ImGuiとファイルの保存を使用して実装していく.
**/

class Wall final
	: public StaticMeshObject
{
public:
	Wall();
	~Wall() override;

	void Update() override;
	void Draw() override;

	//WallManagerクラスで配置するための関数.
	//ToDo : ImGuiで座標系統の移動を実装させるために必要.
	D3DXVECTOR3& GetEditablePos();
	D3DXVECTOR3& GetEditableScale();
	D3DXVECTOR3& GetEditableRotation();

private:

};