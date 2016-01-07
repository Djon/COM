///////////////////////////////////////////////////////////////////////////
// Workfile : SingletonBase.h
// Author : Reinhard Penn, Bernhard Selymes
// Date : 18.11.2012
// Description : Definition of template class SingletonBase
///////////////////////////////////////////////////////////////////////////

#ifndef SINGLETONBASE_H
#define SINGLETONBASE_H

#include "Object.h"

//template singleton base class
template <typename T_DERIVED>
class SingletonBase {
public:
	static T_DERIVED& GetInstance()
	{
		static T_DERIVED oInstance;
		return oInstance;
	}
protected:
	SingletonBase(){};
private:
	SingletonBase(SingletonBase const& s){};
	SingletonBase& operator = (SingletonBase const& s){};
};

#endif