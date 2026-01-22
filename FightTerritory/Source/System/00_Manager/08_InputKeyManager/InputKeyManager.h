#pragma once
#include "System/04_Input/InputKey/InputKey.h"

#include <unordered_map>
#include <string>

/*****************************************************************
*	InputKeyManager.
**/

class InputKeyManager
{
public:
	InputKeyManager();
	~InputKeyManager();
	
	void Update();

	//キー設定.
	void SetAnyKey(char k);

	//キーを押した瞬間.
	bool NowPressKey();
	//キーを押し続けている間.
	bool HoldDownKey();
	//キーを離した瞬間.
	bool ReleaseKey();

	//特定のキーの状態を取得するための関数.
	const InputKey* GetKey(const std::string& keyName) const;

private:
	//第一要素が型名(キー).
	//第二要素がクラス名(値).
	std::unordered_map<std::string, InputKey> m_Keys;
};