// das ist die Funktion, die das Fenster behandelt, das
// zur Darstellung der Betriebszeit dient.
//
// Stunden pro Wochentag! Abgeleitet von BWindow

// hier ist der construktor! Er tut einfach nix, außer von BWindow abzuleiten!

#include "BalkenWin.h"
#include "StatistikView.h"
#include "ShadowBox.h"
#include "myLogfile.h"
#include "menu.h"
#include "TimeCop.h"

#include <stdio.h>

/* Das ist der Constructor!	*/

BalkenWin::BalkenWin(BRect frame, char* title, myLogfile *file, int balken_anzahl) 
	: BWindow(frame, title, B_TITLED_WINDOW, 0)

{
	BRect rect = frame;
	SeebaerMenuBar		*stdMenu;
	
	rect.OffsetTo(B_ORIGIN);
	stdMenu = new SeebaerMenuBar(rect, NULL);

	rect.top += 15.0;
	stdStatView = new StatistikView(rect, title, balken_anzahl);

	AddChild(stdMenu);
	AddChild(stdStatView);
	printf("End of BalkenWin Constructor\n");
}

//	nun kommt noch der Empfang der Nachricht QuitRequested!  

bool BalkenWin::QuitRequested()
{
	BMessage	msg(MSG_WIN_CLOSE);

	msg.AddInt32("kind", WIN_SHUTDOWN);
	msg.AddRect("pos", Frame());
	be_app->PostMessage(&msg);
	return true;
}
