#include "Boss.h"
#include "System/00_Manager/00_SkinMeshManager/SkinMeshManager.h"
#include "System/00_Manager/04_BossShotManager/BossShotManager.h"
#include "System/02_Singleton/00_Timer/Timer.h"
#include "..//00_Boss/00_BossContext/BossContext.h"
#include "..//00_Boss/01_BossStateBase/BossStateBase.h"

#include "GameObject/01_SpriteObject/00_Shadow/Shadow.h"

#include "SceneManager/SceneManager.h"

constexpr float zero = 0.0f;

Boss::Boss(std::shared_ptr<Portal> pPortal)
    : Character()


    , m_Difficulty(BossDifficulty::Easy)

    , m_MoveSpeed(1.5f)

    , m_ShotInterval(0.0f)
    , m_DefenseRadius(0.0f)

    , m_PressureShotInterval(0.0f)

    , m_IsCapturingPortal(false)

    , m_pENShotManager(nullptr)
    
    , m_pIdol(std::make_unique<BossIdol>(this))
    , m_pDead(std::make_unique<BossDead>(this))
    , m_pBossAnim(std::make_unique<BossPortalAnim>(this))

    
    , m_InitialPosition {}

    , m_CurrentRound    (1) //開始はラウンド1からのため.

    , m_IsVisible   (true)

    , m_IsFrozen    (false)

    , m_pShadow     ( std::make_shared<Shadow>() )
    , m_pPlayer     ( std::make_unique<Player>() )
    , m_CoolTime(1.0f)       
    , m_ShotCoolDown(0.0f)  

    , m_CaptureTimer(0.0f)

{
    SkinMesh* raw_mesh = SkinMeshManager::GetInstance()->GetSkinMeshInstance(SkinMeshManager::SkinList::Enemy);
    auto shared_mesh = std::shared_ptr<SkinMesh>(raw_mesh, [](SkinMesh*) {});
    AttachMesh(shared_mesh);

    // サイズと位置
    SetScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));

    m_InitialPosition = D3DXVECTOR3(20.0f, 0.f, 15.f);
    //m_InitialPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    SetPosition(m_InitialPosition);
    //SetPosition(0.0f, 0.0f, 0.0f);

    // ステートセット
    m_pCurrentState = m_pIdol.get();
    if (m_pCurrentState)
    {
        m_pCurrentState->Enter();
    }

    //アニメーションの再生速度.
    m_AnimSpeed = 1.0f / 60.0f;

    m_pENShotManager = BossShotManager::GetInstance();

    m_BSphere.SetRadius(1.0f);
    m_HitCenterOffset = D3DXVECTOR3(0.0f, 3.5f, 0.0f);

    Init();
}

Boss::~Boss()
{
}

void Boss::Init()
{
    m_HitPoint = 100.0f;

    int score = SceneManager::GetInstance()->GetPlayerScore() + SceneManager::GetInstance()->GetEnemyScore();
    m_CurrentRound = score + 1;

    //待機アニメーションを再生.
    const int IDLE_ANIM = 0;
    if (m_pMesh && m_pAnimCtrl)
    {
        m_pMesh->ChangeAnimSet(IDLE_ANIM, m_pAnimCtrl);
        m_AnimNo = IDLE_ANIM;
        m_pMesh->SetAnimSpeed(m_AnimSpeed);
    }

    m_pShadow->Create();
    m_pShadow->SetTargetShadowPos(this);

    Character::Init();
}

