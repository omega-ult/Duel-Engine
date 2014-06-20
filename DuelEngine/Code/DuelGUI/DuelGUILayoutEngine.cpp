//  [7/13/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "DuelCommon.h"
#include "DuelGUIWidget.h"
#include "DuelGUISize.h"
#include "DuelGUISizePolicy.h"
#include "DuelGUILayoutEngine.h"

namespace Duel
{


	DGSize DGLayoutEngine::smartMinSize( DGWidgetItem* item )
	{
		return smartMinSize(item->getWidget());
	}

	DGSize DGLayoutEngine::smartMinSize( DGWidget* w )
	{
		return smartMinSize(w->getSizeHint(), w->getMinimumSize(),
			w->getMaximumSize(), w->getSizePolicy());
	}

	DGSize DGLayoutEngine::smartMinSize( const DGSize& sizeHint, const DGSize& minSize, const DGSize& maxSize, const DGSizePolicy& sizePolicy )
	{
		DGSize s(0, 0);

		if (sizePolicy.getHorizontalPolicy() != SP_Ignored)
		{
			if (sizePolicy.getHorizontalPolicy() & SPF_Shrink)
				s.setWidth(minSize.getWidth());
			else
				s.setWidth(DMath::Max<DReal>(sizeHint.getWidth(), minSize.getWidth()));
		}

		if (sizePolicy.getVerticalPolicy() != SP_Ignored) {
			if (sizePolicy.getVerticalPolicy() & SPF_Shrink) {
				s.setHeight(minSize.getHeight());
			} else {
				s.setHeight(DMath::Max<DReal>(sizeHint.getHeight(), minSize.getHeight()));
			}
		}

		s = s.boundedTo(maxSize);
		
		return s.expandedTo(DGSize(0.0f,0.0f));
	}

	DGSize DGLayoutEngine::smartMaxSize( DGWidgetItem* item, DGAlignment alignment )
	{
		return smartMaxSize(item->getWidget(), alignment);
	}

	DGSize DGLayoutEngine::smartMaxSize( DGWidget* w, DGAlignment alignment )
	{
		return smartMaxSize(w->getSizeHint(), 
			w->getMinimumSize(), w->getMaximumSize(),
			w->getSizePolicy(), alignment);
	}

	DGSize DGLayoutEngine::smartMaxSize( const DGSize& sizeHint, const DGSize& minSize, const DGSize& maxSize,
		const DGSizePolicy& sizePolicy, DGAlignment align )
	{
		if (align & AF_HorizontalMask && align & AF_VerticalMask)
			return DGSize(DGLAYOUTSIZE_MAX, DGLAYOUTSIZE_MAX);
		DGSize s = maxSize;
		DGSize hint = sizeHint.expandedTo(minSize);
		if (s.getWidth() == DGLAYOUTSIZE_MAX && !(align & AF_HorizontalMask))
			if (!(sizePolicy.getHorizontalPolicy() & SPF_Grow))
				s.setWidth(hint.getWidth());

		if (s.getHeight() == DGLAYOUTSIZE_MAX && !(align & AF_VerticalMask))
			if (!(sizePolicy.getVerticalPolicy() & SPF_Grow))
				s.setHeight(hint.getHeight());

		if (align & AF_HorizontalMask)
			s.setWidth(DGLAYOUTSIZE_MAX);
		if (align & AF_VerticalMask)
			s.setHeight(DGLAYOUTSIZE_MAX);
		return s;
	}

