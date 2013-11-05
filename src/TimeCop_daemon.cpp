/*-----------------------------------------------------------------
!	Diese Datei ist der TimeCop-Dämon. Dieses kleine Programm
!	tut nichts anderes, als beim Start die Datei TimeCop.log
!	zu überprüfen und die Startkennung hineinzuschreiben.
!
!	Jede Minute wird ein Touch gemacht
!	Beim beenden wird eine Endekennung geschrieben.
+----------------------------------------------------------------*/

#include "myLogfile.h"

class SeebaerDaemon : public BApplication {

public:
	myLogfile		*file;

public:
			SeebaerDaemon(void);
	void	Pulse(void);
	void	ReadyToRun(void);
	bool	QuitRequested(void);
};

main (void)
{
	SeebaerDaemon	*myD;
	
	myD = new SeebaerDaemon();
	myD->Run();

	delete myD;
	return(0);
}

SeebaerDaemon::SeebaerDaemon() : BApplication("application/x-vnd.mp-timecopd")
{
	SetPulseRate(60000000LL);	// Einmal die Minute einen Tick!	
	return;
}

// Funktion wird aufgerufen, wenn die Applikation initialisiert wurde

void SeebaerDaemon::ReadyToRun(void)
{
	file = new myLogfile();
	file->WriteStart();
}

bool SeebaerDaemon::QuitRequested(void)
{
	file->WriteShutdown();	// Registrieren des Shutdowns!
	delete file;

	return(true);			// Ende der Applikation bestätigen
}

// Funktion wird regelmäßig aufgerufenqaw

void SeebaerDaemon::Pulse(void)
{
	file->Touch();			// Modificationtime ändern!
}
