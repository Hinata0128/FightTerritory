#include "Renderer.h"

Renderer& Renderer::GetInstance()
{
	static Renderer s_instance;
	return s_instance;
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::SetDrawParameters(const D3DXMATRIX& view, const D3DXMATRIX& proj, const LIGHT& light, const CAMERA& camera)
{
	m_View = view;
	m_Proj = proj;
	m_Light = light;
	m_Camera = camera;
}

const D3DXMATRIX& Renderer::GetView() const
{
	return m_View;
}

const D3DXMATRIX& Renderer::GetProj() const
{
	return m_Proj;
}

const LIGHT& Renderer::GetLight() const
{
	return m_Light;
}

const CAMERA& Renderer::GetCamera() const
{
	return m_Camera;
}