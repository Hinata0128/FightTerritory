#pragma once
#include "..//ManagerBase.h"

#include <fstream>
#include <iomanip>
#include <json.hpp>

class Wall;
class ImGuiManager;

/***************************************************************************************************
*	ステージの壁を生成するクラス.
*	ToDo : jsonとImGuiを使用して壁の位置の変更、josnへの場所の保存を行っていく.
*		   シングルトンの設計で作成していきます.
**/

class WallManager
	: public ManagerBase
{
public:
	
	static WallManager* GetInstance()
	{
		static WallManager s_Instance;
		return &s_Instance;
	}
	~WallManager() override;

	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;
	//jsonファイルを保存する.
	void SaveData(const std::string& Path);
	//jsonファイルの場所を呼ぶ.
	void LoadData(const std::string& Path);

private:
	WallManager();
	//コピー禁止.
	WallManager(const WallManager& rhs) = delete;
	WallManager& operator = (const WallManager& rhs) = delete;

private:
	std::vector<std::unique_ptr<Wall>> m_Wall;
};