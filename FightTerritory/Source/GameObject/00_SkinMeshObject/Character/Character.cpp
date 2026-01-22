#include "Character.h"

Character::Character()
	: SkinMeshObject()
	, m_HitPoint	( 0.0f )
{
}

Character::~Character()
{
}

void Character::Update()
{
	SkinMeshObject::Update();
}

void Character::Draw()
{
	SkinMeshObject::Draw();
}

void Character::Init()
{
	//‰½‚à‘‚©‚È‚¢.
}
