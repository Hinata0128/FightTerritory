#include "SceneBase.h"

SceneBase::SceneBase()
	: m_pMesh		(nullptr)
	, m_pDx11		(DirectX11::GetInstance())
	, m_pDx9		(DirectX9::GetInstance())

	, m_hWnd		(nullptr)
	, m_Score		(0)
	, m_Count		(0)
	, m_Timer		(0.0f)
	
	, m_InputTimer	(0.0f)
	, m_FadeAlpha	(0.0f)
	, m_FadeSpeed	(1.5f)
{
}

void SceneBase::PreDraw()
{
	Renderer::GetInstance().SetDrawParameters(
		m_mView,
		m_mProj,
		m_Light,
		m_Camera);
}
