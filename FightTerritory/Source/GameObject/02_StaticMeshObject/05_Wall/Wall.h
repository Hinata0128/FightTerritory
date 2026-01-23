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

private:

};