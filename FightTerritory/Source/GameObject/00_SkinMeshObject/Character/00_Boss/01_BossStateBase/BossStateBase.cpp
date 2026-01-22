#include "BossStateBase.h"
#include "..//Boss.h"

BossStateBase::BossStateBase(Boss* pOwner)
	: StateBase	()
	, m_pOwner	(pOwner)
{
}

BossStateBase::~BossStateBase()
{
}

void BossStateBase::Enter()
{
}

void BossStateBase::Update()
{
}

void BossStateBase::Exit()
{
}

void BossStateBase::Draw()
{
}

void BossStateBase::Init()
{
}

Boss* BossStateBase::GetOwner() const
{
	return m_pOwner;
}
