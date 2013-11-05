// das ist die Funktion, die das Fenster behandelt, das
// zur Darstellung der Betriebszeit dient.
//
// Stunden pro Wochentag! Abgeleitet von BWindow

// hier ist der construktor! Er tut einfach nix, außer von BWindow abzuleiten!

#include "BalkenWin.h"
#include "StatistikView.h"
#include "myLogfile.h"
#include "TimeCop.h"

#include <stdio.h>

/* Das ist der Constructor!	*/

BalkenWin::BalkenWin(BRect frame, char* title, myLogfile *file, int which_win) 
	: BWindow(frame, title, B_TITLED_WINDOW, 0)

{
	int			disp_text = DISPLAY_ORGINAL;
	BRect		rect = frame;
	BScrollView	*scv;
	time_t		tst, d;
	tm			*tim;
	long		dummy;
	
	tst = file->GetTimestamp(0, &dummy);
	win_kind = which_win;
	switch ( which_win ) {
		case WIN_SHUTDOWN:	balken_anzahl = 2;	disp_text = DISPLAY_ORGINAL; break;
		case WIN_MPH:		balken_anzahl = 24;	disp_text = DISPLAY_NO_HOUR; break;
		case WIN_HPDW:		balken_anzahl = 7;	disp_text = DISPLAY_MINUTES; break;
		case WIN_HPDY:		balken_anzahl = 12;	disp_text = DISPLAY_MINUTES; break;
		case WIN_AVRUN:		balken_anzahl = 12;	disp_text = DISPLAY_ORGINAL; break;
		case WIN_ENDLDAY:  
			if (tst > 0) {
				d = tst % (3600 * 24);	// errechnet die Sekundenzahl des ersten Tages
				balken_anzahl =  ((difftime (time(NULL), tst) + d) / (3600 * 24) ) + 1;
			} else balken_anzahl = -1;
			disp_text = DISPLAY_MINUTES;
			break;
		case WIN_ENDLWEEK:
			if (tst > 0) {
				tim = localtime(&tst);
				tst -= (tim->tm_wday * 86400); // Wochenanfang berechnen
				d = tst % (3600 * 24);	// errechnet die Sekundenzahl des ersten Tages
				balken_anzahl = ((difftime (time(NULL), tst)+d) / (3600 * 24 * 7)) + 1;
			} else balken_anzahl = -1;
			disp_text = DISPLAY_MINUTES;
			break;
	}
	
	rect.OffsetTo(B_ORIGIN);
	stdMenu = new SeebaerMenuBar(rect, NULL);
	AddChild(stdMenu);

	rect.top = stdMenu->Frame().bottom;
	rect.bottom -= B_H_SCROLL_BAR_HEIGHT;

	stdStatView = new StatistikView(rect, title, balken_anzahl);
	stdStatView->SetTextDisp( disp_text );

	scv = new BScrollView("scr", stdStatView, B_FOLLOW_ALL_SIDES, 0, true, false, B_NO_BORDER);
	AddChild(scv);

	if (which_win == WIN_ENDLDAY || which_win == WIN_ENDLWEEK) {
		stdStatView->SetTextRotation(90.0);
	}
}

// Der Anwender will das Fenster schließen, also Meldung an die App! um welches
// Fenster es sich handelt!

bool BalkenWin::QuitRequested()
{
	BMessage	msg(MSG_WIN_CLOSE);

	msg.AddInt32("kind", win_kind);
	msg.AddRect("pos", Frame());
	be_app->PostMessage(&msg);
	return true;
}


/*=============================================================================*/
/* Ab hier stehen die abgeleiteten Fensterklassen.                             */
/*=============================================================================*/

/* Das Fenster für die Anzeige der Shutdowngründe!	*/

StdDownWin::StdDownWin(BRect frame, myLogfile *file) 
	: BalkenWin(frame, "shutdown reasons", file, WIN_SHUTDOWN)

