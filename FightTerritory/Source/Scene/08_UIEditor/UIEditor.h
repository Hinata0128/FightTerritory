#pragma once
#include "Scene//SceneBase.h"
#include "GameObject//03_UIObject//UIObject.h"

/************************************************************************************************
*	UIエディタークラス.
*	ToDo : 各シーンのUIの配置をする用のクラスです.
**/

class UIEditor
	: public UIObject
{
public:
	UIEditor();
	~UIEditor() override;

	void Create() override;
	void Update() override;
	void Draw() override;

private:

};