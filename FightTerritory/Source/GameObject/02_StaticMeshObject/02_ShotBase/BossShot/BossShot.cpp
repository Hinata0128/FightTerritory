#include "BossShot.h"
#include "System//00_Manager//01_StaticMeshManager//StaticMeshManager.h"

#include "GameObject//01_SpriteObject//01_ShadowSmall//ShadowSmall.h"

BossShot::BossShot()
    : ShotBase          ()
    , m_pShadowSmall    ( std::make_shared<ShadowSmall>() )
{
    AttachMesh(*StaticMeshManager::GetInstance()->GetMeshInstance(StaticMeshManager::CMeshList::EnemyBullet));

    m_pShadowSmall->Create();
    m_pShadowSmall->SetTargetShadowSmallPos(this);

    Init();
}

BossShot::~BossShot() {}

void BossShot::Update()
{
    constexpr float MaxRange = 9999.0f;
    if (!m_IsActive) return;

    if (m_Disp)
    {
        m_Position += m_Direction * m_Speed;
        if (D3DXVec3Length(&m_Position) >= MaxRange) Init();
        UpdateBPosition();

        m_pShadowSmall->Update();
    }
}

void BossShot::Draw()
{
    if (m_Disp)
    {
        ShotBase::Draw();
        m_pShadowSmall->Draw();
    }
}

void BossShot::Init()
{
    ShotBase::Init();

    m_Scale = D3DXVECTOR3(3.0f, 3.0f, 3.0f);
    if (m_BSphere) m_BSphere->SetRadius(0.3f); 
}

void BossShot::Reload(const D3DXVECTOR3& pos, const D3DXVECTOR3& direction, float speed)
{
    m_Position = pos;
    m_Direction = direction;
    m_Speed = speed;
    m_Disp = true;
    m_IsActive = true;

    if (m_BSphere) m_BSphere->SetPosition(m_Position);

    m_pShadowSmall->SetTargetShadowSmallPos(this);
    if (m_pShadowSmall)
    {
        m_pShadowSmall->Update();
    }
}

void BossShot::SetMatrices(const D3DXMATRIX& view, const D3DXMATRIX& proj)
{
    if (m_pShadowSmall)
    {
        m_pShadowSmall->SetViewMatrix(view);
        m_pShadowSmall->SetProjMatrix(proj);
    }
}
