/*	Diese Datei erzeugt das Logfenster in dem die Zeiten
	in Klartext enthalten sind.	*/
	
#include "LogWin.h"
#include "menu.h"
#include "TimeCop.h"

/* Das ist der Constructor!*/
// Es wird ein Zeiger auf das Logfile übergeben!

LogWin::LogWin(BRect frame, myLogfile *file) 
	: BWindow(frame, "Logfile", B_TITLED_WINDOW, 0)

{
	BRect 				rect = frame;
	SeebaerMenuBar		*stdMenu;
	BListView			*bol_view;
	BScrollView			*scv;
	unsigned long 		index;
	char				zeile[128];
	
	rect.OffsetTo(B_ORIGIN);
	stdMenu = new SeebaerMenuBar(rect, NULL);
	AddChild(stdMenu);

	rect = frame;
	rect.OffsetTo(B_ORIGIN);
	rect.top = stdMenu->Frame().bottom+1;
	rect.right -= B_V_SCROLL_BAR_WIDTH;

	bol_view = new BListView(rect, "view", B_MULTIPLE_SELECTION_LIST, B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE); 

	scv = new BScrollView("scr", bol_view, B_FOLLOW_ALL_SIDES, 0, false, true);
    
	for (index = 0; (file->GetString(index, zeile, 128)) != NULL; index++) {
		bol_view->AddItem( new BStringItem(zeile) );
	}
	AddChild(scv);
    scv->SetFont(be_fixed_font);
}

//	nun kommt noch der Empfang der Nachricht QuitRequested!  

bool LogWin::QuitRequested()
{
	BMessage	msg(MSG_WIN_CLOSE);

	msg.AddInt32("kind", WIN_LOG);
	msg.AddRect("pos", Frame());
	be_app->PostMessage(&msg);
	return true;
}
