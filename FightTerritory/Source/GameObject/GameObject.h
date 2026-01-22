#pragma once

#include "System//01_Renderer//Renderer.h"

/**************************************************************
*	ゲームオブジェクトクラス.
*	ToDo : 継承元のクラスで使用するので純粋仮想関数を使用している.
*		   ポジション等の設定は、このクラスで書いている.
**/
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	//処理は子クラスに任せる.
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//座標設定関数.
	void SetPosition( float x, float y, float z ){
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
	};
	//x,y,zをうたなくてもいい関数.
	void SetPosition(const D3DXVECTOR3& pos)
	{
		m_Position = pos;
	}

	//座標取得関数.
	const D3DXVECTOR3& GetPosition() const {
		return m_Position;
	}

	//回転設定関数.
	void SetRotation( float x, float y, float z ){
		m_Rotation.x = x;
		m_Rotation.y = y;
		m_Rotation.z = z;
	};
	//x,y,zをうたなくてもいい関数.
	void SetRotation(const D3DXVECTOR3& rot)
	{
		m_Rotation = rot;
	}

	void SetRotationY(float y_rot)
	{
		m_Rotation.y = y_rot;
	}

	//回転取得関数.
	const D3DXVECTOR3& GetRotation() const 
	{
		return m_Rotation;
	}
	//拡縮設定関数.
	void SetScale(const D3DXVECTOR3& newScale)
	{
		m_Scale = newScale;
	}

	//拡縮取得関数.
	const D3DXVECTOR3& GetScale() const {
		return m_Scale;
	}
	//位置の追加関数.
	void AddPosition(const D3DXVECTOR3& delta)
	{
		m_Position += delta;
	}


protected://protectedは子クラスのみアクセス可能.
	D3DXVECTOR3	m_Position;
	D3DXVECTOR3	m_Rotation;
	D3DXVECTOR3	m_Scale;


};