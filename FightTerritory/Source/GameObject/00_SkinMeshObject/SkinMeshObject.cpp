#include "SkinMeshObject.h"

SkinMeshObject::SkinMeshObject()
	: GameObject		()
	, m_pMesh			( std::make_shared<SkinMesh>() )
	, m_pAnimCtrl		( nullptr )
	, m_AnimNo			( 0 )
	, m_AnimSpeed		( 0.0 )
	, m_AnimTime		( 0.0 )
	, m_BonePos			()
	, m_Loop			( false )
{

}

SkinMeshObject::~SkinMeshObject()
{
	DetachMesh();
}

void SkinMeshObject::Update()
{
	if( m_pMesh == nullptr ){
		return;
	}

	// 時間を進める
	m_AnimTime += m_AnimSpeed;

	// ループ再生が有効な場合
	if (m_Loop)
	{
		double animPeriod = m_pMesh->GetAnimPeriod(m_AnimNo); // ← アニメの全長取得
		if (m_AnimTime > animPeriod)
		{
			m_AnimTime = 0.0; // 最初に戻す
			m_pAnimCtrl->SetTrackPosition(0, 0.0);
		}
	}

	// DirectXアニメーション時間を進める
	m_pAnimCtrl->AdvanceTime(m_AnimSpeed, nullptr);
}

void SkinMeshObject::Draw()
{
	if (!m_pMesh) return;

	auto& renderer = Renderer::GetInstance();

	// 座標・回転・スケール反映
	m_pMesh->SetPosition(m_Position);
	m_pMesh->SetRotation(m_Rotation);
	m_pMesh->SetScale(m_Scale);

	D3DXVECTOR3 camPos = renderer.GetCamera().vPosition; // 型を確認

	// アニメーションコントローラがnullptrならnullptrを渡す
	m_pMesh->Render(
		renderer.GetView(),
		renderer.GetProj(),
		renderer.GetLight(),
		camPos,
		m_pAnimCtrl
	);
}

//メッシュを接続する.
void SkinMeshObject::AttachMesh(std::shared_ptr<SkinMesh> pMesh)
{
	m_pMesh = pMesh;

	//アニメーションコントローラを取得
	LPD3DXANIMATIONCONTROLLER pAC = m_pMesh->GetAnimationController();

	//アニメーションコントローラのクローンを作成
	if (FAILED(
		pAC->CloneAnimationController(
			pAC->GetMaxNumAnimationOutputs(),
			pAC->GetMaxNumAnimationSets(),
			pAC->GetMaxNumTracks(),
			pAC->GetMaxNumEvents(),
			&m_pAnimCtrl)))
	{
		_ASSERT_EXPR(false, L"アニメーションコントローラのクローン作成失敗");
	}
}
//メッシュを切り離す.
void SkinMeshObject::DetachMesh()
{
	m_pMesh = nullptr;

	SAFE_RELEASE(m_pAnimCtrl);
}