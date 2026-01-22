#include "PlayerMove.h"

#include "..//..//Player.h"
#include "..//..//PlayerContext/PlayerContext.h"

#include "System/02_Singleton/01_Camera/Camera.h"

#include "Sound/SoundManager.h"

//ローカル変数.
constexpr float zero = 0.0f;
constexpr float add_value = 0.1f;

//テストbranch作成しました

PlayerMove::PlayerMove(Player* pOwner)
    : PlayerState   (pOwner)
    , step          (enStep::none)
    , LStep         (enLeftStep::none)
    , Move          (enMove::Idol)
    , m_Key         (std::make_unique<InputKeyManager>())
{
    //初期化を書いている.
    Init();
}

PlayerMove::~PlayerMove()
{
}

void PlayerMove::Enter()
{
    PlayerState::Enter();
}

void PlayerMove::Update()
{
    PlayerContext ctx(m_pOwner);

    float deltaTime = Timer::GetInstance().DeltaTime();
    //右クリックの攻撃の関数.
    if (LStep == enLeftStep::none)
    {
        RbuttonAttackStep(ctx);
    }
    bool RAttack = IsRAttacking;
    if (step == enStep::none)
    {
        LButtonAttackStep(ctx);
    }
    bool LAttacking = IsLAttacking;
    //WASDの入力取得.
    Move = GetMoveInput();

    //PlayerのWASDの動作をさせるための関数.
    D3DXVECTOR3 ForwardAndBackward = m_pOwner->Player_WS(ctx.Rotation.y);
    D3DXVECTOR3 LeftAndRight = m_pOwner->Player_AD(ctx.Rotation.y);

    m_Key->Update();

    //Playerの移動処理.
    //第一引数にPlayerContext.
    //第二引数にWSの関数.
    //第三引数にADの関数.
    HandleMove(ctx, ForwardAndBackward, LeftAndRight);

    //スペースキーで回転をさせる.
    if (m_Key->GetKey("Space") && m_Key->GetKey("Space")->HoldDownKey())
    {
        ctx.Rotation.y += add_value;
    }

    //ステートの共通処理.
    PlayerState::Update();
}


void PlayerMove::Exit()
{
    PlayerState::Exit();
}

void PlayerMove::Draw()
{
    PlayerState::Draw();
}

void PlayerMove::Init()
{
    PlayerContext ctx(m_pOwner);

    // 全ての攻撃状態を強制的に none にする
    step = enStep::none;      // 遠距離リセット
    LStep = enLeftStep::none; // 近距離リセット

    IsRAttacking = false;
    IsLAttacking = false;

    ctx.AnimTime = 0.0f;
    m_IsShot = false;

    // 攻撃タイプもリセット
    m_pOwner->ChangeAttackType(PlayerAttackManager::enAttack::NoAttack);

    PlayerState::Init();
}

//右クリックの遠距離攻撃.
void PlayerMove::RbuttonAttackStep(PlayerContext& ctx)
{
    float deltaTime = Timer::GetInstance().DeltaTime();

    if (ctx.AnimNo == 7)
    {
        if (step != enStep::none)
        {
            step = enStep::none;
            IsRAttacking = false;
            m_pOwner->ChangeAttackType(PlayerAttackManager::enAttack::NoAttack);
        }
        return;
    }

    // 右クリック押されたら初期ステップへ
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
    {
        if (step == enStep::none)
        {
            step = enStep::first;
            IsRAttacking = true;
        }
    }

    switch (step) 
    {
        case enStep::none:
            IsRAttacking = false;
            break; //攻撃していない→移動処理に移動.
        case enStep::first:
            //アニメーション切り替え.
            ctx.AnimNo = 8; //アニメーション番号.
            ctx.AnimTime = 0.0f;    //アニメーションタイマーの初期化.
            m_IsShot = false;
            ctx.Mesh->SetAnimSpeed(ctx.AnimSpeed, ctx.AnimCtrl);
            ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);//アニメーションの変更.
            step = enStep::run;
            break;
        case enStep::run:
        
            //アニメーションの停止を良い位置でさせるため.
            if (ctx.AnimTime > ctx.Mesh->GetAnimPeriod(6))
            {
                step = enStep::end;
            }
            else
            {
                ctx.Mesh->SetAnimSpeed(ctx.AnimSpeed, ctx.AnimCtrl);
            }
            break;
        
        case enStep::end:
        {
            //アニメーションの停止.
            ctx.Mesh->SetAnimSpeed(0.0f, ctx.AnimCtrl);
            m_pOwner->ChangeAttackType(PlayerAttackManager::enAttack::Long);
            if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
            {
                step = enStep::release_anim;
            }
            break;
        }
        case enStep::release_anim:
        {
            ctx.AnimTime += deltaTime * ctx.AnimSpeed;

            ctx.AnimNo = 0;
            ctx.AnimTime = 0.0f;
            ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
            step = enStep::none;

            m_pOwner->ChangeAttackType(PlayerAttackManager::enAttack::NoAttack);


            break;
        }
    }
}

