#ifndef __BEHAVIAC_HANDLER__
#define __BEHAVIAC_HANDLER__
#include "MethodIdDefine.h"
#include <Map>


class BehaviacHandler
{
	
	class Handler
	{
		public:
			virtual ~Handler() {}
			virtual  RetVar Handle(int argc, void ** argv)=0;
		private:
	};


	template<typename _Obj>
	class ObjHandler : public Handler
	{
		public:
			typedef _Obj ObjType;
			typedef ObjType * pointer;
			typedef RetVar(ObjType::*BcHandler)(int , void **);

			ObjHandler(BcHandler h,pointer obj):_h(h),_obj(obj) {};
			~ObjHandler() {};

			RetVar Handle(int argc,void ** argv)
			{
				if (_h&&_obj) {
					return (((pointer)_obj)->*_h)(argc,argv);
				 }
				RetVar ret;
				ret.iRet = 0;
				return ret;
			}
		private:
			BcHandler _h=nullptr;
			pointer	  _obj=nullptr;
	};

	typedef std::map<int, Handler*> HanderRegister_T;
	HanderRegister_T _handler;
public:
	BehaviacHandler() {
	}
	~BehaviacHandler() {
		Clear();
	}
	
	void Clear() 
	{
		_handler.clear();
	}

	template<typename _Obj>
	int ReginsterHandler(int MethodId,
		typename ObjHandler<_Obj>::pointer obj,
		typename ObjHandler<_Obj>::BcHandler h
	) 
	{
		Handler * handler = new ObjHandler<_Obj>(h, obj);
		if (!handler) 
		{
			return (-1);
		}
		_handler[MethodId] = handler;
		return (0);
	}

	RetVar  handle(int MethodId, int argc, void ** argv)
	{
		HanderRegister_T::iterator it=_handler.find(MethodId);
		if (it==_handler.end()) 
		{
			RetVar ret;
			ret.iRet = -1;
			return ret;
		}

		Handler *handler = it->second;
		return handler->Handle(argc, argv);
	}
};

#endif

