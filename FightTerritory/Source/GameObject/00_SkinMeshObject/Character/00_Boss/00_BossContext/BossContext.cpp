#include "BossContext.h"

#include "GameObject//00_SkinMeshObject//Character//00_Boss//Boss.h"
#include "SkinMesh//SkinMesh.h"

BossContext::BossContext(Boss* nomal)
    : Position  ( nomal->m_Position )
    , Rotation  ( nomal->m_Rotation )
    , Scale     ( nomal->m_Scale )
    , AnimNo    ( nomal->m_AnimNo )
    , AnimTime  ( nomal->m_AnimTime )
    , AnimSpeed ( nomal->m_AnimSpeed )
    , Mesh      ( nomal->m_pMesh.get() )
    , AnimCtrl  ( nomal->m_pAnimCtrl )
    , BonePos   ( nomal->m_BonePos )
{
}