void Boss::Update()
{
    float deltaTime = Timer::GetInstance().DeltaTime();

    if (m_IsFrozen)
    {
        Character::Update();
        return;
    }

    m_pENShotManager->Update();

    //ポータル取得アニメーションを再生させる.
    if (IsCapturingState())
    {
        //タイマーの更新.
        m_CaptureTimer -= Timer::GetInstance().DeltaTime();
        if (m_CaptureTimer <= 0.0f)
        {
            //タイマー終了 : StateをIdolに戻す.
            BossStateBase* nextState = m_pIdol.get();
            if (m_pCurrentState)
            {
                m_pCurrentState->Exit();
            }
            m_pCurrentState = nextState;
            if (m_pCurrentState)
            {
                m_pCurrentState->Enter();
            }
            m_CaptureTimer = 0.0f;
        }
        return;
    }

    // クールタイム
    m_ShotCoolDown -= deltaTime;

    if (m_ShotCoolDown < 0.0f)
    {
        m_ShotCoolDown = 0.0f;
    }
    
    if (!IsDaed())
    {
        //ToDo : 壁の配置をするためボスは今動かないようにしておく.
        //m_pCom->Update();
    }

    // ボーン座標取得
    m_pMesh->GetPosFromBone("boss_head", &m_BonePos);

    m_pShadow->Update();

    Character::Update();
}

void Boss::Draw()
{
    m_pMesh->SetAnimSpeed(m_AnimSpeed);
    m_pShadow->Draw();
    Character::Draw();
}

void Boss::Hit()
{
    m_HitPoint -= 10.0f;

    if (m_HitPoint <= 0.0f)
    {
        m_HitPoint = 0.0f;

        BossStateBase* nextState = m_pDead.get();

        if (m_pCurrentState != nextState)
        {
            if (m_pCurrentState) m_pCurrentState->Exit();
            m_pCurrentState = nextState;
            if (m_pCurrentState) m_pCurrentState->Enter();

            SetVisible(false);
        }
    }
}

bool Boss::CanShot() const
{
    return m_ShotCoolDown <= 0.0f;
}

void Boss::RequestShot()
{
    if (!CanShot())
    {
        return;
    }

    AutoShot();
    //クールタイムのリセット.
    //ToDo : 攻撃できるように.
    m_ShotCoolDown = m_CoolTime;
}

void Boss::SetShotInterval(float time)
{
    m_CoolTime = time;
}

void Boss::StartNextRound(int nextround)
{
    m_CurrentRound = nextround;

    Respawn();
}

void Boss::Respawn()
{
    SetPosition(m_InitialPosition);
    m_HitPoint = 100.0f;

    //状態をIdolに戻す.
    m_pCurrentState = m_pIdol.get();
    if (m_pCurrentState)
    {
        m_pCurrentState->Enter();
    }

    //アニメーションを強制的に初期化
    const int IDLE_ANIM = 0;
    if (m_pMesh && m_pAnimCtrl)
    {
        m_pMesh->ChangeAnimSet(IDLE_ANIM, m_pAnimCtrl);
        m_AnimNo = IDLE_ANIM;
        m_pMesh->SetAnimSpeed(m_AnimSpeed);
    }

    SetVisible(true);

    m_CaptureTimer = 0.0f;

    Init();
}

D3DXVECTOR3 Boss::GetHitCenter() const
{
    return m_Position + m_HitCenterOffset;
}


void Boss::ChangeState(BossStateBase* state)
{
    if (m_pCurrentState == state) return;

    if (m_pCurrentState) m_pCurrentState->Exit();

    m_pCurrentState = state;

    if (m_pCurrentState) m_pCurrentState->Enter();
}

void Boss::AutoShot()
{
    if (!m_pPlayer) return;

    //距離チェック
    D3DXVECTOR3 playerPos = GetPlayerPos();
    D3DXVECTOR3 toPlayer = playerPos - GetPosition();
    float distance = D3DXVec3Length(&toPlayer);

    constexpr float SHOOT_DISTANCE = 20.0f;

    if (distance <= SHOOT_DISTANCE)
    {
        //発射位置
        D3DXVECTOR3 shotPos = GetPosition() + D3DXVECTOR3(0.0f, 3.0f, 0.0f);

        //狙うターゲットの位置
        D3DXVECTOR3 targetPos;
        if (!m_pPlayer->GetBonePosition("Bone002", &targetPos))
        {
            //ボーンが取得できなかった時の保険
            targetPos = playerPos + D3DXVECTOR3(0.0f, 2.0f, 0.0f);
        }

        //方向ベクトルの計算
        D3DXVECTOR3 finalDir = targetPos - shotPos;
        float len = D3DXVec3Length(&finalDir);

        //ゼロ除算.
        if (len <= 0.001f)
        {
            m_pENShotManager->Clear();
            return; // 弾は発射せずに終了   
        }
        D3DXVec3Normalize(&finalDir, &finalDir);
        m_pENShotManager->AddEnemyNomalShot(shotPos, finalDir);
    }
}

