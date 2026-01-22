#include "GameObject.h"

//コンストラクタ.
GameObject::GameObject()
	: m_Position	{}
	, m_Rotation	( 0.0f, 0.0f, 0.0f )
	, m_Scale		( 1.0f, 1.0f, 1.0f )
{
}

//デストラクタ.
GameObject::~GameObject()
{
}