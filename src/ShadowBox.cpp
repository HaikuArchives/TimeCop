// Mehrere von BView abgeleitete Klassen, die ein bestimmtes Objekt malen!

// myShadowBox zeichnet eine angedeutete 3D-Box mit einem Textobjekt!
// das Textobjekt hängt am unteren Rand der Box!

#include "ShadowBox.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//=======================================================================================
// We define here the Rectangular, which is exactly one Box.
//
	myShadowBox::myShadowBox(BRect rect, char *name, char *titel, uint32 resizingMode, uint32 flags)
	   	   : BView(rect, name, resizingMode, flags)
{
	wert = 0;
	view_div = 1;
	text = NULL;
	calcNeeded = false;
}

// Der Destructor löscht das Textfeld!

	myShadowBox::~myShadowBox(void)
{
	if (text != NULL) free(text);
}

//=====================================================================================
// This function copys the text and calculates the Starting Point of the DrawString
//
void myShadowBox::SetText(const char *string)
{ 
	text = strdup(string);
	calcNeeded = true;
}

//======================================================================================
// This function calculates the StartingPoint of the Texttring depending on the String
// and the Rotation!
//
void myShadowBox::CalcTextPoint(void)
{
	BRect grect;
	font_height	fnth;

	GetFontHeight(&fnth);		// the Hight of our Font
	grect = Bounds();			// and we want to know, which rectangular our Rect is.

	if (textRotation == 0) {
		textStartPoint.Set ((grect.Width() - StringWidth(text))/2, grect.bottom - (fnth.descent+ 2));
	} else {
		textStartPoint.Set (fnth.ascent + fnth.descent, grect.Height() - 2);
	}
	calcNeeded = false;
}

// Diese Funktion setzt den View auf das gewünschte Rechteck

void myShadowBox::SetRect(const BRect rect)
{
	if (rect.LeftTop() != Frame().LeftTop()) MoveTo(rect.left, rect.top);
	if (rect.RightBottom() != Frame().RightBottom()) ResizeTo(rect.Width(), rect.Height());	
	calcNeeded = true;
}

void myShadowBox::SetTextRotation(float rotation)
{
	BFont font;

	textRotation = rotation;
	font.SetRotation(rotation); 
		
	SetFont(&font, B_FONT_ROTATION );
	calcNeeded = true;
}

//=============================================================================================
// This function draws the Box which is needed to display all

void myShadowBox::Draw(BRect rect)
{
	BPoint p1, p2, p3, p4;
	BRect grect;
	rgb_color color, high, low;

	color = ViewColor();							// lets ask, which color the View has!
	high = tint_color(color, B_LIGHTEN_2_TINT);		// we need a light color
	low = tint_color(color, B_DARKEN_2_TINT);		// and also a dark one

	grect = Bounds();								// and we want to know, which rectangular our Box is.

	if (calcNeeded) CalcTextPoint();
	SetHighColor(textColor);	// set the TextColor
	SetLowColor(color);			// and the AntialiasingColor
	MovePenTo(textStartPoint);	// Move the Pen to the correct position
	DrawString(text);			// and print the String

	p1.Set(grect.right, grect.top);		// then lets do the small 3-d effect
	p2.Set(grect.right, grect.bottom);
	p3.Set(grect.left, grect.bottom);
	p4.Set(grect.left, grect.top);
	
	BeginLineArray(4);	// Begins drawing about 4 Lines!
	AddLine(p1, p2, low);
	AddLine(p2, p3, low);
	AddLine(p3, p4, high);
	AddLine(p4, p1, high);
	EndLineArray();
}

