//  [10/5/2012 OMEGA] created

#ifndef _DUELD3D9COMMON_H_
#define _DUELD3D9COMMON_H_

#include "DuelConfig.h"
#include "DuelD3D9Config.h"
#include "DuelD3D9PreDeclare.h"
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"D3Dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"DxErr.lib")

#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }


#endif