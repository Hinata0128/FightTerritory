#include "Wall.h"

#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"

Wall::Wall()
	: StaticMeshObject	()
{
	//壁のアタッチ.
	AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::Wall));

	//壁の位置(仮).
	D3DXVECTOR3 Pos = { -20.0f,1.5f,0.0f };
	//壁のサイズ(仮).
	D3DXVECTOR3 Scale = { 5.0f,5.0f,5.0f};

	SetPosition(Pos);
	SetScale(Scale);
}

Wall::~Wall()
{
}

void Wall::Update()
{
	StaticMeshObject::Update();
}

void Wall::Draw()
{
	StaticMeshObject::Draw();
}

D3DXVECTOR3& Wall::GetEditablePos()
{
	return m_Position;
}

D3DXVECTOR3& Wall::GetEditableScale()
{
	return m_Scale;
}

D3DXVECTOR3& Wall::GetEditableRotation()
{
	return m_Rotation;
}
