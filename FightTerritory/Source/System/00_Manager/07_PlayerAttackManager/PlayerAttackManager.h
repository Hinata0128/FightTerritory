#pragma once
#include "..//ManagerBase.h"
#include <map>
#include <string>

#include "GameObject/00_SkinMeshObject/Character/01_Player/PlayerAttckStateBase/PlayerAttckStateBase.h" 
class AttackLong;
class AttackShort;
class NoAttack;
class Player;

/***************************************************************************
*	Playerの攻撃(Long/Short)ManagerClass
*	Managerで状態を取得してPlayerMoveStateに呼ぶ.
*	PlayerAttackManagerClassは、ポリモーフィズムを使用して作成していく.
**/

class PlayerAttackManager
	: public ManagerBase
{
public:
	//ここにポリモーフィズムに必要なコードを作成する.
	//列挙型で名前を書く.
	enum class enAttack : byte
	{
		//遠距離攻撃.
		Long = 0, 
		//近距離攻撃.
		Short,

		//何も攻撃していない.
		NoAttack,

		Cooldown,

		max,
	};
public:
	PlayerAttackManager(Player* pOwner);
	~PlayerAttackManager() override;

	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;

	void CleanUpState(enAttack en);

	//攻撃のStateの切り替え
	void ChangeAttackState(enAttack tyep);

	AttackShort* GetCurrentShortAttack() const;
private:
	Player* m_pOwner;
	PlayerAttckStateBase* m_pCurrentAttackState;

	// この map の定義時に PlayerAttackStateBase の完全な情報が必要
	std::map <enAttack,std::unique_ptr<PlayerAttckStateBase>> m_StateMap;
	//Stateの登録関数.
	void RegisterStates();
};