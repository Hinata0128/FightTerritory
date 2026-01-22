//#pragma once
//#include "..//BossStateBase.h"	//基底クラス.
//#include <d3dx9.h> 
//
//class Boss;
//class BossContext;
//
///********************************************************************
//*	NomalMoveClass
//*	攻撃の実装をする.
//**/
//class BossMove final
//	: public BossStateBase
//{
//public:
//	//移動のアニメーション用の列挙.
//	enum class enRunAnimation : byte
//	{
//		none,		//何もしない.
//		Right_Run,	//右の移動.
//		Left_Run,	//左の移動.
//	};
//public:
//	BossMove(Boss* pOwner);
//	~BossMove();
//
//	void Enter() override;
//	void Update() override;
//	void Exit() override;
//
//	void Draw() override;
//	void Init() override;
//
//public:
//	const D3DXVECTOR3& GetDirectionToPlayer() const {
//		return m_DirectionToPlayer;
//	}
//private:
//	//===================================================
//	// EnemyNomalに書いてあったメンバ変数(Moveで使う用).
//	//===================================================
//	float m_RotationSpeed;
//	float m_RotationDirection;
//	float m_RotationAngle;
//	D3DXVECTOR3 m_DirectionToPlayer;
//
//	//移動ロジックで使用する定数
//	float MIN_DISTANCE_SQ;
//	float ENEMY_NOMAL_RADIUS;
//	float SPECIFIED_RANGE_ANGLE;
//	float MOVE_SPEED;
//
//	enRunAnimation m_RunList;
//};