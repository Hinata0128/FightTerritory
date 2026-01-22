#include "BossDead.h"

#include "..//..//Boss.h"
#include "..//..//00_BossContext/BossContext.h"

BossDead::BossDead(Boss* pOwner)
	: BossStateBase(pOwner)
{
}

BossDead::~BossDead()
{
}

void BossDead::Enter()
{
	BossContext ctx(m_pOwner);

	ctx.Mesh->SetAnimSpeed(ctx.AnimSpeed);

	ctx.AnimNo = 1;
	ctx.AnimTime = 0.0;
	ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);

	BossStateBase::Enter();
}

void BossDead::Update()
{
	BossStateBase::Update();
}

void BossDead::Exit()
{
	BossStateBase::Exit();
}

void BossDead::Draw()
{
	BossStateBase::Draw();
}

void BossDead::Init()
{
	BossStateBase::Init();
}
