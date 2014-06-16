//  [6/7/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelGpuProgram.h"
#include "DuelD3D9GpuProgram.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9GpuProgram, DGpuProgram);


	Duel::DString D3D9GpuProgram::msLanguage = "HLSL";

	D3D9GpuProgram::D3D9GpuProgram( DResourceManager* fatherManager, const DString& name, const DString& groupName ) :
		DGpuProgram(fatherManager, name, groupName),
		mCodeBuf(NULL),
		mConstTable(NULL)
	{

	}

	const DString& D3D9GpuProgram::getLanguage( void ) const
	{
		return msLanguage;
	}

	void D3D9GpuProgram::loadFromSource()
	{
		ID3DXBuffer* err = NULL;
		std::vector<D3DXMACRO> defines;
		const D3DXMACRO* pDefines = 0;
		DString stringBuffer;
		if (!mPreProcessor.empty())
		{
			stringBuffer = mPreProcessor;

			// Split preprocessor defines and build up macro array
			D3DXMACRO macro;
			DString::size_type pos = 0;
			while (pos != DString::npos)
			{
				macro.Name = &stringBuffer[pos];
				macro.Definition = 0;

				DString::size_type start_pos=pos;

				// Find delims
				pos = stringBuffer.find_first_of(";,=", pos);

				if(start_pos==pos)
				{
					if(pos==stringBuffer.length())
					{
						break;
					}
					pos++;
					continue;
				}

				if (pos != DString::npos)
				{
					// Check definition part
					if (stringBuffer[pos] == '=')
					{
						// Setup null character for macro name
						stringBuffer[pos++] = '\0';
						macro.Definition = &stringBuffer[pos];
						pos = stringBuffer.find_first_of(";,", pos);
					}
					else
					{
						// No definition part, define as "1"
						macro.Definition = "1";
					}

					if (pos != DString::npos)
					{
						// Setup null character for macro name or definition
						stringBuffer[pos++] = '\0';
					}
				}
				else
				{
					macro.Definition = "1";
				}
				if(strlen(macro.Name)>0)
				{
					defines.push_back(macro);
				}
				else
				{
					break;
				}
			}

			// Add NULL terminator
			macro.Name = 0;
			macro.Definition = 0;
			defines.push_back(macro);

			pDefines = &defines[0];
		}

		D3DXCompileShader(mSourceCode.c_str(), mSourceCode.length(),pDefines,NULL,mEntry.c_str(),
			mProfile.c_str(), 0, &mCodeBuf, &err, &mConstTable);
		if (err != NULL)
		{
			mCompileError.assign(static_cast<const char*>(err->GetBufferPointer()));
			mbFailed = true;
			ReleaseCOM(err);
		}
	}

	size_t D3D9GpuProgram::calculateSize()
	{
		return 0;
	}

	void D3D9GpuProgram::setProfile( const DString& profile )
	{
		if (DStringTool::isStartWith(profile, "vs"))
		{
			mProgramType = GPT_Vertex;
		}
		else if (DStringTool::isStartWith(profile, "ps"))
		{
			mProgramType = GPT_Pixel;
		}
		else
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Unknown shader type: " + mProfile,
				"Duel::D3D9GpuProgram::setProfile")
		}
		DGpuProgram::setProfile(profile);
	}

	void D3D9GpuProgram::unloadImpl()
	{
		ReleaseCOM(mCodeBuf);
		ReleaseCOM(mConstTable);
		DGpuProgram::unloadImpl();
	}

}