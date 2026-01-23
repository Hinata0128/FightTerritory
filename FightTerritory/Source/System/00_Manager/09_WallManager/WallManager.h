#pragma once
#include "..//ManagerBase.h"

class Wall;

/***************************************************************************************************
*	ステージの壁を生成するクラス.
*	ToDo : jsonとImGuiを使用して壁の位置の変更、josnへの場所の保存を行っていく.
**/

class WallManager
	: public ManagerBase
{
public:
	WallManager();
	~WallManager() override;

	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;


private:

};