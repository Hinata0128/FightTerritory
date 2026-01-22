#include "Ground.h"
#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"

Ground::Ground()
{
	AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Ground));

}

Ground::~Ground()
{
}

void Ground::Update()
{
}
