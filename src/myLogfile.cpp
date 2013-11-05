/*-----------------------------------------------------------------
!	Die Klasse für das Logfile wird sowohl vom Daemon benötigt
!	als auch vom normalen Programm!
+----------------------------------------------------------------*/

#include "myLogfile.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

myLogfile::myLogfile(void) {
	find_directory(B_COMMON_LOG_DIRECTORY, &path);
	path.Append("TimeCop.log");
	mem = NULL;
	anzahl = 0L;
}

myLogfile::~myLogfile(void) {
	logfile.Unset();	// Falls es jemand vergessen sollte!
	if (mem != NULL) free(mem);	// Evtl. reserviertes Ram freigeben
}

// Die folgende Funktion positioniert auf das Ende und schreibt die Start-Informationen
// Ist der letzte Eintrag allerdings nicht identisch mit dem boot_timestamp, dann wird 
// zusätzlich ein CRASH-Eintrag geschrieben!

void myLogfile::WriteStart(void) {

	off_t			size;
	time_t			mod_time, boot_time;
	
	boot_time = (time_t )(real_time_clock() - (system_time() / 1000000L));
	Open();								// Datei öffnen

	logfile.GetSize(&size);
	if (size >= sizeof(log_info)) {
		logfile.Seek(sizeof(log_info)*-1LL, SEEK_END);		// Auf 1 Eintrag vor dem Ende positionieren !ACHTUNG mit -1LL multiplizieren!
	
		logfile.Read(&log_info, sizeof(log_info));		// letzten Eintrag lesen!
		log_info.reserve[0] = log_info.reserve[1] = log_info.reserve[2] = 0;

		if ((log_info.what == LOG_SHUTDOWN) && (boot_time < log_info.timestamp)) { // Evtl. nur Server beendet?
			if (size >= sizeof(log_info)*2) {
				logfile.Seek(sizeof(log_info)*-2LL, SEEK_END); // vorletzen Eintrag positionieren
				logfile.Read(&log_info, sizeof(log_info));
				if (log_info.what == LOG_START && log_info.timestamp == boot_time) {
					logfile.SetSize(size - sizeof(log_info));
				}
			}
		}


		if (log_info.what == LOG_START) {
			if (log_info.timestamp == 0) {	// hier ist mehr schief gelaufen geändert am 3.4.99
				logfile.Seek(sizeof(log_info)*-1LL, SEEK_CUR); // vorherigen Eintrag positionieren
				log_info.timestamp = boot_time;	// Bootzeit ist in der system_info-Struktur abgelegt
				log_info.what = LOG_START;
				logfile.Write(&log_info, sizeof(log_info));
			}
			if (log_info.timestamp != boot_time) {
				// Hier stimmt was nicht. Der letzte Eintrag darf nicht START mit falscher Zeit sein!
			
				logfile.GetModificationTime(&mod_time);	// Letzte Änderungszeit holen
				log_info.what = LOG_CRASH;
				log_info.timestamp = mod_time;	// jetzt wird der korrekte Timestamp eingetragen werden!
				logfile.Write(&log_info, sizeof(log_info));
			}
		}
	}

	if (size == 0 || log_info.what != LOG_START) {
		log_info.timestamp = boot_time;	// Bootzeit ist in der system_info-Struktur abgelegt
		log_info.what = LOG_START;
		logfile.Write(&log_info, sizeof(log_info));
	}
	
	logfile.Unset();							// Datei wieder schließen
}

// Die folgende Funktion positioniert auf das Ende und schreibt die aktuelle
// Uhrzeit als Shutdownkennung

void myLogfile::WriteShutdown(void) {

	Open();								// Datei ist noch nicht geöffnet!
	logfile.Seek(0LL, SEEK_END);		// Aufs Ende positionieren 
	log_info.timestamp = real_time_clock();
	log_info.what = LOG_SHUTDOWN;
	log_info.reserve[0] = log_info.reserve[1] = log_info.reserve[2] = 0;
	logfile.Write(&log_info, sizeof(log_info));
	logfile.Unset();					// Datei schließen
}

// Diese Funktion setzt das Änderungsdatum der Datei auf die aktuelle Zeit

void myLogfile::Touch(void) {
	Open();
	logfile.SetModificationTime(real_time_clock());
	logfile.Unset();
}

// Diese Funktion öffnet das File

