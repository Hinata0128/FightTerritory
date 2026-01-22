#pragma once

#include <vector>

/**************************************************************************
*	InputKeyClass
*	キーボードを押している時等の判定.
**/

class InputKey
{
public:
	InputKey();
	~InputKey();

	void Update();

	//キー設定.
	void SetAnyKey(char k) { m_CheckKey = k; }

	//キーを押した瞬間.
	bool NowPressKey();
	//キーを押し続けている間.
	bool HoldDownKey() const;
	//キーを離した瞬間.
	bool ReleaseKey();
public:
	//キーの情報を保存.
	char m_CheckKey;

private:
	//押した瞬間.
	bool m_NowKey;
	//過去の瞬間.
	bool m_OldKey;
};