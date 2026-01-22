#include "ShotBase.h"

ShotBase::ShotBase()
	: m_Speed(0.02f)
	, m_Disp(false)
	, m_IsActive(false)
	, m_BSphere(std::make_shared<BoundingSphere>())
{
}

ShotBase::~ShotBase()
{
}

void ShotBase::Update()
{
	StaticMeshObject::Update();
}

void ShotBase::Draw()
{
	// 表示中のみ描画
	if (m_Disp)
	{
		StaticMeshObject::Draw();
	}
}

void ShotBase::Init()
{
	m_Position = D3DXVECTOR3(0.0f, -50.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_Disp = false;
	m_IsActive = false;

	if (m_BSphere)
	{
		m_BSphere->SetPosition(m_Position);
	}
}

void ShotBase::UpdateBPosition()
{
	if (m_BSphere)
	{
		m_BSphere->SetPosition(m_Position);
	}
}

BoundingSphere& ShotBase::GetBoundingSphere() const
{
	return *m_BSphere;
}