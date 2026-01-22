#include "BackGround.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"

BackGround::BackGround()
	: StaticMeshObject	()
{
	AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::SkyBox));

	//SkyBoxÇÃÉTÉCÉY.
	D3DXVECTOR3 Scale = { 100.0f,100.0f,100.0f };

	SetScale(Scale);
}

BackGround::~BackGround()
{
}

void BackGround::Update()
{
	StaticMeshObject::Update();
}

void BackGround::Draw()
{
	StaticMeshObject::Draw();
}
