//  [7/13/2013 OMEGA] created

#ifndef _DUELGUILAYOUTENGINE_H_
#define _DUELGUILAYOUTENGINE_H_

#include "DuelCommon.h"
#include "DuelGUICommon.h"
#include "DuelGUISize.h"
#include "DuelGUISizePolicy.h"
#include "DuelGUILayoutItem.h"

namespace Duel
{
	// stretch is the factor take effect on multi items, for example,
	// there are three items in one layout i1, i2, i3, with stretch factor
	// 1, 3, 2, then the i2 has more space than i3, i3 has more space than i1.
	struct DGLayoutStruct
	{
		inline void		init(uint32 stretchFactor = 0, DReal minSize = 0.0f) {
			stretch = stretchFactor;
			minimumSize = sizeHint = minSize;
			maximumSize = DGLAYOUTSIZE_MAX;
			expansive = false;
			empty = true;
			spacing = 0;
		}
		// 仅仅是一个快速取值的接口.- -
		DReal	smartSizeHint() 
		{
			return (stretch > 0) ? minimumSize : sizeHint;
		}
		uint32	effectiveSpacer(uint32 uniformSpacer) const 
		{
			assert(uniformSpacer >= 0 || spacing >= 0);
			return (uniformSpacer >= 0) ? uniformSpacer : spacing;
		}
		// properties
		uint32	stretch;
		DReal	sizeHint;
		DReal	maximumSize;
		DReal	minimumSize;
		bool	expansive;	// 中译: 扩展性
		bool	empty;
		uint32	spacing;	// 物件后方的间隔大小.

		// temporary storage
		bool	done;

		// result
		DReal	pos;
		DReal	size;
	};

	// just a collection of static functions, used in calculation of item size.
	class DUELGUI_API DGLayoutEngine
	{
	public:
		// used to generate a proper size.
		static DGSize	smartMinSize(DGWidgetItem* item);
		static DGSize	smartMinSize(DGWidget* w);
		static DGSize	smartMinSize(const DGSize& sizeHint,
			const DGSize& minSize, const DGSize& maxSize,
			const DGSizePolicy& sizePolicy);
		static DGSize	smartMaxSize(DGWidgetItem* item, DGAlignment alignment);
		static DGSize	smartMaxSize(DGWidget* w, DGAlignment alignment);
		static DGSize	smartMaxSize(const DGSize& sizeHint,
			const DGSize& minSize, const DGSize& maxSize,
			const DGSizePolicy& sizePolicy, DGAlignment alignment);

		
		/*
		  This is the main workhorse of the DGGridLayout. It portions out
		  available space to the chain's children.

		  The calculation is done in fixed point: "fixed" variables are
		  scaled by a factor of 256.

		  If the layout runs "backwards" (i.e. RightToLeft or Up) the layout
		  is computed mirror-reversed, and it's the caller's responsibility
		  do reverse the values before use.

		  chain contains input and output parameters describing the geometry.
		  count is the count of items in the chain; pos and space give the
		  interval (relative to parentWidget topLeft).
		*/
		/*	comment:
			qGeomCalc()计算大小主要分为三种情况来进行的，这其中也隐藏了它的控制规则，三种情况如下：
			1）所分配的总空间大小 < 所有子控件的minimumSize总和
			此时，会根据控件size由大到小的顺序，减小控件的size。
			2）所分配的总空间大小 < 所有子控件的smartSizeHint总和
			此时，会先保证满足固定大小的控件的minimumSize，然后再通过逐一地平均地减小各控件的smartSizeHint()来得到size值。
			3）所分配的总空间大小 > 所有子控件的smartSizeHint的总和（即，需要扩展子控件大小）
			此时， 会先设置好固定大小的子控件，然后再平均地分配剩下的子控件的大小，再看平均分配大小后的子控件是否能够满足条件(大于smartSizeHint 且小于maximumSize)，且进行调整

			参数说明, chain即为计算结果, start, count 即为起始点和计算的物件数, pos为虚拟的相对空间中点, space是可用长度, spacer为物件间隔

		*/
		static void		calculateGeometry(std::vector<DGLayoutStruct> &chain, uint32 start, uint32 count,
					   DReal pos, DReal space, uint32 spacer);
		
		/*
		  Modify total maximum (max), total expansion (exp), and total empty
		  when adding boxmax/boxexp.

		  Expansive boxes win over non-expansive boxes.
		  Non-empty boxes win over empty boxes.
		  // 这个用于计算最大展开长度, 传入的max是当前最大, boxmax是用于对比的最大, 如果boxmax大于当前最宽,
		  // 则将当前最宽撑大.
		*/
		static inline void	maxExpandingCalculation(DReal & max, bool &exp, bool &empty,
			DReal boxmax, bool boxexp, bool boxempty)
		{
			if (exp) 
			{
				if (boxexp)
				{
					max = DMath::Max<DReal>(max, boxmax);
				}
			}
			else 
			{
				if (boxexp || (empty && (!boxempty || DMath::realEqual(max, 0.0f))))
				{
					max = boxmax;
				}
				else if (empty == boxempty)
				{
					max = DMath::Min<DReal>(max, boxmax);
				}
			}
			exp = exp || boxexp;
			empty = empty && boxempty;
		}

	};

}


#endif