/*---------------------------------------------------------------
!	Das ist das Headerfile, in dem die Statistik-Class
!	definiert wird!
+--------------------------------------------------------------*/
#ifndef _StatistikView
#define _StatistikView

#include "ShadowBox.h"

#define DISPLAY_ORGINAL 0
#define DISPLAY_MINUTES 1
#define DISPLAY_SECONDS 2
#define DISPLAY_NO_HOUR 3

class StatistikView : public BView {

	private:
		myShadowBox		**balken;
		int				max_x, max_y, max_index;	// Zeigt wieviel Balken es maximal gibt
		long			max_height;		// Gibt die maximale Höhe der Balken an
		float			divisor;		// Gibt an, durch was geteilt werden muß, um auf Bildschirmhöhe anzupassen.
		int				display;		// Gibt an wie der Textwert angezeigt werden soll

		void			SetDivisor ( void );
		
	public:
		BStringView		**bsv;
	
		StatistikView ( BRect rect, char *name, const int x, const int y = 1, int resiceingMode = B_FOLLOW_ALL_SIDES );
		~StatistikView ( void );
		virtual void	FrameResized(float width, float height);

		void			Initialise (int neu);
		void			SetWert(unsigned long wert, unsigned long div, char *text, const int x, const int y = 0);
		void			SetWerte(unsigned long *wert, unsigned long *wert_div, char **text);
		void			SetTextRotation( float rotation );
		void			SetTextDisp( int display );
		void 			SetDispString(unsigned long wert, char *string);

		
		virtual	void	AttachedToWindow();

//virtual	void	Draw(BRect updateRect);

};

#endif