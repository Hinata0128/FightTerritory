#include "PlayerContext.h"

#include "GameObject/00_SkinMeshObject/Character/01_Player/Player.h"
#include "SkinMesh/SkinMesh.h"

PlayerContext::PlayerContext(Player* player) 
    : Position      (player->m_Position)
    , Rotation      (player->m_Rotation)
    , Scale         (player->m_Scale)
    , AnimNo        (player->m_AnimNo)
    , AnimTime      (player->m_AnimTime)
    , AnimSpeed     (player->m_AnimSpeed)
    , Mesh          (player->m_pMesh.get())
    , AnimCtrl      (player->m_pAnimCtrl)
    , BonePos       (player->m_BonePos)
{
}