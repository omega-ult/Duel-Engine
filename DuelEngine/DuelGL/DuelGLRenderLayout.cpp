//  [8/28/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelRenderLayout.h"
#include "DuelGLRenderLayout.h"
#include "DuelGLVertexBuffer.h"
#include "DuelGLTranslator.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(GLRenderLayout, DRenderLayout);

	GLRenderLayout::GLRenderLayout(DRenderResourceFactory* creator) :
		DRenderLayout(creator),
		mVAO(0)
	{

	}

	GLRenderLayout::~GLRenderLayout()
	{
		if (mVAO)
		{
			glDeleteVertexArrays(1, &mVAO);
		}
	}



	void GLRenderLayout::seal()
	{
		if (mVAO)
		{
			glDeleteVertexArrays(1, &mVAO);
		}
		glGenVertexArrays(1, &mVAO);

		glBindVertexArray(mVAO);

		//////////////////////////////////////////////////////////////////////////
		// bind input layout.
		DVertexDeclaration vDecl = getVertexData().getDeclaration();
		DVertexDeclaration::VertexElementIterator vi = vDecl.getVertexElementIterator();
		// give an invalid source to begin with.
		//  [8/4/2013 OMEGA] ticket:
		// VAO 可以与layout绑定实现更高的效率. 这里我暂时不做优化.
		//glBindVertexArray(mVAO);
		int32 curSrc = -1;
		size_t	vertexStride = 0;
		// http://stackoverflow.com/questions/14249634/opengl-vaos-and-multiple-buffers
		// ps:
		/*
			This is still a very new extension, 
			and only NVIDIA really provides support for it at this point.
			And even that's still early. So you can't rely on it.
		*/
		std::vector<int32>	attrStack;
		while (vi.hasMoreElements())
		{
			char*	offsetP = 0;
			DVertexElement vd = vi.getNext();
			if (vd.getSource() != curSrc)
			{
				curSrc = vd.getSource();
				vertexStride = vDecl.getVertexSize(curSrc);
				DVertexStreams vs = getVertexData().getBufferStreams();
				DVertexBufferPtr vb = vs.getStream(curSrc);
				if (vb == NULL)
				{
					// collapsed streams. stop processing.
					break;
				}
				GLVertexBuffer*	glVB = vb->getAs<GLVertexBuffer>();
				glBindBuffer(GL_ARRAY_BUFFER, glVB->getGLVertexBufferID());
			}
			VertexElementSemantic sm = vd.getSemantic();
			// 我这里吐槽一下. 我的显卡支持最多29道attribute, 但!是!, position 必须是在0的位置上. 其他可以很随意.. 
			int32 smIndex = (int32)sm;
			if (sm == VES_TexCoord)
			{
				smIndex += vd.getIndex();
			}
			GLuint	size = 0;
			GLenum	type = 0;
			GLboolean	bNormalize = GL_FALSE;
			// store the index we've enabled. 
			attrStack.push_back(smIndex);
			GLTranslator::getGLVertexFormat(size, type, bNormalize, vd.getElementType());
			char*	offset = 0;
			// bind stream. - -ah..
			glVertexAttribPointer(
				smIndex, size, type, bNormalize, vertexStride, (offset + vd.getOffset()));
			glEnableVertexAttribArray(smIndex);
		}
		//  [8/27/2013 OMEGA] ati的支持问题??? 
		// 关于VAO的流程就是如此, 以后再对每一个layout进行优化. 对layout增加一个sealed属性, 如果是sealed则生成对应
		// VAO, 这样可以直接完成VAO的绑定进行绘制, 不需要再进行如上措施,
		// 当其中的某个设置如vertexbuffer/vertexdeclaration/indexbuffer改变时触发sealed属性改变, 则重新生成
		// 对应的VAO
		glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, NULL);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

		//////////////////////////////////////////////////////////////////////////
		//glBindVertexArray(mVAO);  

	}

}