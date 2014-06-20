//  [6/7/2014 OMEGA] created

#ifndef _DUELD3D9GPUPROGRAM_H_
#define _DUELD3D9GPUPROGRAM_H_

#include "DuelD3D9Common.h"
#include "DuelGpuProgram.h"
#include "DuelD3D9Include.h"

namespace Duel
{
	class D3D9GpuProgram : public DGpuProgram
	{
		DUEL_DECLARE_RTTI(D3D9GpuProgram)
	public:
		D3D9GpuProgram(DResourceManager* fatherManager, const DString& name, const DString& groupName);
		virtual const DString& getLanguage( void ) const;


		virtual void	setProfile(const DString& profile);

		virtual size_t	calculateSize();
		INTERNAL void	setCompileError(const DString& err) { mCompileError = err; }

		ID3DXBuffer*	getCodeBuffer() { return mCodeBuf; }
		ID3DXConstantTable*	getConstantTable() { return mConstTable; }
	protected:		
		virtual void	loadFromSource();
		virtual void	unloadImpl();


		static DString		msLanguage;
		DString				mCompileError;

		ID3DXBuffer*	mCodeBuf;
		ID3DXConstantTable*	mConstTable;

	};

}

#endif