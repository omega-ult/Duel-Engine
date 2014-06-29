//  [4/11/2013 OMEGA] created


#include "pch.h"
#include "Duel.h"
#include "DuelGUI.h"
#include "DuelApplication.h"
#pragma comment(lib, "Duel_d.lib")
#pragma comment(lib, "DuelGUI_d.lib")


int main()
{
	Duel::DApplication app;
	app.initialize("config.xml");
	// Now we're ready to receive and process Windows messages.
	bool bGotMsg;
	MSG  msg;
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
	ShowCursor(NULL);

	while( WM_QUIT != msg.message )
	{
		// Use PeekMessage() so we can use idle time to render the scene. 
		bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );

		if( bGotMsg )
		{
			// Translate and dispatch the message
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			app.run();
		}
	}
	app.shutdown();
	return 0;
}