void Boss::SetCaptureState(float duration)
{
    BossStateBase* nextState = m_pBossAnim.get();
    if (m_pCurrentState == nextState)
    {
        return;
    }

    //タイマーをセット.
    m_CaptureTimer = duration;

    if (m_pCurrentState)
    {
        m_pCurrentState->Exit();    //古いステートの終了処理.
    }
    m_pCurrentState = nextState;    //ステートをBossPortalAnimに切り替える.
    if (m_pCurrentState)
    {
        //ToDo : ボスのポータル取得アニメーションクラスに入ってアニメーションの再生をさせる.
        m_pCurrentState->Enter();   //新しいステートの開始.
    }
}

bool Boss::IsCapturingState() const
{
    return m_pCurrentState == m_pBossAnim.get();
}

//ラウンドごとに難易度を設定.
void Boss::DecideDifficltyByRound(float raund)
{
    //確率を使用して難易度を設定させる.
    int Raund = rand() % 100;

    //ラウンドごとに難易度を設定.
    //1ラウンド目.
    if (raund == 1)
    {
        if (Raund < 90)
        {
            SetDifficulty(BossDifficulty::Easy);
        }
        else
        {
            SetDifficulty(BossDifficulty::Hard);
        }
    }
    //2ラウンド目.
    else if (raund == 2)
    {
        if (Raund < 70)
        {
            SetDifficulty(BossDifficulty::Hard);
        }
        else
        {
            SetDifficulty(BossDifficulty::Easy);
        }
    }
    //3ラウンド目.
    else if (raund == 3)
    {
        SetDifficulty(BossDifficulty::Final);
    }
}

//難易度用のSet関数.
void Boss::SetDifficulty(BossDifficulty diff)
{
    m_Difficulty = diff;
    ApplyDifficultyParam();
}

//ボスの行動調整関数.
void Boss::ApplyDifficultyParam()
{
    //それぞれの難易度の移動速度等を変更.
    switch (m_Difficulty)
    {
    //Easyモードのパラメータ.
    case Boss::BossDifficulty::Easy:
        m_MoveSpeed = 1.2f;
        m_ShotInterval = 5.0f;
        m_PressureShotInterval = 2.5f;
        m_DefenseRadius = 6.0f;
        break;
    //Hardモードのパラメータ.
    case Boss::BossDifficulty::Hard:
        m_MoveSpeed = 3.5f;
        m_ShotInterval = 2.0f;
        m_PressureShotInterval = 1.5f;
        m_DefenseRadius = 8.0f;
        break;
    //Finalモードのパラメータ.
    case Boss::BossDifficulty::Final:
        m_MoveSpeed = 5.0f;
        m_ShotInterval = 1.0f;
        m_PressureShotInterval = 0.5f;
        m_DefenseRadius = 10.0f;
        break;
    default:
        break;
    }
}

void Boss::DecideAction()
{
}

void Boss::MoveToPortal()
{
}

void Boss::PortalMoveEasy()
{
}

void Boss::PortalMoveHard()
{
}

void Boss::PortalMoveFinal()
{
}

void Boss::AttackEasy()
{
}

void Boss::AttackHard()
{
}

void Boss::AttackFinal()
{
}

void Boss::DefenseEasy()
{
}

void Boss::DefenseHard()
{
}

void Boss::DefenseFinal()
{
}
