#include "Player.h"

#include "System//00_Manager//00_SkinMeshManager//SkinMeshManager.h"

#include "System//00_Manager//02_PShotManager//PShotManager.h"

#include "System//02_Singleton//00_Timer//Timer.h"

#include "System//00_Manager//03_ImGuiManager//ImGuiManager.h"

#include "..//01_Player//PlayerState//PlayerState.h"
#include "GameObject//00_SkinMeshObject//Character//01_Player//PlayerState//00_PlayerIdol//PlayerIdol.h"
#include "GameObject//00_SkinMeshObject//Character//01_Player//PlayerContext//PlayerContext.h"
#include "..//01_Player//PlayerState//03_PlayerDead//PlayerDead.h"

#include "..//01_Player//PlayerState//02_PlayerPortalAnim//PlayerPortalAnim.h"
#include "GameObject//01_SpriteObject//00_Shadow//Shadow.h"



Player::Player()
    :Character()
    , m_pShotManager    ( PShotManager::GetInstance() )
    , m_pAttackManager  ( std::make_unique<PlayerAttackManager>(this) )
    , m_pPlayerIdol     ( std::make_shared<PlayerIdol>(this) )
    , m_pPlayerDead     ( std::make_shared<PlayerDead>(this) )
    , m_pPlayerAnim     ( std::make_shared<PlayerPortalAnim>(this) )

    , m_pCurrentState   ( nullptr )

    , m_CaptureTimer    ( 0.0f )

    , m_InitialPosition {}

    , m_pShadow         ( std::make_unique<Shadow>() )

    , m_IsVisible       ( true )

    , m_IsFrozen        ( false )
{
    //プレイヤーのスキンメッシュの設定.
	SkinMesh* raw_mesh = SkinMeshManager::GetInstance()->GetSkinMeshInstance(SkinMeshManager::SkinList::Player);
	auto shared_mesh = std::shared_ptr<SkinMesh>(raw_mesh,[](SkinMesh*){});
	//AttachMesh に shared_ptr を渡す
	AttachMesh(shared_mesh);
    
    //Game開始時にIdolclassに入る.
    m_pCurrentState = m_pPlayerIdol.get();

    if (m_pCurrentState) 
    {
        m_pCurrentState->Enter();
    }
    //初期化.
    Init();
}

Player::~Player()
{
}

void Player::Update()
{
    //プレイヤーが死んだとき.
    if (IsDead())
    {
        if (m_pAnimCtrl) 
        { 
            m_pAnimCtrl->AdvanceTime(m_AnimSpeed, nullptr); 
        }

        Character::Update();
        return; 
    }
    //ポイントをどちらかが取得したら.
    if (m_IsFrozen)
    {
        //見た目だけ動かす.
        Character::Update();
        return;
    }

    //ToDo : 最初に書いておかないと、ポータルを触った時に弾が進まない.
    m_pShotManager->Update();

    //ポータル取得アニメーション再生中.
    if (IsCapturingState())
    {
        //タイマーの更新.
        m_CaptureTimer -= Timer::GetInstance().DeltaTime();

        if (m_CaptureTimer <= 0.0f)
        {
            //タイマー終了: StateをIdolに戻す.
            PlayerState* nextState = m_pPlayerIdol.get();
            
            if (m_pCurrentState) 
            {
                m_pCurrentState->Exit(); // 古いステート(PortalAnim)の終了処理
            }
            m_pCurrentState = nextState; // ステートを切り替え
            if (m_pCurrentState) 
            {
                m_pCurrentState->Enter(); // 新しいステート(Idol)の開始処理
            }
            m_CaptureTimer = 0.0f;
        }
        return;
    }



    //プレイヤーの攻撃マネージャーの更新.
    m_pAttackManager->Update();

    //プレイヤーステートの更新.
    m_pCurrentState->Update();

    //アニメーション更新
    m_pAnimCtrl->AdvanceTime(m_AnimSpeed, nullptr);

    //基底クラスの更新処理.
    Character::Update();


    D3DXVECTOR3 neckPos;
    if (GetBonePosition("Bone002", &neckPos))
    {
        m_BSphere.SetPosition(neckPos);
    }
    else
    {
        // ボーンが取れない場合のフォールバック（モデルの中心＋オフセット）
        m_BSphere.SetPosition(m_Position + m_HitCenterOffset);
    }

    m_pMesh->GetPosFromBone("blade_l_head", &m_BonePos);
    m_pShadow->Update();

}

void Player::Draw()
{
    m_pAttackManager->Draw();
    //ToDo : 影の表示は先.
    m_pShadow->Draw();
	Character::Draw();
#ifdef _DEBUG
    // プレイヤーのスフィアを明示的に描画
    m_BSphere.Draw();

    ImGui::Begin(JAPANESE("playerのポジションを変更"));
    ImGui::InputFloat3("pos", m_Position);
    D3DXVECTOR3 bPos = m_BSphere.GetPostion();
    ImGui::Text("Sphere Pos: %.2f, %.2f, %.2f", bPos.x, bPos.y, bPos.z);
    ImGui::End();
#endif

#ifdef _DEBUG
    ImGui::Begin(JAPANESE("playerのポジションを変更"));
    ImGui::InputFloat3("pos", m_Position);
    ImGui::End();
#endif

}

