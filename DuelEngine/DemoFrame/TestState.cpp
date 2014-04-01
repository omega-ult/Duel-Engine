//  [6/28/2013 OMEGA] created

#include "pch.h"
#include "Duel.h"
#include "DuelCore.h"
#include "DuelStateMachine.h"
#include "DuelApplication.h"
#include "DuelGUISystem.h"
#include "DuelGUIManager.h"
#include "DuelInputManager.h"
#include "DuelInputEventQueue.h"
#include "DuelSceneInstance.h"
#include "DuelTerrainPage.h"
#include "DuelImage2D.h"
#include "DuelMaterial.h"
#include "TestState.h"

#include "DuelGUIShowcasePictureBox.h"
#include "DuelGUIAlignment.h"

using Duel::DCore;
using Duel::DAStateBase;
using Duel::DApplication;
using Duel::DGUISystem;
using Duel::DInputManager;

namespace DemoKit
{
	DUEL_IMPLEMENT_RTTI_1(TestState, DAStateBase);

	TestState::TestState() :
		Duel::DAStateBase("TestState")
	{
		mTimer = new Duel::DTimer();
		mSceneInstance = new Duel::DSceneInstance("yooooo");
	}

	TestState::~TestState()
	{
		delete mSceneInstance;
		delete mTimer;
	}


	void TestState::initialize()
	{
		mTimer->reset();
		mRenderWindow = DApplication::getSingleton().getMainWindow();
		mGManager = DGUISystem::getSingleton().createGUIManager(mRenderWindow);
		mGManager->setInputEventQueue(DInputManager::getSingleton().registerWindow(mRenderWindow, false));

		Duel::DArchivePtr mediaArchive = Duel::DArchiveManager::getSingleton().
			getArchive("Assets\\_BasicMediaPack");
		if (mediaArchive != NULL)
		{
			// for textures.
			Duel::DDataStreamPtr descFilePtf = 
				mediaArchive->open("ShowcaseTexture.xml");
			if (descFilePtf != NULL)
			{
				Duel::ResourceDescriptionList resList =  Duel::DResourceGroupManager::getSingleton().
					getResouceManager("Texture")->parseParameter(descFilePtf.get());
				Duel::ResourceDescriptionList::iterator i, iend = resList.end();
				for (i = resList.begin(); i != iend; ++i)
				{
					Duel::DResourceGroupManager::getSingleton().declareResource(*i);
				}
			}				
			Duel::DResourceGroupManager::getSingleton().createDeclaredResource("_ShowcasePack");
		}


		Duel::DResourcePtr aTestMesh = Duel::DResourceGroupManager::getSingleton().
			getResouceManager("Mesh")->getResource("_BasicMediaPack", "a test.dm");
		aTestMesh->touch();

		Duel::DResourcePtr aTestSkel = Duel::DResourceGroupManager::getSingleton().
			getResouceManager("Skeleton")->getResource("_BasicMediaPack", "a test.ds");
		aTestSkel->touch();


		Duel::DResourcePtr reimuTex = Duel::DResourceGroupManager::getSingleton().
			getResouceManager("Texture")->getResource("_ShowcasePack", "reimu.dds");
		reimuTex->touch();

		Duel::DResourcePtr testNormal = Duel::DResourceGroupManager::getSingleton().
			getResouceManager("Texture")->getResource("_BasicMediaPack", "T_Test_Normal.dds");
		testNormal->touch();

		Duel::DTerrainPageData data;
		data.pageScale = 1.0f;
		data.batchSize = 17;
		data.pageSize = 65;

		// 咱们自己构造一个高度图.
		float* heightData  = new float[3*3];
		float* buf = heightData;
		for (int i = 0; i < 9; ++i)
		{
			*buf++ = 1.0f * i;
		}
		Duel::DImage2D *heightImg = new Duel::DImage2D();
		Duel::DImageDesc imgDesc;
		imgDesc.width = 3;
		imgDesc.height = 3;
		imgDesc.depth = 1;
		imgDesc.mipMapCount = 1;
		imgDesc.arraySize = 1;
		imgDesc.pixelFormat = Duel::PF_R32_Float;
		heightImg->loadFromData((char*)heightData, imgDesc);
		delete[] heightData;

		data.heightMap = heightImg;

		mSceneInstance->getTerrain()->generate(&data);

		delete heightImg;

		mTestBox = new Duel::DGSPictureBox(NULL);
		mTestBox->resize(600.0f,200.0f);
// 		mTestBox->setTexture(reimuTex);
//		mTestBox->setVerticalAlignement(Duel::AF_Top);
		//mTestBox->move(50.0f, 50.0f);
		mSubTestBox = new Duel::DGSPictureBox(mTestBox);
		mSubTestBox->setTexture(reimuTex);
//		mSubTestBox->setScalable(true);
		mSubTestBox->resize(640,480);
		mSubTestBox->move(200.0f, 0.0f);
//		mSubTestBox->setVerticalAlignement(Duel::AF_Top);
		//mSubTestBox->getTransformNode()->translateZ(-50.0f);
		///mTestBox->move(-99.0f, 0.0f);
		mGManager->addWidget(mTestBox);



		Duel::DMesh::SubMeshIterator sbi = aTestMesh->getAs<Duel::DMesh>()->getSubMeshIterator();
		while (sbi.hasMoreElements())
		{
			Duel::DSubMeshPtr sb = sbi.getNext();
			Duel::DMaterialInstancePtr mtl = Duel::DMaterialManager::getSingleton().createMaterialInstance("Lambert");
			sb->setMaterialInstance(mtl);
		}
		mSceneInstance->initialize(Duel::DAxisAlignedBox(-10.0f, -10.0f, -10.0f, 10.0f, 10.0f, 10.0f), 5.0f);
		mSceneInstance->getSceneCamera()->setEyePosition(0.0f,0.0f, -10.0f);
		mTestEntity = new Duel::DEntity("yooooooo");
		mTestEntity->loadFromMesh(aTestMesh);
		Duel::DEntity::SubEntityIterator ei = mTestEntity->getSubEntityIterator();
		while (ei.hasMoreElements())
		{
			Duel::DSubEntityPtr se = ei.getNext();
			Duel::DMaterialInstance::TextureConstant texConst;
			texConst.first = testNormal->getGroupName();
			texConst.second = testNormal->getName();
			se->getMaterialInstance()->setValue("NormalTexture", texConst);
		}
		Duel::DSceneNode* anode = mSceneInstance->getSceneManager()->createSceneNode("yoooooNode");
		anode->attachMovable(mTestEntity);
	}

