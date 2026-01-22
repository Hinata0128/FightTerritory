#pragma once

#include <iostream>
#include <D3DX9.h>


/************************************************************************************
*	レンダラークラス.
**/
class Renderer
{
public:
	static Renderer& GetInstance();

	virtual ~Renderer();

	void SetDrawParameters(const D3DXMATRIX& view, const D3DXMATRIX& proj, const LIGHT& light, const CAMERA& camera);

	const D3DXMATRIX& GetView() const;
	const D3DXMATRIX& GetProj() const;
	const LIGHT& GetLight() const;
	const CAMERA& GetCamera() const;

private:
	Renderer();

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(Renderer&&) = delete;

private:
	D3DXMATRIX	m_View;
	D3DXMATRIX	m_Proj;

	CAMERA		m_Camera;
	LIGHT		m_Light;
};