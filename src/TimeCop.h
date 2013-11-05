/* das ist das Headerfile, in dem die Nachrichten definiert werden!
*/

// Grundfestlegungen

#ifndef TIMECOP_H
#define TIMECOP_H

#define MIN_BALKEN_WIDTH 	20

// Nachrichtennummern

#define MSG_WIN_CLOSE				'wcls'
#define MSG_OPEN_SHUTDOWN			'opsd'
#define MSG_OPEN_MIN_PER_HOUR		'opmh'
#define MSG_OPEN_HOUR_PER_WEEK		'ophw'
#define MSG_OPEN_HOUR_PER_YEAR		'ophy'
#define MSG_OPEN_LOGWINDOW			'oplg'
#define MSG_OPEN_ENDLESS_DAY		'oped'
#define MSG_OPEN_ENDLESS_WEEK		'opew'
#define MSG_OPEN_AVERAGE_UPTIME		'opau'
#define MSG_OPEN_AVERAGE_RUNTIME	'opar'

// Fensterindizes

enum {
	WIN_SHUTDOWN = 1,
	WIN_MPH,
	WIN_HPDW,
	WIN_HPDY,
	WIN_LOG,
	WIN_AVUP,
	WIN_AVRUN,
	WIN_ENDLDAY,
	WIN_ENDLWEEK
};

#endif
