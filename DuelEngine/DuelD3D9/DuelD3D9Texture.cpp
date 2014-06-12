//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9Texture.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(D3D9Texture, DTexture);

	D3D9Texture::D3D9Texture( DResourceManager* fathermanager, const DString& name, const DString& groupName ) :
		DTexture(fathermanager, name, groupName)
	{

	}

	bool D3D9Texture::isHardwareGammaSupported()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	bool D3D9Texture::isHardwareMipMapSupported()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9Texture::copyToTexture( DTexture* dstTex )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DPixelBufferPtr D3D9Texture::getBuffer( CubeFace face /*= CF_PositiveX*/, size_t mipmap /*= 0 */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DGpuTextureConstantPtr D3D9Texture::getGpuTexutureConstant()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9Texture::createHardwareResource()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9Texture::releaseHardwareResource()
	{
		throw std::exception("The method or operation is not implemented.");
	}

}