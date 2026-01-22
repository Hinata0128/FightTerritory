#include "AttackLong.h"

#include "GameObject/00_SkinMeshObject/Character/01_Player/Player.h"
#include "System/00_Manager/02_PShotManager/PShotManager.h"
#include "GameObject/00_SkinMeshObject/Character/01_Player/PlayerContext/PlayerContext.h"
#include "System/02_Singleton/00_Timer/Timer.h"

#include "System/00_Manager/03_ImGuiManager/ImGuiManager.h"

AttackLong::AttackLong()
	: PlayerAttckStateBase  ()
    , m_ShotCoolDown	    (0.0f)
	, m_CoolTime		    (1.0f)
{
}

AttackLong::~AttackLong()
{
}

void AttackLong::Enter(Player* player)
{
	m_ShotCoolDown = m_CoolTime;
}

void AttackLong::ExecuteAttack(Player* player)
{
    //クールタイムのデルタタイムを取得.
    float deltaTime = Timer::GetInstance().DeltaTime();

    //クールタイム管理.
    m_ShotCoolDown -= deltaTime;
    if (m_ShotCoolDown > 0.0f) return;

    //SkinMeshの取得.
    if (player->GetAttachMesh().expired()) return;
    //dynamic_pointer_castを使用してSkinMeshClassの関数を取得する.
    auto Mesh = std::dynamic_pointer_cast<SkinMesh>(player->GetAttachMesh().lock());
    if (!Mesh) return;

    //発射元ボーン.
    //右の剣.
    std::string BoneName = "blade_r_head";

    //ボーンのワールド位置.
    D3DXVECTOR3 BoneWorldPos;
    if (!Mesh->GetPosFromBone(BoneName.c_str(), &BoneWorldPos))
    {
        return;
    }

    //Playerのワールド行列.
    D3DXMATRIX MatS, MatR, MatT, MatWorld;

    //PlayerのPositionを取得.
    const D3DXVECTOR3& pos = player->GetPosition();
    //PlayerのRotationを取得.
    const D3DXVECTOR3& rot = player->GetRotation();
    //PlayerのScaleを取得.
    const D3DXVECTOR3& scl = player->GetScale();

    //Scale.
    D3DXMatrixScaling(&MatS, scl.x, scl.y, scl.z);
    //Rotation.
    D3DXMatrixRotationYawPitchRoll(&MatR, rot.y, rot.x, rot.z);
    //Position.
    D3DXMatrixTranslation(&MatT, pos.x, pos.y, pos.z);
    MatWorld = MatS * MatR * MatT;

    //ボーンのローカルオフセットを回転させる  
    D3DXVECTOR3 FinalShotPos = BoneWorldPos;

    //弾の向き.
    D3DXVECTOR3 Dir = player->Player_WS(rot.y);
    //正規化.
    D3DXVec3Normalize(&Dir, &Dir);

    //弾生成
    PShotManager::GetInstance()->AddPlayerShot(FinalShotPos, Dir);
    //クールタイム
    m_ShotCoolDown = m_CoolTime;
}

void AttackLong::Exit(Player* player)
{
}
