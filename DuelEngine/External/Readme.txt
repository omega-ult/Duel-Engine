Here is where external libs the Engine relies on located.
Generally, you don't have to download any libs, they are all here.
You may build them using your own configuration, I recommand you 
to read their building documentation.

But if you are new here, let me tell you how do I build them.

/// on Windows, with visual studio

---boost	
		1.	Download and extract the package, run bootstrap.bat, this method
			will build the bjam.exe file for further building. 
		2.	Copy the bjam.exe to ./tools/bcp, run bjam.exe. this will build bcp
			tool for extracting boost files we need.
		3.	Copy the bcp.exe in bin.v2/tools/bcp/[toolset]/[stage]/[thread_option]/
			to the boost root dir.
		4.	Create a new dir named bcpOutput in the boost root dir, then	execute command:
			"bcp.exe bind chrono signals2 thread smart_ptr .\bcpOutput", or just run 
			bcpCmd.bat which I provided.
		5.	Copy the bjam.exe and boostcpp.jam to ./bcpOutput we just created.
		6.	Run bjam.exe with specified configuration on your decision, or just run
			build_vc10.bat(vs2010) or build_vc11.bat(vs2012) I provided. 
		7.	Copy the contents in the ./bcpOutput to ./External/boost(create it
			if not exist).
		8.	Remove the comment BOOST_ALL_DYN_LINK in ./boost/config/user.hpp if you use the
			cmd I provided to build the boost.

rapidxml-1.13
		Just put all .hpp files into ./External/rapidxml-1.13/.

ois-v1-3
		Open ./Win32/ois.sln and compile it.
		
freetype-2.5.0.1
		1.	Open ./builds/win32/vc2010/freetype.sln.
		2.	Comment the ./include/freetype/config/ftconfig.h line 518~526,
		add "
		#ifdef   DLL_EXPORT
		#undef   DLL_EXPORT
		#define DLL_EXPORT   __declspec(dllexport)
		#else
		#define DLL_EXPORT   __declspec(dllimport)
		#endif /* !DLL_EXPORT */

		#ifndef FT_EXPORT

		#ifdef __cplusplus
		#define FT_EXPORT( x ) extern "C" DLL_EXPORT x
		#else
		#define FT_EXPORT( x ) extern DLL_EXPORT x
		#endif
		#endif
		".
		3.	Add "DLL_EXPORT" to the pre-processor defines in the project config.
		4.	Compile it using visual studio.

maya	
		Well... you need to install maya and copy the devkit's include and lib dir to the 
		./maya/[mayaVersion]/ dir. this lib is only relied by MayaExporter, You need to prepare
		it if you indeed have to use that.