#include "PlayerState.h"

#include "..//Player.h"

//ƒ[ƒJƒ‹•Ï”.
constexpr float zero = 0.0f;

PlayerState::PlayerState(Player* pOwner)
	: StateBase	()
	, m_pOwner	(pOwner)
{
}

PlayerState::~PlayerState()
{
}

void PlayerState::Enter()
{
}

void PlayerState::Update()
{
}

void PlayerState::Exit()
{
}

void PlayerState::Draw()
{
}

void PlayerState::Init()
{
}



Player* PlayerState::GetOwner() const
{
    return m_pOwner;
}
