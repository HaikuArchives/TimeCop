#ifndef _myLogfile
#define _myLogfile

#define LOG_START 0			// Werte für what!
#define LOG_SHUTDOWN 1
#define LOG_CRASH 2
#define LOG_ENDLISTE 0xFF		// Ende der geladenen Liste

#include <time.h>
#include <StorageKit.h>

typedef struct {
		time_t		timestamp;		// enthält die Uhrzeit in Sekunden
		char		what;			// legt fest, ob es ein Absturz, ein Neustart oder ein Shutdown war
		char		reserve[3];		// damit wir auf 64 Bit = 4 Byte kommen!
} STAT_WERTE;

class myLogfile {
	public:
					myLogfile(void);
					~myLogfile(void);
	void			WriteStart(void);	// schreibt ein Startobjekt in die Datei
	void			WriteShutdown(void);	// schreibt ein Endeobjekt in die Datei
	void			Touch(void);		// Ändert die ModificationTime auf die aktuelle Zeit
	STAT_WERTE *	LoadToRam(void);	// Lädt die logdatei in den Speicher und gibt die Adresse zurück
	int				GetShutdown(long entry);
	tm *			GetTm(long entry, long *dauer);
	time_t			GetTimestamp(long entry, long *dauer);
	char *			GetString(long entry, char *zeile, size_t max_len);
	time_t			GetFirstTimestamp( void );

	private:
	void			Open(void);					

	public:
		long anzahl;			// Anzahl der Datensätze

	private:
		BPath		path;			// Hier merken wir uns den Pfad und den Dateinamen!
		BFile		logfile;		// Enthält das BFile Objekt!

		STAT_WERTE 	*mem;			// Zeiger auf evtl. gelandene Logdatei
		STAT_WERTE	log_info;		// Hier werden die Werte abgelegt
};

#endif