{
	unsigned long index;
	int i;
	char *titel[]={"crash","shutdown"};
	unsigned long divisor[]={1,1}, werte[]={0,0};

	index = 0;
	while ( (i = file->GetShutdown(index++)) >= 0) {
		switch ( i ) {
			case LOG_CRASH:	werte[0]++; break;
			case LOG_SHUTDOWN:werte[1]++; break;
		}
	}
	stdStatView->SetWerte(werte, divisor, titel);
}

/* Das Fenster für die Anzeige der Stunden pro Wochentag!	*/

StdWoWin::StdWoWin(BRect frame, myLogfile *file) 
	: BalkenWin(frame, "hours per day (1 week)", file, WIN_HPDW)

{
	unsigned long index, heute;
	long dauer, tag, start;
	int i, weekday;
	unsigned long wochen[]={1,1,1,1,1,1,1};
	unsigned long werte[]={0,0,0,0,0,0,0};
	char *titel[]={"SU", "MO","TU","WE","TH","FR","SA"};
	tm *tim;
	
	printf("Start WoWin \n");
	
	// Sekunden zählen und den einzelnen Zählern zuordnen!
	for (index = 0; (tim = file->GetTm(index, &dauer)) != NULL; index++) {
		heute = 86400 - (tim->tm_sec + (tim->tm_min * 60) + (tim->tm_hour * 3600));
		weekday = tim->tm_wday;
		while (dauer > 0L) {
			if (heute > dauer) heute = dauer;
			werte[weekday++] += heute;
			dauer -= heute;
			heute = 86400L;
			if (weekday >= 7) weekday = 0;
		}
	}

	// Divisor feststellen!
	tim = file->GetTm(0, &dauer); // Welcher Wochentag ist der erste Eintrag?
	if (tim != NULL) {
		weekday = tim->tm_wday;
		
		start = time(NULL);
		start -= start % (3600 * 24);
		
		tag = (difftime (start, file->GetFirstTimestamp()) / (3600 * 24)) + 1;
	
		for (i = 0; i < 7; i++) {
			wochen[weekday] = (tag / 7) + 1;
			if (--weekday < 0) weekday = 6;
			tag--;
		}
	} 
	
	stdStatView->SetWerte(werte, wochen, titel);
}

/* Das Fenster für die Anzeige der Stunden pro Tag in jedem Monat!	*/

StdMoWin::StdMoWin(BRect frame, myLogfile *file) 
	: BalkenWin(frame, "hours per day (1 year)", file, WIN_HPDY)

