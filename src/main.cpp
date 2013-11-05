// Das ist mein erstes Be-Programm
// es soll mal meinen Seebär für den ATARI ersetzen.
// mal schaun, wie einfach / schwer es werden wird!

#include "BalkenWin.h"
#include "LogWin.h"
#include "StatistikView.h"
#include "myLogfile.h"
#include "TimeCop.h"

class SeebaerApp : public BApplication {

private:
	myLogfile		*file;
	StdWoWin		*stdWoWin;
	BRect			rectWoWin;
	StdMoWin		*stdMoWin;
	BRect			rectMoWin;
	StdDownWin		*stdDownWin;
	BRect			rectDownWin;
	StdDayWin		*stdDayWin;
	BRect			rectDayWin;
	LogWin			*logWin;
	BRect			rectLogWin;
	AvRunWin		*avRunWin;
	BRect			rectAvRunWin;
	EndlDayWin		*endlDayWin;
	BRect			rectEndlDayWin;
	EndlWeekWin		*endlWeekWin;
	BRect			rectEndlWeekWin;
	
public:
					SeebaerApp();
					~SeebaerApp(void);
	void			AboutRequested(void);
	void			MessageReceived(BMessage* msg);
};

main (void)
{
	SeebaerApp *myApp;

	myApp = new SeebaerApp();
	myApp->Run();
	
	delete(myApp);
	return(0);
}

SeebaerApp::SeebaerApp() : BApplication("application/x-vnd.mp-timecop")
{
	file = new myLogfile();
	file->LoadToRam();
	
	BPath path;
	BFile setting;
	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	path.Append("TimeCop_settings");
	if (setting.SetTo(path.Path(), B_READ_ONLY) == B_NO_ERROR) {
		setting.Read(&rectWoWin, sizeof(rectWoWin));
		setting.Read(&rectMoWin, sizeof(rectMoWin));
		setting.Read(&rectDownWin, sizeof(rectDownWin));
		setting.Read(&rectDayWin, sizeof(rectDayWin));
		setting.Read(&rectLogWin, sizeof(rectLogWin));
		setting.Read(&rectAvRunWin, sizeof(rectAvRunWin));
		setting.Read(&rectEndlDayWin, sizeof(rectEndlDayWin));
		setting.Read(&rectEndlWeekWin, sizeof(rectEndlWeekWin));
		setting.Unset();
	} else {
		rectWoWin.Set(100,100,297,250);	
		rectMoWin.Set(120,120,460,270);	
		rectDownWin.Set(140,140,340,290);	
		rectDayWin.Set(160,160,840,310);	
		rectLogWin.Set(180,180,800,330);	
		rectAvRunWin.Set(200,200,700,350);	
		rectEndlDayWin.Set(220,220,320,370);	
		rectEndlWeekWin.Set(240,240,340,390);	
	}
	
	stdWoWin = new StdWoWin(rectWoWin, file);	// Create and View the Window
	stdWoWin->Show();
	
	stdMoWin = new StdMoWin(rectMoWin, file);	// Create and View the Window
	stdMoWin->Show();

	stdDownWin = new StdDownWin(rectDownWin, file);	// Create and View the Window
	stdDownWin->Show();

	stdDayWin = new StdDayWin(rectDayWin, file);	// Create and View the Window
	stdDayWin->Show();

	logWin = new LogWin(rectLogWin, file);	// Create and View the Window
	logWin->Show();

	avRunWin = new AvRunWin(rectAvRunWin, file);	// Create and View the Window
	avRunWin->Show();

	endlDayWin = new EndlDayWin(rectEndlDayWin, file);	// Create and View the Window
	endlDayWin->Show();

	endlWeekWin = new EndlWeekWin(rectEndlWeekWin, file);	// Create and View the Window
	endlWeekWin->Show();
}

SeebaerApp::~SeebaerApp(void) {

	BPath path;
	BFile setting;
	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	path.Append("TimeCop_settings");
	setting.SetTo(path.Path(), B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE);
	setting.Write(&rectWoWin, sizeof(rectWoWin));
	setting.Write(&rectMoWin, sizeof(rectMoWin));
	setting.Write(&rectDownWin, sizeof(rectDownWin));
	setting.Write(&rectDayWin, sizeof(rectDayWin));
	setting.Write(&rectLogWin, sizeof(rectLogWin));
	setting.Write(&rectAvRunWin, sizeof(rectAvRunWin));
	setting.Write(&rectEndlDayWin, sizeof(rectEndlDayWin));
	setting.Write(&rectEndlWeekWin, sizeof(rectEndlWeekWin));
	setting.Unset();

	delete file;
}