void Player::Init()
{
    //Playerのサイズの変更.
    SetScale(D3DXVECTOR3(0.05f, 0.05f, 0.05f));
    //SetScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));
    //Playerの初期位置の変更.
    m_InitialPosition = D3DXVECTOR3(-20.f, 0.0f, -15.f);
    SetPosition(m_InitialPosition);
    

    //Playerの体力.
    m_HitPoint = 100.0f;

    //アニメーションの速度.
    m_AnimSpeed = 1.0f / 60.0f;
    //当たり判定のサイズ
    m_BSphere.SetRadius(0.7f);

    //当たり判定の位置を変更.
    m_BSphere.SetRadius(0.5f);
    m_HitCenterOffset = D3DXVECTOR3(0.0f, 2.5f, 0.0f);

    //影の処理.
    m_pShadow->Create();
    m_pShadow->SetTargetShadowPos(this);

    Character::Init();
}

void Player::Hit()
{
    //ダメージ系統.
    constexpr float zero = 0.0f;
    constexpr float ten = 10.0f;
    //Playerのダメージ実装用のローカル変数.
    constexpr float Dead_HP = zero;
    constexpr float Damege_Hit = ten;

    //くらった時のHPの減少.
    m_HitPoint -= Damege_Hit;
    if (m_HitPoint <= Dead_HP)
    {
        //ここでPlayerのHPが0になったら死亡ステートを呼ぶ.
        m_HitPoint = Dead_HP;

        //死んだときのステートへ移動.
        PlayerState* nextState = m_pPlayerDead.get();

        if (m_pCurrentState != nextState)
        {
            if (m_pCurrentState) 
            {
                m_pCurrentState->Exit();    //古いステートの終了処理
            }
            m_pCurrentState = nextState;    //ステートを切り替え
            if (m_pCurrentState) 
            {
                m_pCurrentState->Enter();   //新しいステートの開始処理
            }
            //死んだのでプレイヤーを非表示.
            SetVisible(false);
        }
    }
}

D3DXVECTOR3 Player::GetHitCenter() const
{
    //プレイヤーモデルの位置 (m_Position) にオフセットを加算して返す
    return m_BSphere.GetPostion();;
}

void Player::InitializePlayerMove()
{
    //ToDo : 遠距離攻撃の動作バグの修正.
    m_pAttackManager->CleanUpState(PlayerAttackManager::enAttack::Long);
    m_pPlayerIdol->Init();
}

void Player::Respawn()
{
    //初期位置に戻す.
    SetPosition(m_InitialPosition);

    //ステートをIdolに戻す.
    if (m_pCurrentState)
    {
        m_pCurrentState->Exit();
    }
    m_pCurrentState = m_pPlayerIdol.get();
    if (m_pCurrentState) 
    {
        m_pCurrentState->Enter(); 
    }
    
    //パラメータのリセット.
    m_HitPoint = 100.0f;
    //プレイヤーを表示する.
    SetVisible(true);

    m_CaptureTimer = 0.0f;

    //攻撃マネージャー等の初期化
    InitializePlayerMove();
}

D3DXVECTOR3 Player::Player_WS(float RotationY) const
{
    D3DXVECTOR3 dir;
    dir.x = sinf(RotationY);
    dir.y = 0.0f;
    dir.z = cosf(RotationY);
    return dir;
}

D3DXVECTOR3 Player::Player_AD(float RotationY) const
{
    D3DXVECTOR3 dir;
    dir.x = cosf(RotationY);
    dir.y = 0.0f;
    dir.z = -sinf(RotationY);
    return dir;
}

void Player::ChangeAttackType(
    PlayerAttackManager::enAttack type)
{
    //プレイヤーの攻撃状態の変更.
    if (m_pAttackManager)
    {
        m_pAttackManager->ChangeAttackState(type);
    }
}

bool Player::GetBonePosition(const char* boneName, D3DXVECTOR3* outPos) const
{
    if (m_pMesh)
    {
        return m_pMesh->GetPosFromBone(boneName, outPos);
    }
    return false;
}

D3DXVECTOR3 Player::GetShortAttackCenter() const
{
    D3DXVECTOR3 center = m_BonePos;

    // 剣の先端方向へ 0.3 くらい前に出す（Z 方向はモデルの向きで変化）
    D3DXVECTOR3 offset(0.f, 0.f, 0.3f);

    // プレイヤーの回転に合わせて offset を回転
    D3DXMATRIX rot;
    D3DXMatrixRotationY(&rot, m_Rotation.y);
    D3DXVec3TransformCoord(&offset, &offset, &rot);

    return center + offset;
}

AttackShort* Player::GetShortAttackState() const
{
    if (m_pAttackManager)
    {
        return m_pAttackManager->GetCurrentShortAttack();
    }
    return nullptr;
}

void Player::SetCaptureState(float duration)
{
    PlayerState* nextState = m_pPlayerAnim.get();
    if (m_pCurrentState == nextState)
    {
        return;
    }

    //タイマーをセット.
    m_CaptureTimer = duration;

    if (m_pCurrentState) 
    {
        m_pCurrentState->Exit();    // 古いステートの終了処理
    }
    m_pCurrentState = nextState;    // ステートを切り替え
    if (m_pCurrentState) 
    {
        m_pCurrentState->Enter();   // 新しいステートの開始処理（PortalAnim::Enter() が呼ばれる）
    }
}

bool Player::IsCapturingState() const
{
    return m_pCurrentState == m_pPlayerAnim.get();
}
