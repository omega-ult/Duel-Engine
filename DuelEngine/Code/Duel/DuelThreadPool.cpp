//  [7/12/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelThreadPool.h"
#include "DuelThread.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DThreadPool, DObject);



	DThreadPool::DThreadPool( uint8 threadCount /*= 4*/ )
	{
		for (int i = 0; i< threadCount ;i++) 
		{
			mThreads.push_back(SharedPtr<DThread>(new DThread()));  
		}
	}

	DThreadPool::~DThreadPool()
	{

	}

	size_t DThreadPool::getThreadCount() const
	{
		return mThreads.size();
	}

	void DThreadPool::join()
	{
		WorkThreads::iterator i, iend = mThreads.end();
		for (i = mThreads.begin(); i != iend; i++)
		{
			(*i)->terminate();
			(*i)->join();
		}
	}

	void DThreadPool::waitForIdle()
	{
		int n_tasks = 0;  
		WorkThreads::iterator i, iend = mThreads.end();
		do  
		{  
			if (n_tasks)  
				DUEL_THREAD_SLEEP(200);  
			n_tasks = 0;  
			for (i = mThreads.begin(); i != iend; i++)
			{  
				n_tasks += (*i)->load();  
			};  
		}while (n_tasks); 
	}

	void DThreadPool::terminate()
	{
		WorkThreads::iterator i, iend = mThreads.end();
		for (i = mThreads.begin(); i != iend; i++)
		{
			(*i)->terminate();
		}
	}

	Duel::uint32 DThreadPool::load( uint8 threadIndex )
	{
		return (threadIndex>=mThreads.size()) ? 0 : mThreads[threadIndex]->load(); 
	}

	void DThreadPool::append( DFunction<void(void)> func )
	{
		uint8 nIdx = -1;  
		uint8 nMinLoad = -1;  
		uint8 tCount = mThreads.size();
		for (uint8 i = 0; i < tCount; i++)  
		{  
			if (nMinLoad > mThreads[i]->load())  
			{  
				nMinLoad = mThreads[i]->load();  
				nIdx = i;  
			}  
		}  

		assert(nIdx >= 0 && nIdx < tCount);  
		mThreads[nIdx]->append(func);  
	}

	void DThreadPool::start()
	{
		WorkThreads::iterator i, iend = mThreads.end();
		for (i = mThreads.begin(); i != iend; i++)
		{
			(*i)->start();
		}
	}

	/*
	 ≤‚ ‘≥Ã–Ú»Áœ¬:
	//a function which will be executed in sub thread.  
	void hello()  
	{  
	//sleep for a while  
	boost::this_thread::sleep(boost::posix_time::milliseconds(rand()%900+100));  
	std::cout <<  
	"Hello world, I'm a function runing in a thread!"  
	<< std::endl;  
	}  

	//a class has a method, which will be called in a thread different from the main thread.  
	class A  
	{  
	private:  
	int m_n;  
	public:  
	A(int n)  
	:m_n(n)  
	{}  
	~A(){}  
	public:  
	void foo (int k)  
	{  
	//sleep for a while  
	boost::this_thread::sleep(boost::posix_time::milliseconds(rand()%900+100));  
	std::cout <<"n*k = "<<k*m_n<<std::endl;  
	m_n++;  
	}  
	};  

	//let's test the thread.  
	int main()  
	{  
	cpp11_thread_pool thread(2);  
	srand((unsigned int)time(0));  
	A a(1),b(2),c(3);  
	int nsleep = rand()%900+100;  
	//append a simple function task  
	thread.append(&hello);  
	//append lamda  
	thread.append  
	(  
	[&nsleep]()  
	{  
	boost::this_thread::sleep(boost::posix_time::milliseconds(nsleep));  
	std::cout<<"I'm a lamda runing in a thread"<<std::endl;  
	}  
	);  
	//append object method with copy-constructor(value-assignment)  
	thread.append(std::bind(&A::foo,a,10));  
	thread.append(std::bind(&A::foo,b,11));  
	thread.append(std::bind(&A::foo,c,12));  
	thread.append(std::bind(&A::foo,a,100));  
	//append object method with address assignment, will cause the objects' member increase.  
	thread.append(std::bind(&A::foo,&a,10));  
	thread.append(std::bind(&A::foo,&b,11));  
	thread.append(std::bind(&A::foo,&c,12));  
	thread.append(std::bind(&A::foo,&a,100));  

	//wait for all tasks done.  
	thread.wait_for_idle();  
	//kill  
	thread.terminate();  
	//wait for killed  
	thread.join();  

	//test function  
	std::function < void (void) > func1 = &hello;  
	std::function < void (void) > func2 = &hello;  
	if (func1.target<void (void) >()!=func2.target<void (void)>())  
	return 0;  
	else  
	return 1;  
	}  
	*/
}