//--------------------------------------------------------------------

void SeebaerApp::MessageReceived(BMessage* msg)
{
	
	switch (msg->what) {
		case MSG_OPEN_SHUTDOWN:
			if (stdDownWin != NULL) stdDownWin->Activate();
			else {
				stdDownWin = new StdDownWin(rectDownWin, file);	// Create and View the Window
				stdDownWin->Show();
			}
			break;
		case MSG_OPEN_HOUR_PER_WEEK:
			if (stdWoWin != NULL) stdWoWin->Activate();
			else {
				stdWoWin = new StdWoWin(rectWoWin, file);	// Create and View the Window
				stdWoWin->Show();
			}
			break;
		case MSG_OPEN_HOUR_PER_YEAR:
			if (stdMoWin != NULL) stdMoWin->Activate();
			else {
				stdMoWin = new StdMoWin(rectMoWin, file);	// Create and View the Window
				stdMoWin->Show();
			}
			break;
		case MSG_OPEN_MIN_PER_HOUR:
			if (stdDayWin != NULL) stdDayWin->Activate();
			else {
				stdDayWin = new StdDayWin(rectDayWin, file);	// Create and View the Window
				stdDayWin->Show();
			}
			break;
		case MSG_OPEN_LOGWINDOW:
			if (logWin != NULL) logWin->Activate();
			else {
				logWin = new LogWin(rectLogWin, file);	// Create and View the Window
				logWin->Show();
			}
			break;
		case MSG_OPEN_AVERAGE_RUNTIME:
			if (avRunWin != NULL) avRunWin->Activate();
			else {
				avRunWin = new AvRunWin(rectAvRunWin, file);	// Create and View the Window
				avRunWin->Show();
			}
			break;
		case MSG_OPEN_ENDLESS_DAY:
			if (endlDayWin != NULL) endlDayWin->Activate();
			else {
				endlDayWin = new EndlDayWin(rectEndlDayWin, file);	// Create and View the Window
				endlDayWin->Show();
			}
			break;
		case MSG_OPEN_ENDLESS_WEEK:
			if (endlWeekWin != NULL) endlWeekWin->Activate();
			else {
				endlWeekWin = new EndlWeekWin(rectEndlWeekWin, file);	// Create and View the Window
				endlWeekWin->Show();
			}
			break;

		case MSG_WIN_CLOSE:
			switch (msg->FindInt32("kind")) {
				case WIN_SHUTDOWN:	
					stdDownWin = NULL; 
					msg->FindRect("pos", &rectDownWin);
					break;
				case WIN_MPH: 		
					stdDayWin = NULL; 
					msg->FindRect("pos", &rectDayWin);
					break;
				case WIN_HPDW:		
					stdWoWin = NULL; 
					msg->FindRect("pos", &rectWoWin);
					break;
				case WIN_HPDY:		
					stdMoWin = NULL; 
					msg->FindRect("pos", &rectMoWin);
					break;
				case WIN_LOG:		
					logWin = NULL; 
					msg->FindRect("pos", &rectLogWin);
					break;
				case WIN_AVRUN:		
					avRunWin = NULL; 
					msg->FindRect("pos", &rectAvRunWin);
					break;
				case WIN_ENDLDAY:		
					endlDayWin = NULL; 
					msg->FindRect("pos", &rectEndlDayWin);
					break;
				case WIN_ENDLWEEK:		
					endlWeekWin = NULL; 
					msg->FindRect("pos", &rectEndlWeekWin);
					break;
			}
			if (CountWindows() <= 1) be_app->PostMessage(B_QUIT_REQUESTED);
			break;

		default:
			BApplication::MessageReceived(msg);
			break;
	}
}

// If someone would know, what this program is for, and from whom it was programmed

void SeebaerApp::AboutRequested(void) {
	BAlert *alert = new BAlert("TimeCop", 
			"TimeCop 0.64ß (" B_UTF8_COPYRIGHT " 25.Apr.1999 by Michael Pieper)\n\n"
			"This program displays how long your computer is running BeOS\n\n"
			"Written by Michael Pieper\nmichael@augusta.de", "OK"); 
	alert->Go();
}
