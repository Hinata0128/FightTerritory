#include "BossIdol.h"

#include "..//..//Boss.h"
#include "..//..//00_BossContext//BossContext.h"

#include "System/02_Singleton/00_Timer/Timer.h"

#include "GameObject/00_SkinMeshObject/Character//00_Boss/02_Com/Com.h"

BossIdol::BossIdol(Boss* pOwner)
	: BossStateBase(pOwner)
{
}

BossIdol::~BossIdol()
{
}

void BossIdol::Enter()
{
}

void BossIdol::Update()
{
    BossContext ctx(m_pOwner);

    const int IDLE_ANIM_NO = 2;

    // アニメ番号が違うときだけセット（1回だけ実行される）
    if (ctx.AnimNo != IDLE_ANIM_NO)
    {
        ctx.AnimNo = IDLE_ANIM_NO;
        ctx.AnimTime = 0.0f;

        ctx.Mesh->ChangeAnimSet(IDLE_ANIM_NO, ctx.AnimCtrl);
    }

    // アニメーションを前進
    float delta = Timer::GetInstance().DeltaTime();
    ctx.AnimTime += ctx.AnimSpeed * delta;

    // 共通更新
    BossStateBase::Update();

    // 距離で移動ステートへ遷移
    Boss* pEnemy = dynamic_cast<Boss*>(m_pOwner);

    D3DXVECTOR3 diff = pEnemy->GetPlayerPos() - pEnemy->GetPosition();
    float distanceSq = D3DXVec3LengthSq(&diff);

    const float changeDistance = 15.0f * 15.0f;

}

void BossIdol::Exit()
{
    BossStateBase::Exit();
}

void BossIdol::Draw()
{
    BossStateBase::Draw();
}

void BossIdol::Init()
{
    BossStateBase::Init();
}
