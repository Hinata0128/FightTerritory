#include "StaticMeshObject.h"

StaticMeshObject::StaticMeshObject()
	: m_pMesh(nullptr)
{
}

StaticMeshObject::~StaticMeshObject()
{
	DetachMesh();
}

void StaticMeshObject::Update()
{
	if (m_pMesh == nullptr) {
		return;
	}
}

void StaticMeshObject::Draw()
{
	if (!m_pMesh) return;

	// Rendererから描画パラメータ取得
	auto& renderer = Renderer::GetInstance();

	// 座標・回転・スケールを反映
	m_pMesh->SetPosition(m_Position);
	m_pMesh->SetRotation(m_Rotation);
	m_pMesh->SetScale(m_Scale);

	// Render呼び出し
	m_pMesh->Render(
		renderer.GetView(),
		renderer.GetProj(),
		renderer.GetLight(),
		renderer.GetCamera().vPosition // カメラ位置 (D3DXVECTOR3型を想定)
	);
}