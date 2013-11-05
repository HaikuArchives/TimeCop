#ifndef SEEBAER_BALKENWIN
#define SEEBAER_BALKENWIN

#include "myLogfile.h"
#include "StatistikView.h"
#include "menu.h"


// Diese Klasse definert ein fenster, in dem mehrere Balken
// enthalten sind, die als Statistikgruppe angesehen werden können

class BalkenWin : public BWindow {

private:

	int				win_kind;	// Enthält Wert um welches fenster es sich handelt

protected:
// in diesem teil stehen Variablen, auf die die ableitende Klasse
// zugreifen können muß.
// gerade stdStatView mit den Funktionen SetWert(e) ist wichtig!

	int				balken_anzahl; // Enthält den Wert der maximalen Balkenanzahl

	SeebaerMenuBar	*stdMenu;
	StatistikView	*stdStatView;
					
public:
// Constuructor mit Fenstergröße, Fenstertitel, dem Logfile und der Fensterkennung
					BalkenWin(BRect frame, char *title, myLogfile *file, int which_win); 

// QuitRequested muß nicht überall zur verfügung stehen!
	bool			QuitRequested(void);
};

/*================== Abgeleitete Klassen ===============================*/

class StdDownWin : public BalkenWin {
public:
					StdDownWin(BRect frame, myLogfile *file); 
};

class StdWoWin : public BalkenWin {
public:
					StdWoWin(BRect frame, myLogfile *file); 
};

class StdMoWin : public BalkenWin {
public:
					StdMoWin(BRect frame, myLogfile *file); 
};

class StdDayWin : public BalkenWin {
public:
					StdDayWin(BRect frame, myLogfile *file); 
};

class AvRunWin : public BalkenWin {
public:
					AvRunWin(BRect frame, myLogfile *file); 
};

class EndlDayWin : public BalkenWin {
public:
					EndlDayWin(BRect frame, myLogfile *file); 
};

class EndlWeekWin : public BalkenWin {
public:
					EndlWeekWin(BRect frame, myLogfile *file); 
};

#endif