	void TestState::release()
	{
		mSceneInstance->getSceneManager()->clearScene();
		delete mTestEntity;
		DGUISystem::getSingleton().destroyGUIManager(mRenderWindow);
		mGManager->setInputEventQueue(NULL);

		mGManager->removeWidget(mTestBox);
		delete mSubTestBox;
		delete mTestBox;
	}

	void TestState::parseInput()
	{
		mGManager->processInputEvent();
		// now there is no logic layer. pop all events.
		Duel::DInputEventQueue* eq = Duel::DInputManager::getSingleton().getEventQueue(mRenderWindow);
		eq->clearEvents();
	}

	void TestState::update()
	{
		mSceneInstance->update(mRenderWindow->getViewport());

		mGManager->update();
	}

	void TestState::render()
	{
		mRenderWindow->clear(Duel::CBM_Stencil|Duel::CBM_Color|Duel::CBM_Depth, 
			Duel::DColor(1.0f, 0.5f, 0.0f, 1.0f), 1.0f, 0);
		
		Duel::DRenderWorkshop* ws = Duel::DCore::getSingleton().getRenderWorkshop();

		// just write to final window.
		ws->setPresentTarget(mRenderWindow);
		ws->render(mSceneInstance->getRenderQueue());
		ws->render(mGManager->getRenderQueue());
		
	}

	void TestState::present()
	{
		mRenderWindow->update();
	}

	bool TestState::stateChangingFrom( Duel::DAStateBase* preState )
	{
		return true;
	}
}