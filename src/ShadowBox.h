// Headerfile zu meinen Zeichenklassen

#ifndef _MY_SHADOWBOX
#define _MY_SHADOWBOX

#include <InterfaceKit.h>
#include <string.h>

class myShadowBox : public BView {

public:
	long		wert;				// enthält den Wert des Balkens
	long		view_div;			// enthält divisor, durch den der Balken geteilt werden muß um 
									// den tatsächlichen Wert zu erhalten
private:
	bool		calcNeeded;			// we have to calculate the StartingPoint
	char		*text;				// a Pointer to a textstring, which is displayed inside the Box
	rgb_color	textColor;			// we want to know, which color the Textstring should have
	float		textRotation;		// the direction of the TextDrawing
	BPoint		textStartPoint;		// the startingPoint where the Text should start
	
public:
				myShadowBox(BRect frame, char *name, char *titel, uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP, uint32 flags = B_WILL_DRAW); 
				~myShadowBox(void);
virtual	void	Draw(BRect updateRect);

void			SetRect(BRect rect);
void 			SetTextRotation(float rotation);

void			SetText(const char *string);	
const char		*Text(void)							{ return (text); }

void 			SetTextColor(rgb_color color)		{ textColor = color; }

private:
void			CalcTextPoint(void);
};

#endif