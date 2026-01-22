#include "InputKey.h"

InputKey::InputKey()
	: m_NowKey		(false)
	, m_OldKey		(false)
	, m_CheckKey	()
{
}

InputKey::~InputKey()
{
}

void InputKey::Update()
{
	m_OldKey = m_NowKey;

	m_NowKey = GetAsyncKeyState(m_CheckKey) != 0;
}

//キーを押した瞬間.
bool InputKey::NowPressKey()
{
	//もし今のキーがfalseの時と過去のキーfalse.
	if (m_NowKey == true && m_OldKey == false)
	{
		return true;
	}
	return false;
}
//キーを押し続けている間.
bool InputKey::HoldDownKey() const
{
	return m_NowKey;
}
//キーを離した瞬間.
bool InputKey::ReleaseKey()
{
	if (m_NowKey == false && m_OldKey == true)
	{
		m_NowKey = m_OldKey;
		return true;
	}
	return false;
}