	void DGLayoutEngine::calculateGeometry(std::vector<DGLayoutStruct> &chain, uint32 start, uint32 count,
		DReal pos, DReal space, uint32 spacer)
	{
		//  [11/5/2013 OMEGA]
		// 好吧这段代码只能慢慢啃了... 

		// c = cumulative 累计
		DReal cHint = 0.0f;
		DReal cMin = 0.0f;
		DReal cMax = 0.0f;
		uint32 sumStretch = 0;
		// sumSpacing子项间的间隔总和
		uint32 sumSpacing = 0;

		bool wannaGrow = false; // anyone who really wants to grow?
		//    bool canShrink = false; // anyone who could be persuaded to shrink?

		bool allEmptyNonstretch = true;
		// pendingSpacing即为最后那个物件的后方使用的spacer, 用于回头减去.
		int32 pendingSpacing = -1;
		uint32 spacerCount = 0;
		uint32 i;

		for (i = start; i < start + count; i++) 
		{
			DGLayoutStruct *data = &chain[i];

			// 将所有物件的done设为初始值 以便下面计算.
			data->done = false;
			// 累加各种长度总和.
			cHint += data->smartSizeHint();
			cMin += data->minimumSize;
			cMax += data->maximumSize;
			sumStretch += data->stretch;
			if (!data->empty) 
			{
				/*
				Using pendingSpacing, we ensure that the spacing for the last
				(non-empty) item is ignored.
				这里的手法是, 记录该物件的spacer, 在测试下一个物件时加上, 这样就保证最后一个
				不会被处理到.
				*/
				if (pendingSpacing >= 0)
				{
					sumSpacing += pendingSpacing;
					++spacerCount;
				}
				pendingSpacing = data->effectiveSpacer(spacer);
			}
			// 检测grow性, 一旦其中一个元素有扩展性, 那么整个grow都是成立的.
			wannaGrow = wannaGrow || data->expansive || data->stretch > 0;
			// 如果所有物体都为空, 则下面属性为真.
			allEmptyNonstretch = allEmptyNonstretch && !wannaGrow && data->empty;
		}

		// 额外空间?
		DReal extraspace = 0.0f;

		// 来了 第一种情况, 所分配空间不足够放置所有物件.的情况.
		if (space < cMin + sumSpacing) 
		{
			/*
			Less space than minimumSize; take from the biggest first
			这句话的意思是从最大的元素里面抠出来.
			*/

			DReal minSize = cMin + sumSpacing;

			// shrink the spacers proportionally
			// 减小spacer
			if (spacer >= 0.0f) 
			{
				spacer = minSize > 0.0f ? (uint32)(spacer * space / minSize) : 0;
				sumSpacing = spacer * spacerCount;
			}
			// 存储最小长度的列表.
			std::vector<DReal> list;

			for (i = start; i < start + count; i++)
			{
				list.push_back(chain[i].minimumSize);
			}
			// 从小到大排列, 
			std::stable_sort(list.begin(),list.end());

			// 剩余可用空间, 从总共的空间中减去spacer的总长
			DReal space_left = space - sumSpacing;

			// 从小物件开始适配, 看看能使用多少空间.
			DReal sum = 0.0f;
			uint32 idx = 0;
			DReal space_used = 0.0f;	// 这是个暂存值.
			DReal current = 0;	// 当前测试的最大的物件.
			while (idx < count && space_used < space_left)
			{
				current = list.at(idx);
				// 尝试将更多的小物件容纳进去.
				space_used = sum + current * (count - idx);
				sum += current;
				++idx;
			}
			// 将id返回到最后一个物件上.
			--idx;
			// 不足的值.
			DReal deficit = space_used - space_left;
			// 不够使用的物件数.
			uint32 items = count - idx;
			/*
			* If we truncate all items to "current", we would get "deficit" too many pixels. Therefore, we have to remove
			* deficit/items from each item bigger than maxval. The actual value to remove is deficitPerItem + remainder/items
			* "rest" is the accumulated error from using integer arithmetic.
			
			这句话的意思是, 如果我们缩减所有物件到 "当前最大" 的那个值, 我们像素会不够用,
			所以我们必须去除那些大于"maxval"的物件, 实际需要移除的是 deficitPerItem + remainder/items
			但是我们用的是浮点数, 这里的deficitPerItem 已经是足够了的.
			*/
			DReal deficitPerItem = deficit/items;
			//DReal remainder = deficit % items;	// 这个不需要了. 在浮点数的情况下, deficitPerItem已经是每物件需要缩减的数量.
			// 这个是能使用的最大值.
			DReal maxval = current - deficitPerItem;

//			DReal rest = 0.0f;
			for (i = start; i < start + count; i++)
			{
				// 这里的做法是对整数的一个trick, 整数取余剩下的像素逐个分配到每个元素中
// 				DReal maxv = maxval;
// 				rest += remainder;
// 				if (rest >= items) 
// 				{
// 					maxv--;
// 					rest-=items;
// 				}
				DGLayoutStruct *data = &chain[i];
				// 将超过最大值的物件缩减到maxval上.
				data->size = DMath::Min<DReal>(data->minimumSize, maxval);
				data->done = true;
			}
		} 
		// 第二种情况, 空间小于最佳适配空间. 但大于最小值.
		else if (space < cHint + sumSpacing) 
		{
			/*
			Less space than smartSizeHint(), but more than minimumSize.
			Currently take space equally from each, 从每个物件中取出相同的空间.
			Commented-out lines will give more space to stretchier
			items.
			*/
			// 分配不足的物件数量
			uint32 n = count;
			DReal space_left = space - sumSpacing;
			// 透支的部分. 
			DReal overdraft = cHint - space_left;

			// first give to the fixed ones:
			for (i = start; i < start + count; i++) 
			{
				DGLayoutStruct *data = &chain[i];
				if (!data->done
					&& data->minimumSize >= data->smartSizeHint())
				{
						data->size = data->smartSizeHint();
						data->done = true;
						space_left -= data->smartSizeHint();
						n--;
				}
			}
			bool finished = n == 0;
			while (!finished)
			{
				finished = true;
				DReal deficitPerItem = 0.0f;

				// 下面从小到大逐渐尝试新的缩减长度(fp_over/n)
				for (i = start; i < start+count; i++) 
				{
					DGLayoutStruct *data = &chain[i];
					if (data->done)
					{
						continue;
					}
					// 由于使用DReal型, 这里只要将每个物件减少的空间计算出来即可.
					// 这里不会发生除0的结果.. 因为一旦处理到最后一个, 必然满足n==1的情况,
					// 该必须能够缩减overdraft/n的值(如果不能, 则不会进入这种minSize<space<sizeHint的情况.
					n = n == 0 ? n : 1;
					deficitPerItem = overdraft / n; 

					data->size = data->smartSizeHint() - deficitPerItem;
					// 如果该物件实在不能缩了(大小小于最小值), 则跳过该物件.
					if (data->size < data->minimumSize) 
					{
						data->done = true;
						data->size = data->minimumSize;
						finished = false;
						// 从overdraft中减去该物件缩减带来的影响.
						overdraft -= data->smartSizeHint() - data->minimumSize;
						n--;
						break; // 重新从头计算.
					}
				}
			}
		}
		// 第三种情况, 空间大于最佳适配空间, 此时展开的物件可以展开..
		else 
		{ 
			// extra space
			uint32 n = count;
			DReal space_left = space - sumSpacing;
			// first give to the fixed ones, and handle non-expansiveness
			// 先弄好那些固定最大长度的家伙.
			for (i = start; i < start + count; i++) 
			{
				DGLayoutStruct *data = &chain[i];
				if (!data->done
					&& (data->maximumSize <= data->smartSizeHint()
					|| (wannaGrow && !data->expansive && data->stretch == 0)
					|| (!allEmptyNonstretch && data->empty &&
					!data->expansive && data->stretch == 0))) 
				{
					data->size = data->smartSizeHint();
					data->done = true;
					space_left -= data->size;
					sumStretch -= data->stretch;
					n--;
				}
			}
			extraspace = space_left;

			/*
			Do a trial distribution and calculate how much it is off.
			If there are more deficit pixels than surplus pixels, give
			the minimum size items what they need, and repeat.
			Otherwise give to the maximum size items, and repeat.

			Paul Olav Tvete has a wonderful mathematical proof of the
			correctness of this principle, but unfortunately this
			comment is too small to contain it.
			
			上面那句话的意思是先分配一组, 看看如果剩余的不足像素大于超出的像素, 给minimumSize的
			物件安排好空间, 然后重复, 否则给那些maximumSize的物件, 然后重复.
			Paul Olav Tvete对此有有数学的证明, 但是这里的版面太小了. 写不下去.- -....
			*/
			
			DReal surplus, deficit;
			do {
				surplus = deficit = 0.0f;
// 				DReal fp_space = toFixed(space_left);
				DReal fp_w = 0.0f;
				for (i = start; i < start + count; i++) 
				{
					DGLayoutStruct *data = &chain[i];
					if (data->done)
					{
						continue;
					}
					extraspace = 0.0f;
					if (sumStretch = 0)
					{
						fp_w = space_left / n;
					}
					else
					{
						// 这里给出了stretch因子的作用效果, 如果总stretch不为0,
						// 那么该物件得到的空间即为该stretch在总stretch中的权重.
						fp_w = (space_left * data->stretch) / sumStretch;
					}
//					DReal w = fRound(fp_w);
					data->size = fp_w;
// 					fp_w -= toFixed(w); // give the difference to the next
					if (fp_w < data->smartSizeHint()) 
					{
						deficit +=  data->smartSizeHint() - fp_w;
					} 
					else if (fp_w > data->maximumSize) 
					{
						surplus += fp_w - data->maximumSize;
					}
				}
				if (deficit > 0.0f && surplus <= deficit)
				{
					// give to the ones that have too little
					for (i = start; i < start+count; i++) 
					{
						DGLayoutStruct *data = &chain[i];
						if (!data->done && data->size < data->smartSizeHint()) 
						{
							data->size = data->smartSizeHint();
							data->done = true;
							space_left -= data->smartSizeHint();
							sumStretch -= data->stretch;
							n--;
						}
					}
				}
				if (surplus > 0.0f && surplus >= deficit) 
				{
					// take from the ones that have too much
					for (i = start; i < start + count; i++)
					{
						DGLayoutStruct *data = &chain[i];
						if (!data->done && data->size > data->maximumSize)
						{
							data->size = data->maximumSize;
							data->done = true;
							space_left -= data->maximumSize;
							sumStretch -= data->stretch;
							n--;
						}
					}
				}
			} while (n > 0 && surplus != deficit);

			if (n == 0)
			{
				extraspace = space_left;
			}
		}

		/*
		As a last resort, we distribute the unwanted space equally
		among the spacers (counting the start and end of the chain). We
		could, but don't, attempt a sub-pixel allocation of the extra
		space.
		// 这里需要转变坐标系, 使用中点定位的情况
		*/
		DReal extra = extraspace / (spacerCount + 2);
		DReal p = pos + extra - space/2;
		for (i = start; i < start+count; i++) 
		{
			DGLayoutStruct *data = &chain[i];
			p += data->size/2;
			data->pos = p;
			p += data->size/2;
			if (!data->empty)
			{ 
				// skip the empty box.
				p += data->effectiveSpacer(spacer) + extra;
			}
		}
	}

}