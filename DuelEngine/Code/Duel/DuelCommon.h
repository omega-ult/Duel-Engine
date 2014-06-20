/*
	[7/15/2012 OMEGA] created

	Used for defs and constants.
	
*/


#ifndef _DUELCOMMON_H_
#define _DUELCOMMON_H_

/////////////////////////////////
// config 
#include "DuelConfig.h"
/////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// [4/2/2013 OMEGA]
// 引擎核心编程规范 v1.0:
// 1. 属于引擎中定义的类, 非外部已定义的对象, 导出类统一使用以D开头, 对于类内定义的各种类型或者enum型可以不作要求.
// 2. 类成员函数统一使用小写单词开头的Camel规则.
// 3. 类成员变量统一使用m开头, 对于bool型变量, 统一用mb开头.
// 4. enum型必须以全部使用首字母大写的Camel规则, 并且以enum名字的每个单词的首字母组合加下划线作为值的开头.

//////////////////////////////////
#include <stdlib.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
//#include <cReal>
#include <iomanip>
#include <io.h>
//////////////////////////////////
#ifdef DUEL_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

//////////////////////////////////
// STL include
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <map>
#include <unordered_map>	// hashmap
#define HashMap std::tr1::unordered_map
#include <unordered_set>	// hashset
#define	HashSet	std::tr1::unordered_set
#include <set>
#include <list>
#include <deque>
#include <bitset>
#include <limits>
//////////////////////////////////
// boost config
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
#include <wrl.h>
#include <wrl/client.h>
#include <Windows.Foundation.h>
#include <windows.storage.h>
#include <windows.applicationmodel.h>
#else
//#include <boost/config/user.hpp>
#include <boost/config/user.hpp>
#endif // DUEL_PLATFORM_WINDOWS_PHONE_8



#include "DuelType.h"
#include "DuelResultCode.h"
#include "DuelSTLWrapper.h"
#include "DuelSignal.h"
#include "DuelPreDeclare.h"
#include "DuelRTTI.h"
#include "DuelObject.h"
#include "DuelMathLib.h"
#include "DuelThreadLib.h"
#include "DuelXML.h"
#include "DuelString.h"
#include "DuelSingleton.h"


#endif