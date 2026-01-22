#include "InputKeyManager.h"

InputKeyManager::InputKeyManager()
{
	m_Keys["W"].SetAnyKey(0x57); 
	m_Keys["A"].SetAnyKey(0x41); 
	m_Keys["S"].SetAnyKey(0x53); 
	m_Keys["D"].SetAnyKey(0x44);

	m_Keys["Space"].SetAnyKey(0x20);
}

InputKeyManager::~InputKeyManager()
{
}

void InputKeyManager::Update()
{
	for (auto& key : m_Keys)
	{
		key.second.Update();
	}
}

void InputKeyManager::SetAnyKey(char k)
{
	for (auto& key : m_Keys)
	{
		key.second.SetAnyKey(k);
	}
}

bool InputKeyManager::NowPressKey()
{
	for (auto& key : m_Keys)
	{
		if (key.second.NowPressKey())
		{
			return true;
		}
	}
	return false;
}

bool InputKeyManager::HoldDownKey()
{
	for (auto& pair : m_Keys)
	{
		if (pair.second.HoldDownKey())
		{
			return true;
		}
	}
	return false;
}

bool InputKeyManager::ReleaseKey()
{
	for (auto& key : m_Keys)
	{
		if (key.second.ReleaseKey())
		{
			return true;
		}
	}
	return false;
}

const InputKey* InputKeyManager::GetKey(const std::string& keyName) const
{
	auto it = m_Keys.find(keyName);
	if (it != m_Keys.end())
	{
		return &it->second;
	}
	return nullptr;
}