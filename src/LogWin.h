#ifndef SEEBAER_LOGWIN
#define SEEBAER_LOGWIN

#include "myLogfile.h"
#include <InterfaceKit.h>

class LogWin : public BWindow {

public:
					LogWin(BRect frame, myLogfile *file); 
	virtual	bool	QuitRequested();
};

#endif
