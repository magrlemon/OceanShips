#ifndef __BEHAVIAC_MANAGER__
#define __BEHAVIAC_MANAGER__
#include "BehaviacHandler.h"
class BahaviacManager
{
private:
	BahaviacManager();
public:
	BahaviacManager(const BahaviacManager & BahaviacMgr) = delete;
	BahaviacManager& operator=(const BahaviacManager & BahaviacMgr) = delete;
	~BahaviacManager();
public:
	BehaviacHandler& GetBehaviacHandler();
private:
	BehaviacHandler _BehaviacHandler;
private:
	static BahaviacManager * _pInstance;
public:
	static BahaviacManager & GetInstance() 
	{
		if (_pInstance!=nullptr) 
		{
			return *_pInstance;
		}

		_pInstance = new BahaviacManager();
		return *_pInstance;
	}

	static void DestoryInstance() {

		if (_pInstance)
		{
			delete _pInstance;
			_pInstance = nullptr;
		}
	}

};
#endif // !__BEHAVIAC_MANAGER__