// 左クリックを押したときの近距離攻撃.
void PlayerMove::LButtonAttackStep(PlayerContext& ctx)
{
    if (ctx.AnimNo == 7)
    {
        if (LStep != enLeftStep::none) {
            LStep = enLeftStep::none;
            IsLAttacking = false;
        }
        return;
    }

    // 左クリックの入力判定
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
    {
        if (LStep == enLeftStep::none)
        {
            LStep = enLeftStep::first;
            IsLAttacking = true;
        }
    }

    switch (LStep)
    {
    case enLeftStep::none:
        IsLAttacking = false;
        break;

    case enLeftStep::first:
    {
        // アニメーション切り替え。
        ctx.AnimNo = 6;         // 近接攻撃アニメーション番号。
        ctx.AnimTime = 0.0f;    // ここでタイマーをリセット。

        m_pOwner->ChangeAttackType(PlayerAttackManager::enAttack::Short);

        // アニメーションの変更。
        ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);

        LStep = enLeftStep::Attack;
        break;
    }

    case enLeftStep::Attack:
    {
        double period = ctx.Mesh->GetAnimPeriod(ctx.AnimNo);

        // アニメーション終了判定。
        if (ctx.AnimTime >= period)
        {
            LStep = enLeftStep::end;
        }
        else
        {
            // タイマーを累積。
            ctx.AnimTime += ctx.AnimSpeed;
        }
        break;
    }

    case enLeftStep::end:
    {
        // アニメーションの停止。
        ctx.Mesh->SetAnimSpeed(0.0f, ctx.AnimCtrl);

        LStep = enLeftStep::release_anim;
        break;
    }

    case enLeftStep::release_anim:
    {
        double period = ctx.Mesh->GetAnimPeriod(6);
        if (ctx.AnimTime >= period)
        {
            ctx.AnimNo = 0;
            ctx.AnimTime = 0.0f;
            ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);

            m_IsShot = true;
            LStep = enLeftStep::none;

            m_pOwner->ChangeAttackType(PlayerAttackManager::enAttack::NoAttack);
        }
        else
        {
            ctx.AnimTime += ctx.AnimSpeed;
        }
        break;
    }
    }
}

