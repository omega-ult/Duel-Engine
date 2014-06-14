//  [6/14/2014 OMEGA] created

#ifndef _DUELD3D9VOLATILERESOURCE_H_
#define _DUELD3D9VOLATILERESOURCE_H_

#include "DuelD3D9Common.h"
#include "DuelD3D9Include.h"

namespace Duel
{
	// object contains d3d resources created as D3DPOOL_DEFAULT must 
	// implement this method.
	class D3D9VolatileResource 
	{
	public:
		// called when a d3d9device lost.
		virtual void	onDeviceLost() = 0;
		// called when a d3d9device reset.
		virtual void	onDeviceReset(IDirect3DDevice9* dev) = 0;
	};
}

#endif