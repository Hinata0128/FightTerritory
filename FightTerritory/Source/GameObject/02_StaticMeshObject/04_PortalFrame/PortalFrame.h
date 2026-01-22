#pragma once
#include "..//StaticMeshObject.h"

class StaticMeshManager;

/*********************************************************
*	ポータル取得範囲クラス.
*	ToDo : ポータルの取得を目で
*		   分かるようにするためにするクラスです.
**/

class PortalFrame final
	: public StaticMeshObject
{
public:
	PortalFrame();
	~PortalFrame() override;

	void Update() override;
	void Draw() override;

private:

};

