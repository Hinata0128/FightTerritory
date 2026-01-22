#include "PlayerAttackManager.h"

#include "GameObject/00_SkinMeshObject/Character/01_Player/Player.h"

#include "GameObject/00_SkinMeshObject/Character/01_Player/PlayerAttckStateBase/AttackLong/AttackLong.h" 
#include "GameObject/00_SkinMeshObject/Character/01_Player/PlayerAttckStateBase/AttackShort/AttackShort.h"
#include "GameObject/00_SkinMeshObject/Character/01_Player/PlayerAttckStateBase/NoAttack/NoAttack.h"

// ---------------------------------------------------------------------

PlayerAttackManager::PlayerAttackManager(Player* pOwner)
    : ManagerBase()
    , m_pOwner(pOwner)
    , m_pCurrentAttackState(nullptr) 
{
    RegisterStates();
    ChangeAttackState(enAttack::Short);
}

PlayerAttackManager::~PlayerAttackManager()
{
}

// ---------------------------------------------------------------------

void PlayerAttackManager::Update()
{
    if (m_pCurrentAttackState && m_pOwner)
    {
        m_pCurrentAttackState->ExecuteAttack(m_pOwner);
    }
    ManagerBase::Update();
}

void PlayerAttackManager::Draw()
{
    if (m_pCurrentAttackState && m_pOwner)
    {
        if (auto pShort = dynamic_cast<AttackShort*>(m_pCurrentAttackState))
        {
            //AttackShortに固有のDraw関数を呼び出す
            pShort->Draw(m_pOwner); 
        }
    }
    ManagerBase::Draw();
}

void PlayerAttackManager::Create()
{
    ManagerBase::Create();
}

void PlayerAttackManager::Release()
{
    m_StateMap.clear();
    ManagerBase::Release();
}

void PlayerAttackManager::CleanUpState(enAttack en)
{
    m_StateMap[en]->Enter(m_pOwner);
}

// ---------------------------------------------------------------------

void PlayerAttackManager::ChangeAttackState(enAttack type)
{
    if (m_StateMap.count(type) == 0) return;

    PlayerAttckStateBase* nextState = m_StateMap.at(type).get();

    if (m_pCurrentAttackState != nextState)
    {
        if (m_pCurrentAttackState) {
            m_pCurrentAttackState->Exit(m_pOwner);
        }

        m_pCurrentAttackState = nextState;

        if (m_pCurrentAttackState) {
            m_pCurrentAttackState->Enter(m_pOwner);
        }
    }
}

AttackShort* PlayerAttackManager::GetCurrentShortAttack() const
{
    //enAttack::Shortが存在するか確認
    if (m_StateMap.count(enAttack::Short) == 0)
    {
        return nullptr;
    }

    PlayerAttckStateBase* baseState = m_StateMap.at(enAttack::Short).get();

    return dynamic_cast<AttackShort*>(baseState);
}
// ---------------------------------------------------------------------

void PlayerAttackManager::RegisterStates()
{
    m_StateMap.emplace(enAttack::Long, std::make_unique<AttackLong>());
    m_StateMap.emplace(enAttack::Short, std::make_unique<AttackShort>());
    m_StateMap.emplace(enAttack::NoAttack, std::make_unique<NoAttack>());
}