void PlayerMove::HandleMove(
    PlayerContext& ctx,
    const D3DXVECTOR3& ForwardAndBackward, // 既存の引数
    const D3DXVECTOR3& LeftAndRight)       // 既存の引数
{
    Camera& cam = Camera::GetInstance();
    D3DXVECTOR3 camForward = cam.GetForward();
    D3DXVECTOR3 camRight = cam.GetRight();

    // 攻撃中かどうか（アニメを変更するか判断）
    bool IsRAttacking = (step != enStep::none);
    bool IsLAttacking = (LStep != enLeftStep::none);

    // 移動アニメーションを適応させる
    auto ApplyMoveAnimation = [&](int animNo)
        {
            if (IsRAttacking || IsLAttacking)
            {
                return;
            }
            if (ctx.AnimNo != animNo)
            {
                ctx.AnimNo = animNo;
                ctx.AnimTime = 0.0;
                ctx.Mesh->SetAnimSpeed(ctx.AnimSpeed, ctx.AnimCtrl);
                ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
            }
        };

    switch (Move)
    {
    case enMove::Idol:
    {
        ApplyMoveAnimation(0);
        ctx.AnimTime += ctx.AnimSpeed;
    }
    break;

    case enMove::ForWard:
    {
        ctx.Position += camForward * add_value;
        ctx.Rotation.y = atan2f(camForward.x, camForward.z); // キャラを進行方向に向ける
        ApplyMoveAnimation(2);
        ctx.AnimTime += ctx.AnimSpeed;
        double period = ctx.Mesh->GetAnimPeriod(18);
        if (ctx.AnimTime >= period)
        {
            ctx.Mesh->SetAnimSpeed(0.0, ctx.AnimCtrl);
            ctx.AnimTime += ctx.AnimSpeed;
        }
    }
    break;

    case enMove::Back:
    {
        ctx.Position -= camForward * add_value;
        ctx.Rotation.y = atan2f(-camForward.x, -camForward.z);
        ApplyMoveAnimation(0);
        ctx.AnimTime += ctx.AnimSpeed;
    }
    break;

    case enMove::Left:
    {
        ctx.Position -= camRight * add_value;
        ctx.Rotation.y = atan2f(-camRight.x, -camRight.z);
        ApplyMoveAnimation(2);
        ctx.AnimTime += ctx.AnimSpeed;
        double period = ctx.Mesh->GetAnimPeriod(18);
        if (ctx.AnimTime >= period)
        {
            ctx.Mesh->SetAnimSpeed(0.0, ctx.AnimCtrl);
            ctx.AnimTime += ctx.AnimSpeed;
        }
    }
    break;

    case enMove::Right:
    {
        ctx.Position += camRight * add_value;
        ctx.Rotation.y = atan2f(camRight.x, camRight.z);
        ApplyMoveAnimation(2);
        ctx.AnimTime += ctx.AnimSpeed;
        double period = ctx.Mesh->GetAnimPeriod(18);
        if (ctx.AnimTime >= period)
        {
            ctx.Mesh->SetAnimSpeed(0.0, ctx.AnimCtrl);
            ctx.AnimTime += ctx.AnimSpeed;
        }
    }
    break;

    case enMove::ForWardAndLeft:
    {
        D3DXVECTOR3 dir = camForward - camRight;
        D3DXVec3Normalize(&dir, &dir);
        ctx.Position += dir * add_value;
        ctx.Rotation.y = atan2f(dir.x, dir.z);
        ApplyMoveAnimation(2);
        ctx.AnimTime += ctx.AnimSpeed;
        double period = ctx.Mesh->GetAnimPeriod(18);
        if (ctx.AnimTime >= period) { ctx.Mesh->SetAnimSpeed(0.0, ctx.AnimCtrl); }
    }
    break;

    case enMove::ForWardAAndRight:
    {
        D3DXVECTOR3 dir = camForward + camRight;
        D3DXVec3Normalize(&dir, &dir);
        ctx.Position += dir * add_value;
        ctx.Rotation.y = atan2f(dir.x, dir.z);
        ApplyMoveAnimation(2);
        ctx.AnimTime += ctx.AnimSpeed;
        double period = ctx.Mesh->GetAnimPeriod(18);
        if (ctx.AnimTime >= period) { ctx.Mesh->SetAnimSpeed(0.0, ctx.AnimCtrl); }
    }
    break;

    case enMove::BackAndLeft:
    {
        D3DXVECTOR3 dir = -camForward - camRight;
        D3DXVec3Normalize(&dir, &dir);
        ctx.Position += dir * add_value;
        ctx.Rotation.y = atan2f(dir.x, dir.z);
        ApplyMoveAnimation(0);
        ctx.AnimTime += ctx.AnimSpeed;
    }
    break;

    case enMove::BackAndRight:
    {
        D3DXVECTOR3 dir = -camForward + camRight;
        D3DXVec3Normalize(&dir, &dir);
        ctx.Position += dir * add_value;
        ctx.Rotation.y = atan2f(dir.x, dir.z);
        ApplyMoveAnimation(0);
        ctx.AnimTime += ctx.AnimSpeed;
    }
    break;
    }
}
//Playerの動作[InputKeyManagerを使用して]書く関数.
PlayerMove::enMove PlayerMove::GetMoveInput()
{
    // WASDの動作.
    bool W = (m_Key->GetKey("W") && m_Key->GetKey("W")->HoldDownKey());
    bool A = (m_Key->GetKey("A") && m_Key->GetKey("A")->HoldDownKey());
    bool S = (m_Key->GetKey("S") && m_Key->GetKey("S")->HoldDownKey());
    bool D = (m_Key->GetKey("D") && m_Key->GetKey("D")->HoldDownKey());

    //斜め優先判定.
    if (W && D) return enMove::ForWardAAndRight;
    if (W && A) return enMove::ForWardAndLeft;
    if (S && D) return enMove::BackAndRight;
    if (S && A) return enMove::BackAndLeft;

    //単方向判定.
    if (W) return enMove::ForWard;
    if (S) return enMove::Back;
    if (A) return enMove::Left;
    if (D) return enMove::Right;

    //どのキーも押していないときは、0番目のアニメーションを再生.
    return enMove::Idol;
}