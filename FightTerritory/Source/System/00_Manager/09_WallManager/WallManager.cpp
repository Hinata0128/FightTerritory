#include "WallManager.h"

#include "GameObject//02_StaticMeshObject//05_Wall//Wall.h"

using json = nlohmann::json;

WallManager::WallManager()
	: ManagerBase	()
{
}

WallManager::~WallManager()
{
	Release();
}

void WallManager::Update()
{
}

void WallManager::Draw()
{
}

void WallManager::Create()
{
}

void WallManager::Release()
{
}

void WallManager::SaveData(const std::string& Path)
{
}

void WallManager::LoadData(const std::string& Path)
{
}