{
	long dauer, now;
	int i, index, month, leap;
	unsigned long werte[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	unsigned long tage[12]={1,1,1,1,1,1,1,1,1,1,1,1};
	char *titel[]={"JAN","FEB","MAR","APR","MAI","JUN","JUL","AUG","SEP","OCT","NOV","DEZ"};

	tm *tim, tim_next;
	time_t start, ende;

	int day_tab[2][12] = {                       /* Zur Berechnung des aktuellen Wochentages ist eine Tabelle nötig, die         */
		{31,28,31,30,31,30,31,31,30,31,30,31},   /* Beschreibt welcher Monat wieviel Tage hat. Außerdem steht noch die Anzahl    */
		{31,29,31,30,31,30,31,31,30,31,30,31}    /* der Tage im Jahr an erster Stelle. Tab 1 = normales Jahr Tab 2 = Schaltjahr  */
	};
	
	printf("Start MoWin \n");
	tim_next.tm_sec = tim_next.tm_min = tim_next.tm_hour = 0;
	tim_next.tm_mday = 1;

	for (index = 0; (tim = file->GetTm(index, &dauer)) != NULL; index++) {

		tim_next.tm_mon = tim->tm_mon;	// 1. des nächsten Monats!
		tim_next.tm_year = tim->tm_year;
		month = tim->tm_mon;
		start = mktime(tim);
		
		while (dauer > 0L) {
			month = tim_next.tm_mon;
			if (++tim_next.tm_mon >= 12) {
				tim_next.tm_mon = 0;
				tim_next.tm_year++;
			}
			ende = mktime(&tim_next);
			now = difftime (ende, start);
			start = ende;
			
			if (now > dauer) now = dauer;
			werte[month] += now;
			dauer -= now;
		}
	}

	tim = file->GetTm(0, &dauer); // Welcher Monat ist der erste Eintrag?
	if (tim != NULL) {
		dauer = (difftime (time(NULL), file->GetFirstTimestamp()) / (3600 * 24)) + 1;
		month = tim->tm_mon;	// Monatsindex 0-11
		i = tim->tm_year+1900;

		leap = (i%4 == 0 && i%100 != 0 || i%400 == 0);
		tage[month] = day_tab[leap][month]-tim->tm_mday;
		dauer -= tage[month];
		while (dauer > 0) {
			if (++month >= 12) {
				month = 0;
				i++;
				leap = (i%4 == 0 && i%100 != 0 || i%400 == 0);
			}
			tage[month] += day_tab[leap][month];
			dauer -= day_tab[leap][month];
		}
		if (dauer < 0) tage[month] += dauer;	// letzten Monat korrigieren
		
		for (i = 0; i < 12; i++) if (tage[i] <= 0) tage[i] = 1;
	}

	stdStatView->SetWerte(werte, tage, titel);
}

/* Das Fenster für die Anzeige der Zeit pro Stunde an einem Tag!	*/

StdDayWin::StdDayWin(BRect frame, myLogfile *file) 
	: BalkenWin(frame, "minutes per hour (1 day)", file, WIN_MPH)

{
	unsigned long index;
	long dauer, tage;
	int i, now;
	unsigned long werte[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	unsigned long div[24]  ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	char *titel[]={"00","01","02","03","04","05","06","07","08","09","10","11",
					 "12","13","14","15","16","17","18","19","20","21","22","23"};
	tm *tim;
	time_t tst;
	// Sekunden jeweils setzen

	printf("Start DayWin \n");

	for (index = 0; (tim = file->GetTm(index, &dauer)) != NULL; index++) {
		now = 3600 - (tim->tm_sec + (tim->tm_min * 60) );
		i = tim->tm_hour;
		while (dauer > 0L) {
			if (now > dauer) now = dauer;
			werte[i++] += now;
			dauer -= now;
			now = 3600;
			if (i >= 24) i = 0;
		}
	}

	// so genau gehts nicht, also überall den gleichen Divisor eintragen
	tst = file->GetTimestamp(0, &dauer);
	if (tst > 0) {		// dann gültiger Wert vorhanden!
		tage = (difftime (time(NULL), tst) / (3600 * 24)) + 1;
		for (i = 0; i < 24; div[i++] = tage) ;
	}

	stdStatView->SetWerte(werte, div, titel);
	printf("End DayWin \n");
}

/* Das Fenster für die Anzeige der Zeit pro Stunde an einem Tag!	*/

AvRunWin::AvRunWin(BRect frame, myLogfile *file) 
	: BalkenWin(frame, "average runtime", file, WIN_AVRUN)

{
	unsigned long index;
	long dauer;
	unsigned long werte[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	char *titel[]={	"<1m", "<5m", "<15m", "<30m", "<1h", "<2h", 
					"<6h", "<12h", "<1d", "<3d", "<1w", ">1w"};

	// Laufzeit setzen
	printf("Start AvRunWin \n");

	for (index = 0; file->GetTm(index, &dauer) != NULL; index++) {
		if (dauer < 60L) werte[0]++;
		else if (dauer < 300L) werte[1]++;
		else if (dauer < 900L) werte[2]++;
		else if (dauer < 1800L) werte[3]++;
		else if (dauer < 3600L) werte[4]++;
		else if (dauer < 7200L) werte[5]++;
		else if (dauer < 21600L) werte[6]++;
		else if (dauer < 43200L) werte[7]++;
		else if (dauer < 86400L) werte[8]++;
		else if (dauer < 259200L) werte[9]++;
		else if (dauer < 604800L) werte[10]++;
		else werte[11]++;
	}
	stdStatView->SetWerte(werte, NULL, titel);
}

/* Das Fenster für die Anzeige der Zeit pro Stunde an einem Tag!	*/

EndlDayWin::EndlDayWin(BRect frame, myLogfile *file) 
	: BalkenWin(frame, "endless day", file, WIN_ENDLDAY)

{
	unsigned long index;
	long dauer, day, sec;
	unsigned long *werte;
	char **titel;
	
	time_t time, start;
	tm	*tim;
     
	printf("Start EndlDayWin balkenanzahl:%d\n", balken_anzahl);

    if (balken_anzahl > 0) {
		werte = new unsigned long [ balken_anzahl ];
		titel = new char * [ balken_anzahl ] ;
		tim = file->GetTm(0, &dauer);
		if (tim != NULL) {
			for (day = 0; day < balken_anzahl; werte[day++] = 0) {
				titel[day] = new char [11];
				strftime(titel[day], 11, "%d.%m.%Y", tim); // 10 Zeichen + 1 0-Byte!!!
				time = mktime(tim) + 86400;
				tim = localtime(&time);
			}
		}
	
		// Laufzeit setzen
		time = file->GetTimestamp(0, &dauer);
		if (time > 0) {				// Nur abarbeiten, wenn gültiger Bereich vorhanden
			start = time / (3600 *24); // Ergibt die Starttage
			index = 1;
			while (time > 0) {
				day = (time / (3600 * 24)) - start;
				sec = 86400 - (time % 86400);
				do {
					if (day >= balken_anzahl) {
						printf("ERROR: %ld secs left and no day's any more\n", dauer);
						break;
					}
					if (sec > dauer) sec = dauer;
					werte[day++] += sec;
					dauer -= sec;
					sec = 86400;
				} while ( dauer > 0);
				time = file->GetTimestamp(index++, &dauer);
			}
		}
		stdStatView->SetWerte(werte, NULL, titel);
	
		delete [] werte;
		delete [] titel;
	}
}

/* Das Fenster für die Anzeige der Zeit pro Stunde an einem Tag!	*/

EndlWeekWin::EndlWeekWin(BRect frame, myLogfile *file) 
	: BalkenWin(frame, "endless week", file, WIN_ENDLWEEK)

{
	unsigned long index;
	long dauer, week, sec;
	unsigned long *werte;
	char **titel;
	
	time_t time, start;
	tm	*tim;
	
	printf("Start EndlWeekWin \n");

    if (balken_anzahl > 0) {
		werte = new unsigned long [ balken_anzahl ];
		titel = new char * [ balken_anzahl ] ;
		tim = file->GetTm(0, &dauer);
		if (tim != NULL) {
			time = mktime(tim);
			time -= (tim->tm_wday * 86400); // Wochenanfang berechnen
			time -= time % 86400;			// 0 Uhr berechnen
			start = time;
			
			tim = localtime(&time);
		
			for (week = 0; week < balken_anzahl; werte[week++] = 0) {
				titel[week] = new char [11];
				strftime(titel[week], 11, "%d.%m.%Y", tim);
				time = mktime(tim) + (86400 * 7); // um eine Woche erhöhen
				tim = localtime(&time);
			}
	
			// Laufzeit setzen
			time = file->GetTimestamp(0, &dauer);
			if (time > 0) {				// Nur abarbeiten, wenn gültiger Bereich vorhanden
				index = 1;
				while (time > 0) {
					week = (time - start) / (86400 * 7);
					sec = (86400 * 7) - (time % (86400 * 7));
					do {
						if (week >= balken_anzahl) {
							printf("ERROR: %ld secs left and no week's any more\n", dauer);
							break;
						}
						if (sec > dauer) sec = dauer;
						werte[week++] += sec;
						dauer -= sec;
						sec = 86400 * 7;
					} while ( dauer > 0);
					time = file->GetTimestamp(index++, &dauer);
				}
			}
			stdStatView->SetWerte(werte, NULL, titel);
		}
	
		delete [] werte;
		delete [] titel;
	}
}
