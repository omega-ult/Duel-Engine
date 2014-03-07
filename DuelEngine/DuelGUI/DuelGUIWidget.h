//  [6/29/2013 OMEGA] created


#ifndef _DUELGUIWIDGET_H_
#define _DUELGUIWIDGET_H_

#include "DuelCommon.h"
#include "DuelNode.h"
#include "DuelGUICommon.h"
#include "DuelInputEventQueue.h"
#include "DuelGUISizePolicy.h"
#include "DuelGUISize.h"
#include "DuelGUIGeometry.h"
#include "DuelGUILayout.h"
#include "DuelGUISizeAdjustment.h"
#include "DuelGUIRenderDelegate.h"

namespace Duel
{
	// 妈蛋前向声明有时间再全弄到predeclare里.
	class DGUIManager;
	class DGLayout;
	// 1096
#define DGWIDGETSIZE_MAX (0x1000)

	// 我这里先用中文写下这段解释吧.
	/*
		每个widget都是3D空间中的一个元素, 包含一个界定框和一个中心点, 每个widget包含一个transformNode 和一个 locatorNode
		其中transformNode用于展示元素的3D变换, 它的父节点只有也必须是locatorNode, locatorNode的父节点可以为空(根widget),
		也可以是父Widget的transfromNode, 从而实现整个gui系统的3D化(至于节点关系图请自行绘制). 对于标准的2D控件在3D场景中的位置, 使用transformNode的matrix
		乘以其geometry的size上的各个点即可, 然而会碰到子物件被父物件裁切的情况, 对于这样的情形, 我们使用getClippedMargin来得到
		元素在上下左右四个方向上被裁切掉的大小, 叠加上元素的geometry的size属性, 即可得到最终在屏幕上显示的区域.
	*/

	// widget is a rectangle block used in construction of gui interface,
	// it is only designed for forming overall structure
	// its representation is done by DGRenderDelegate
	class DUELGUI_API DGWidget : public DObject
	{
	DUEL_DECLARE_RTTI(DGWidget)
	public:
		// a DGUIManager is needed for receiving events and
		// rendering!, after the DGUIManager is destroyed, this widget
		// will not receive any event,
		DGWidget(DGWidget* parent = NULL);
		// when a parent widget is deleted, 
		virtual ~DGWidget();

		// posInWidget use widget's center as origin.
		DSignal<void(DGWidget* w, const DVector2& posInWidget)>
			signalMouseMove;

		DSignal<void(DGWidget* w, const DVector2& posInWidget)>
			signalMousePressed;

		DSignal<void(DGWidget* w, const DVector2& posInWidget)>
			signalMouseReleased;

		DSignal<void(DGWidget* w, const DVector2& posInWidget)>
			signalMouseClicked;

		DSignal<void(DGWidget* w)>
			signalMoved;
		DSignal<void(DGWidget* w)>
			signalResized;

		//virtual	bool			isModal() const { return mbModal; }
		virtual	bool			isEnable() const { return mbEnable; }
		virtual	void			setEnable(bool flag) { mbEnable = flag; }

		// returns true if the widget is hidden, otherwise returns false.
		bool					isVisible() const { return mbVisible; }
		void					setVisible(bool flag) { mbVisible = flag; }

		// opacity property ranges in [0.0f, 1.0f] 1.0f means you can not see the widgets behind
		// this one. any value bigger than 1.0f is considered as 1.0f, smaller than 0.0f is considered 0.0f relatively.
		void					setOpacity(DReal val);
		DReal					getOpacity();

		// This property holds the default layout behavior of the widget.
		// If there is a DGLayout that manages this widget's children, the size policy specified by that layout is used. 
		// If there is no such DGLayout, the result of this function is used.
		DGSizePolicy			getSizePolicy() const;
		void					setSizePolicy(DGSizePolicy policy);

		// This property holds the recommended size for the widget.
		// If the value of this property is an invalid size, no size is recommended.
		// The default implementation of sizeHint() returns an invalid size if there is no layout for this widget, 
		// and returns the layout's preferred size otherwise.
		// this property can be affected by the resize() method, 
		DGSize					getSizeHint() const;

		// This property holds the widget's minimum size.
		// The widget cannot be resized to a smaller size than the minimum widget size. The widget's size is forced to the minimum size if the current size is smaller.
		// The minimum size set by this function will override the minimum size defined by DGLayout. In order to unset the minimum size, use a value of DGFSize(0, 0).
		// By default, this property contains a size with zero width and height.
		void					setMinimumSize(const DGSize& s);
		DGSize					getMinimumSize() const;
		void					setMaximumSize(const DGSize& s);
		DGSize					getMaximumSize() const;

		bool					isMinimized() const;
		bool					isMaximized() const;

