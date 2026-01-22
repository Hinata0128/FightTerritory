#pragma once

/******************************************************************
*	マネージャーベースクラス.
*	純粋仮想関数で作成しています.
**/

class ManagerBase
{
public:
	ManagerBase() {};
	virtual ~ManagerBase() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void Create() {};
	virtual void Release() {};

protected:

};
