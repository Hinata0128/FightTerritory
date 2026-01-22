#include "BoundingBox.h" 
#include "System//01_Renderer//Renderer.h"

//SAFE_RELEASEマクロの定義.
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }
#endif


//MAX_FVF_DECL_SIZEの定義.
#ifndef MAX_FVF_DECL_SIZE
#define MAX_FVF_DECL_SIZE (27) // MAXD3DDECLLENGTH + 1
#endif


BoundingBox::BoundingBox()
    : m_MinPosition         ( FLT_MAX, FLT_MAX, FLT_MAX )
    , m_MaxPosition         ( -FLT_MAX, -FLT_MAX, -FLT_MAX )
    , m_OriginalLocalMin    ( FLT_MAX, FLT_MAX, FLT_MAX )
    , m_OriginalLocalMax    ( -FLT_MAX, -FLT_MAX, -FLT_MAX )
    , m_pMesh               ( nullptr )
    , m_Position            {}
    , m_Radius              ( 0.0f )
{
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::Draw()
{
    if (!m_pMesh)
    {
        return;
    }

    //Rendererから描画パラメータ取得.
    auto& renderer = Renderer::GetInstance();

    //座標とスケールを設定.
    m_pMesh->SetPosition(m_Position);
    m_pMesh->SetScale(m_Radius);

    //レンダリング実行.
    m_pMesh->Render(
        renderer.GetView(),
        renderer.GetProj(),
        renderer.GetLight(),
        renderer.GetCamera().vPosition);
}

HRESULT BoundingBox::CreateBoxForMesh(const StaticMesh& rMesh)
{
    ID3DXMesh* d3dxMesh = rMesh.GetMeshForRay();
    if (d3dxMesh == nullptr) 
    {
        return E_FAIL;
    }

    LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
    void* pVertices = nullptr;
    HRESULT hr = S_OK;
    D3DXVECTOR3 tempMin, tempMax;

    hr = d3dxMesh->GetVertexBuffer(&pVB);
    if (FAILED(hr)) 
    {
        return hr;
    }

    hr = pVB->Lock(0, 0, &pVertices, 0);
    if (FAILED(hr)) 
    {
        SAFE_RELEASE(pVB);
        return hr;
    }

    DWORD fvf = d3dxMesh->GetFVF();
    DWORD vertexStride = D3DXGetFVFVertexSize(fvf);

    if (vertexStride == 0) 
    {
        D3DVERTEXELEMENT9 elements[MAX_FVF_DECL_SIZE];
        if (FAILED(d3dxMesh->GetDeclaration(elements))) {
            pVB->Unlock();
            SAFE_RELEASE(pVB);
            return E_FAIL;
        }
        vertexStride = D3DXGetDeclVertexSize(elements, 0);
    }

    if (vertexStride == 0) 
    {
        pVB->Unlock();
        SAFE_RELEASE(pVB);
        return E_FAIL;
    }

    D3DXComputeBoundingBox(
        static_cast<const D3DXVECTOR3*>(pVertices),
        d3dxMesh->GetNumVertices(),
        vertexStride,
        &tempMin, 
        &tempMax
    );

    pVB->Unlock();
    SAFE_RELEASE(pVB);

    //計算されたMin/Maxを、現在のAABBとオリジナルのローカルAABBの両方に設定.
    m_MinPosition = tempMin;
    m_MaxPosition = tempMax;
    m_OriginalLocalMin = tempMin; 
    m_OriginalLocalMax = tempMax; 

    const float desiredHeight = 0.0f;
    m_MaxPosition.y = m_MinPosition.y + desiredHeight;
    m_OriginalLocalMax.y = m_OriginalLocalMin.y + desiredHeight; // オリジナルも更新

    return S_OK;
}

bool BoundingBox::IsHit(const BoundingBox& otherBBox) const
{
    //X軸での重なりがないかチェック.
    if (m_MaxPosition.x < otherBBox.m_MinPosition.x || m_MinPosition.x > otherBBox.m_MaxPosition.x)
    {
        return false;
    }
    //Y軸での重なりがないかチェック.
    if (m_MaxPosition.y < otherBBox.m_MinPosition.y || m_MinPosition.y > otherBBox.m_MaxPosition.y)
    {
        return false;
    }
    //Z軸での重なりがないかチェック.
    if (m_MaxPosition.z < otherBBox.m_MinPosition.z || m_MinPosition.z > otherBBox.m_MaxPosition.z)
    {
        return false;
    }

    //全ての軸で重なっていれば衝突.
    return true;
}

D3DXVECTOR3 BoundingBox::GetCenter() const
{
    return (m_MinPosition + m_MaxPosition) * 0.5f;
}

D3DXVECTOR3 BoundingBox::GetSize() const
{
    return m_MaxPosition - m_MinPosition;
}

void BoundingBox::SetOriginalLocalMinMax(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos)
{
    m_OriginalLocalMin = minPos;
    m_OriginalLocalMax = maxPos;
}