		// set/get the geometry for this widget. you should not call set function directly.
		INTERNAL void			setGeometry(const DGGeometry& g);
		const DGGeometry&		getGeometry() const { return mGeometry; }

		
		// reposition this widget using relative coordinate. This interface is provided for client program.
		void					move(DReal relativeX, DReal relativeY);
		// reposition this widget using absolute coordinate.
		void					moveTo(DReal absX, DReal absY);
		// retrieve the center point relative to the parent widget's center point.
		// this value can be calculated in the layout updating.
		DVector2				getCenter();

		// resize this widget 
		void					resize(DReal w, DReal h);

		// query the current size of this widget.
		const DGSize&			getSize() const { return mGeometry.getSize(); }

		// This property holds the internal geometry of the widget
		DReal					getWidth() const { return mGeometry.getSize().getWidth(); }
		DReal					getHeight() const { return mGeometry.getSize().getHeight(); }

//		// 暂时先保留这两个接口, 以备 aspectRatio mode 使用.
// 		bool					hasHeightForWidth() { return false; }
// 		DReal					getHeightForWidth() { return 0.0f; }

		// get the parents' widgets clipped distance in all directions.
		void					getClippedDistance( DReal& L, DReal& R , DReal& T, DReal& B);

		// set transform to this widget, notice : do not reset this node's parent manually, parent
		// is assigned with locatorNode.
		DNode*					getTransformNode();

		// this widget will not own the layout.
		void					setLayout(DGLayout* layout);
		// return a valid layout object if this object belongs to one.
		DGLayout*				getLayout();

		// set a new parent for this widget.
		void					setParent(DGWidget* p);

		// override this function to process input event.
		// this method should be called by DGGUIManager.
		// return true if this widget capture the event.
		// notice that time stamp is calculated in input system, 
		// so do not rely it on its instant value.
		INTERNAL virtual bool	processEvent(const DEventBase* e, uint64 timeStamp);
		
		// set the hotspot for this widget, but do not own it, client should know how and 
		// when to delete it.
		void					setCustomHotspot(DGHotspot* hs);
		// a NULL can be returned if this widget have no hotspot, in this case
		// the rect of the rect will be used to test gui mouse event.
		DGHotspot*				getCustomHotspot();

		// set the remder delegate for this widget, but do not own it, client should know how and 
		// when to delete it.
		void					setCustomRenderDelegate(DGRenderDelegate* dele);
		// used to render this widget.
		DGRenderDelegate*		getCustomRenderDelegate();


		// when call this method, the widget will recalculate geometry for 
		// layout(if it has) in the update procedure.
		void					requestLayoutUpdate();

		// update every frame.
		INTERNAL void			updateLayout();
		// update the hotspot and render delegates.
		INTERNAL void			updateComponent();

		// push renderables to the renderqueue, prepare for rendering.
		void					applyToRenderQueue(DRenderQueue* queue, uint32 groupID);
		
		// get the root parent, which is directly belongs to a manager.
		DGWidget*				getParentWidget();
		DGWidget*				getRootWidget();
		bool					isRootWidget();
		DGUIManager*			getRootManager();

		// query whether this widget is focused.
		bool					isFocused();
		// only be called by DGUIManager, you should call DGUIManager::addWidget() instead of it.
		// do not call this method manually!
		INTERNAL void			setManager(DGUIManager* manager) { mManager = manager; }

		DGSize					getHostWindowSize();

	protected:
		// indicate whether this window is enabled
		bool				mbEnable;		
		bool				mbVisible;

		// geometry's center point rerlative to the parent widget, if this widget is a top-level widget, then this
		// value indicate the center point relative to the world space.
		DGGeometry			mGeometry;

		// size constrains
		DGSize				mMinimumSize;
		DGSize				mMaximumSize;
		// this value stores the preferred size for the widget, it will be 
		// calculated when updating the layout, then the actualSize will be set accrodingly.
		DGSize				mSizeHint;	


		// used in indicating the translate properties.
		DNode				mTransFormNode;
		DNode				mLocatorNode;

		// layout for this widget;
		DGLayout*			mLayout;

		// flag indicate the layout requests update.
		bool				mbLayoutDirty;

		// used to filter gui events
		// if custom hotspor is valid, the widget will use it to filter gui events
		// otherwise the default hotspot will be used.
		DGHotspot*			mCustomHotspot;
		DGHotspot*			mDefaultHotspot;

		// if the custom delegate is valid, the widget will use it for rendering
		// otherwise the default delegate will be used.
		DGRenderDelegate*	mCustomRenderDelegate;
		DGRenderDelegate*	mDefaultRenderDelegate;

		// parent widget.
		DGWidget*			mParent;
		// the manager belongs to.
		DGUIManager*		mManager;
		typedef	std::list<DGWidget*>			ChildrenWidgetList;
		ChildrenWidgetList	mChildren;
		
		// size policy for this widget.
		DGSizePolicy		mSizePolicy;
		// opacity property.
		DReal				mOpacity;
		
	};

}

#endif