void myLogfile::Open(void) {
	
	if (logfile.InitCheck() == B_NO_INIT) {
		logfile.SetTo(path.Path(), B_READ_WRITE | B_CREATE_FILE);
	}
}

// Die Funktion lädt das Logfile komplett in den Speicher
STAT_WERTE *	myLogfile::LoadToRam(void) {

	off_t size;
	
	Open();
	logfile.GetSize(&size);
	if (size > 0) {
		mem = (STAT_WERTE *) malloc(size);
		anzahl = size / sizeof(STAT_WERTE);
		if (mem != NULL) logfile.Read(mem, size);
	} else anzahl = -1;
	logfile.Unset();				// Datei wieder schließen
	return(mem);
}

// Diese Funktion liefert für den Eintrag "entry" zurück, ob er abgestuerzt ist oder
// ob ein normaler SHUTDOWN gefahren wurde

int myLogfile::GetShutdown(long entry) {

	entry *= 2;	// Entry verdoppeln, damit es dem Index entspricht!
	entry++;		// uns interessiert das Ende
	if (entry >= anzahl) return(-1);	// zu hoch gegriffen?
	else return(mem[entry].what);
}

// Diese Funktion liefert für den Eintrag "entry" die tm-Struktur zurück.
// Außerdem wird die Dauer berechnet!
// Bei Fehler wird NULL zurückgeliefert

tm *myLogfile::GetTm(long entry, long *dauer) {
	
	entry *= 2;	// Entry verdoppeln, damit es dem Index entspricht!
	if (entry > anzahl) {
		printf("return(NULL)\n");
		return(NULL);	// zu hoch gegriffen?
	}
	if (entry == anzahl-1) {				// letzter Eintrag?
		*dauer = (long ) difftime(time(NULL), mem[entry].timestamp);
	} else {
		*dauer = (long ) difftime(mem[entry+1].timestamp, mem[entry].timestamp);
	}
	
	return(localtime(&(mem[entry].timestamp)));
}

// Diese Funktion liefert für den Eintrag "entry" die tm-Struktur zurück.
// Außerdem wird die Dauer berechnet!
// Bei Fehler wird NULL zurückgeliefert

time_t myLogfile::GetTimestamp(long entry, long *dauer) {
	
	entry *= 2;	// Entry verdoppeln, damit es dem Index entspricht!
	if (entry > anzahl) return(0);			// zu hoch gegriffen?
	if (entry == anzahl-1) {				// letzter Eintrag?
		*dauer = (long ) difftime(time(NULL), mem[entry].timestamp);
	} else {
		*dauer = (long ) difftime(mem[entry+1].timestamp, mem[entry].timestamp);
	}
	
	return(mem[entry].timestamp);
} 

// Diese Funktion liefert den ersten Eintrag, oder die aktuelle Zeit, wenn keine Datei angelegt wurde!

time_t myLogfile::GetFirstTimestamp( void ) {

	if (mem != NULL) return (mem->timestamp);
	else return (time(NULL));
}

// Diese Funktion liefert für den Eintrag "entry" einen Textstring zurück.
// Es muß die maximale Länge übergeben werden, die der String haben darf.
// Der Übergebene String muß mindestens 50 Zeichen lang sein!

char *myLogfile::GetString(long entry, char *zeile, size_t max_len) {
	
	unsigned long dauer;
	entry *= 2;	// Entry verdoppeln, damit es dem Index entspricht!
	if (entry > anzahl || max_len < 50) return(NULL);	// zu hoch gegriffen?

	strftime(zeile, max_len, "%d.%m.%Y %H:%M:%S - ", localtime(&(mem[entry].timestamp)));

	if (entry == anzahl-1) {				// letzter Eintrag?
		dauer = (long ) difftime(time(NULL), mem[entry].timestamp);
		strcat(zeile, "--.--.---- --:--:-- - ");
	} else {
		dauer = (long ) difftime(mem[entry+1].timestamp, mem[entry].timestamp);
		strftime(&zeile[22], max_len-22, "%d.%m.%Y %H:%M:%S - ", localtime(&(mem[entry+1].timestamp)));
	}
	sprintf(&zeile[strlen(zeile)], "%ld:%.2ld:%.2ld", dauer / 3600, (dauer % 3600) / 60, dauer %60);
	
	return(zeile);
}
