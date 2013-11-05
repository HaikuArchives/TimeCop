/*	Diese Datei dient dazu ein einheitliches Menü bereitszustellen,
	das in allen Fenstern gleich aussieht und einfach an jedes
	Fenster angehängt wird. Evtl. Unterschiede erkennt das Menü
	automatisch und richtet sich entsprechend danach	*/

#include "menu.h"
#include "TimeCop.h"
#include <AppKit.h>

SeebaerMenuBar::SeebaerMenuBar(BRect rect, char *titel)
		: BMenuBar(rect, titel)

{
	BMenu		*menu;
	BMenuItem	*menuitem;
	
	// Generieren des zuständigen Menüs
	
	menu = new BMenu("File");

	menuitem = new BMenuItem("save logfile", NULL, 'S');
	menuitem->SetEnabled(false);
	menu->AddItem(menuitem);

	menu->AddSeparatorItem();
	menuitem = new BMenuItem("About TimeCop", new BMessage(B_ABOUT_REQUESTED), 'A');
	menuitem->SetTarget(be_app);
	menu->AddItem(menuitem);
	
	menu->AddSeparatorItem();
	menuitem = new BMenuItem("Quit TimeCop", new BMessage(B_QUIT_REQUESTED), 'Q');
	menuitem->SetTarget(be_app);
	menu->AddItem(menuitem);

	AddItem(menu);

	menu = new BMenu("Window");

	menuitem = new BMenuItem("show logfile", new BMessage(MSG_OPEN_LOGWINDOW), 'l');
	menuitem->SetTarget(be_app);
	menu->AddItem(menuitem);

	menu->AddSeparatorItem();
	menuitem = new BMenuItem("minutes per hour (1 day)", new BMessage(MSG_OPEN_MIN_PER_HOUR), 'd');
	menuitem->SetTarget(be_app);
	menu->AddItem(menuitem);

	menuitem = new BMenuItem("hours per day (1 week)", new BMessage(MSG_OPEN_HOUR_PER_WEEK), 'w');
	menuitem->SetTarget(be_app);
	menu->AddItem(menuitem);


	menuitem = new BMenuItem("hours per day (1 year)", new BMessage(MSG_OPEN_HOUR_PER_YEAR), 'y');
	menuitem->SetTarget(be_app);
	menu->AddItem(menuitem);

	menu->AddSeparatorItem();
	
	menuitem = new BMenuItem("average runtime", new BMessage(MSG_OPEN_AVERAGE_RUNTIME), 'r');
	menuitem->SetTarget(be_app);
	menu->AddItem(menuitem);

	menuitem = new BMenuItem("average uptime", new BMessage(MSG_OPEN_AVERAGE_UPTIME), 'u');
	menuitem->SetTarget(be_app);
	menu->AddItem(menuitem);
	
	
	menuitem = new BMenuItem("shutdown reasons", new BMessage(MSG_OPEN_SHUTDOWN), 's');
	menuitem->SetTarget(be_app);
	menu->AddItem(menuitem);
	
	menu->AddSeparatorItem();

	menuitem = new BMenuItem("endless day", new BMessage(MSG_OPEN_ENDLESS_DAY), 'D');
	menuitem->SetTarget(be_app);
	menu->AddItem(menuitem);

	menuitem = new BMenuItem("endless week", new BMessage(MSG_OPEN_ENDLESS_WEEK), 'W');
	menuitem->SetTarget(be_app);
	menu->AddItem(menuitem);

	menuitem = new BMenuItem("endless month", NULL, 'M');
	menuitem->SetEnabled(false);
	menu->AddItem(menuitem);

	AddItem(menu);
}
