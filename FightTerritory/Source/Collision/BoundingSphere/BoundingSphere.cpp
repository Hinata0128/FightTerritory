#include "BoundingSphere.h"
#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"

#include "System//01_Renderer//Renderer.h"

BoundingSphere::BoundingSphere()
	: m_Position()
	, m_Radius()
{
	m_pMesh = StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::SphereCollision);
}

BoundingSphere::~BoundingSphere()
{
}

//モデルに合わせたバウンディングスフィアを作成.
HRESULT BoundingSphere::CreateSphereForMesh(const StaticMesh& pMesh)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;	//頂点バッファ.
	void* pVertices = nullptr;				//頂点.
	D3DXVECTOR3 Center(0.f, 0.f, 0.f);		//中心座標.
	float Radius = 0.f;						//半径.

	//頂点バッファを取得.
	if (FAILED(
		pMesh.GetMesh()->GetVertexBuffer(&pVB)))
	{
		return E_FAIL;
	}

	//メッシュの頂点バッファをロックする.
	if (FAILED(
		pVB->Lock(0, 0, &pVertices, 0)))
	{
		SAFE_RELEASE(pVB);
		return E_FAIL;
	}

	//メッシュの外接円の中心と半径を計算する.
	D3DXComputeBoundingSphere(
		static_cast<D3DXVECTOR3*>(pVertices),
		pMesh.GetMesh()->GetNumVertices(),					//頂点の数.
		D3DXGetFVFVertexSize(pMesh.GetMesh()->GetFVF()),	//頂点の情報.
		&Center,											//(out)中心座標.
		&Radius);											//(out)半径.

	//メッシュの頂点バッファをアンロックする.
	if (pVB != nullptr)
	{
		pVB->Unlock();
		SAFE_RELEASE(pVB);
	}

	//中心座標と半径の設定.
	m_Position = Center;
	m_Radius = Radius;

	return S_OK;
}

//他の球体との当たり判定.
bool BoundingSphere::IsHit(const BoundingSphere& pBSphere) const
{
	//2つの球体の中心の距離を求める.
	//参照しているとドット演算子になる.
	D3DXVECTOR3 vLength = m_Position - pBSphere.GetPostion();
	//上記のベクトルから長さに変換.
	float Length = D3DXVec3Length(&vLength);

	//「2つの球体の距離」が「2つの球体のそれぞれの半径を足したもの」より、
	//小さいということは、球体同士が重なっている(衝突している)ということ.
	if (Length <= m_Radius + pBSphere.GetRadius())
	{
		return true;	//衝突している.
	}
	return false;		//衝突していない.
}

void BoundingSphere::Update()
{
}

void BoundingSphere::Draw()
{
	if (!m_pMesh)
	{
		return;
	}

	//Rendererから描画パラメータ取得.
	auto& renderer = Renderer::GetInstance();

	//座標とスケールを設定.
	m_pMesh->SetPosition(m_Position);
	m_pMesh->SetScale(D3DXVECTOR3(m_Radius, m_Radius, m_Radius));

	//レンダリング実行.
	m_pMesh->Render(
		renderer.GetView(),
		renderer.GetProj(),
		renderer.GetLight(),
		renderer.GetCamera().vPosition);
}

