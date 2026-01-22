//#include "BossMove.h"
//
//#include "..//..//00_BossContext/BossContext.h"
//#include "System/02_Singleton/00_Timer/Timer.h"
//#include "..//..//Boss.h" 
//#include <algorithm> // std::atan2 を使うため
//
//// pOwner を取るコンストラクタ
//BossMove::BossMove(Boss* pOwner)
//	: BossStateBase(pOwner)
//	, m_RotationSpeed(0.2f)
//	, m_RotationDirection(1.0f)
//	, m_RotationAngle(0.0f)
//	, m_DirectionToPlayer(0.0f, 0.0f, 0.0f)
//
//	, MIN_DISTANCE_SQ(0.01f * 0.01f)
//	, ENEMY_NOMAL_RADIUS(10.0f)
//	, SPECIFIED_RANGE_ANGLE(D3DX_PI / 4.0f)
//	, MOVE_SPEED(3.0f)
//
//	, m_RunList	(enRunAnimation::none)
//{
//}
//
//BossMove::~BossMove()
//{
//}
//
//void BossMove::Enter()
//{
//	BossStateBase::Enter();
//}
//
//void BossMove::Update()
//{
//
//	Boss* pEnemy = dynamic_cast<Boss*>(m_pOwner);
//	if (!pEnemy) return;
//
//	float deltaTime = Timer::GetInstance().DeltaTime();
//
//
//	BossContext ctx(m_pOwner);
//
//
//	const D3DXVECTOR3& EnemyPos = pEnemy->GetPosition();
//	const D3DXVECTOR3& PlayerPos = pEnemy->GetPlayerPos();
//
//	//デルタタイムを使用して回転角度を更新させる
//	float deltaAngle = m_RotationSpeed * deltaTime * m_RotationDirection;
//	m_RotationAngle += deltaAngle;
//
//	//角度が半径の範囲を超えたら回転方向を反転させる
//	if (abs(m_RotationAngle) >= SPECIFIED_RANGE_ANGLE)
//	{
//		m_RotationDirection *= -1.0f;
//	}
//
//	//回転行列でオフセットベクトルを計算
//	D3DXMATRIX RotatedMatrix;
//	D3DXMatrixRotationY(&RotatedMatrix, m_RotationAngle);
//
//	D3DXVECTOR3 InitialVec(0.0f, 0.0f, 1.0f);
//	D3DXVECTOR3 RotatedVec;
//	D3DXVec3TransformNormal(&RotatedVec, &InitialVec, &RotatedMatrix);
//	D3DXVec3Normalize(&RotatedVec, &RotatedVec);
//
//	//敵の向きを計算し、m_DirectionToPlayer と敵の Y 軸回転を更新
//	D3DXVECTOR3 Direction;
//	D3DXVec3Subtract(&Direction, &PlayerPos, &EnemyPos);
//	D3DXVec3Normalize(&m_DirectionToPlayer, &Direction);
//
//	float dx = Direction.x;
//	float dz = Direction.z;
//	float Angle_Radian = std::atan2(dx, dz) + D3DX_PI;
//	pEnemy->SetRotationY(Angle_Radian);
//
//	// ------------------------------------
//	// 敵の位置を円軌道状に更新する処理.
//	// ------------------------------------
//	//ベクトルの計算.
//	D3DXVECTOR3 offsetVec;
//	D3DXVec3Scale(&offsetVec, &RotatedVec, ENEMY_NOMAL_RADIUS);
//
//	//新しい位置を計算:プレイヤーの位置+オフセットで行う.
//	D3DXVECTOR3 newEnemyPos;
//	D3DXVec3Add(&newEnemyPos, &PlayerPos, &offsetVec);
//
//	//=========================================================
//	// 円軌道上のターゲット位置へゆっくりと移動する.
//	//=========================================================
//	//差分ベクトルを計算
//	D3DXVECTOR3 MoveDirection;
//	D3DXVec3Subtract(&MoveDirection, &newEnemyPos, &EnemyPos);
//
//	//距離チェック
//	float DistanceSq = D3DXVec3LengthSq(&MoveDirection);
//
//	if (DistanceSq > MIN_DISTANCE_SQ)
//	{
//		//移動ベクトルを正規化
//		D3DXVec3Normalize(&MoveDirection, &MoveDirection);
//
//		//1フレームでの移動量を計算
//		D3DXVECTOR3 MoveStep;
//		D3DXVec3Scale(&MoveStep, &MoveDirection, MOVE_SPEED * deltaTime);
//
//		//現在の位置に移動量を加算
//		pEnemy->AddPosition(MoveStep);
//	}
//
//	// 右・左移動のアニメ番号
//	const int RIGHT_RUN_ANIM = 8;
//	const int LEFT_RUN_ANIM = 9;
//
//	// 回転方向で判定
//	//自分たちから見てのアニメーション
//	if (m_RotationDirection > 0)
//	{
//		// → 右移動アニメ
//		if (ctx.AnimNo != LEFT_RUN_ANIM)
//		{
//			ctx.AnimNo = LEFT_RUN_ANIM;
//			ctx.AnimTime = 0.0f;
//			ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
//		}
//	}
//	else
//	{
//		// → 左移動アニメ
//		if (ctx.AnimNo != RIGHT_RUN_ANIM)
//		{
//			ctx.AnimNo = RIGHT_RUN_ANIM;
//			ctx.AnimTime = 0.0f;
//			ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);
//		}
//	}
//
//
//	BossStateBase::Update();
//}
//
//void BossMove::Exit()
//{
//	BossStateBase::Exit();
//}
//
//void BossMove::Draw()
//{
//	BossStateBase::Draw();
//}
//
//void BossMove::Init()
//{
//	BossStateBase::Init();
//}