#include "PortalFrame.h"

#include "System/00_Manager/01_StaticMeshManager/StaticMeshManager.h"

PortalFrame::PortalFrame()
{
	AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::PortalFrame));

	//ポータル枠の位置設定.
	D3DXVECTOR3 Pos = { 0.0f, 0.0f, 5.0f };
	//ポータル枠のサイズ設定.
	D3DXVECTOR3 Scale = { 0.05f,0.05f,0.05f };

	SetPosition(Pos);
	SetScale(D3DXVECTOR3(Scale));
}

PortalFrame::~PortalFrame()
{
}

void PortalFrame::Update()
{
	StaticMeshObject::Update();
}

void PortalFrame::Draw()
{
	StaticMeshObject::Draw();
}
