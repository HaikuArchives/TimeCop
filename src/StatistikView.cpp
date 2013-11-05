/*--------------------------------------------------------------
!	Hier wird ein Objekt definiert, das einen Statistikwert
!	festhält. es ist ein Zewidimensionales Feld angebbar, das
!	die Werte enthält
+-------------------------------------------------------------*/

#include "StatistikView.h"
#include "ShadowBox.h"
#include "TimeCop.h"

#include <stdio.h>

// Der Constructor für die Angabe eines Zweidimmensionalen Feldes

StatistikView::StatistikView(BRect rect, char *name, const int x, const int y, int resiceingMode)
	: BView(rect, name, resiceingMode, B_WILL_DRAW | B_FRAME_EVENTS)
{
	display = DISPLAY_ORGINAL;
	
	max_x = (x > 1) ? x : 1;
	max_y = (y > 1) ? y : 1;
	
	if (x > 0 && y > 0) {
		max_index = x*y;
		balken = new myShadowBox  * [max_index] ;	// Speicherplatz für Zeiger reservieren
		bsv = new BStringView * [max_index];		// Speicherplatz für Zeiger auf Textviews reservieren
	} else {
		balken = NULL;
		bsv = NULL;
	}
}


// Der Destructor löscht wieder den Speicher des Feldes!

StatistikView::~StatistikView(void)

{
	if (balken != NULL) delete [] balken;
	if (bsv != NULL) delete [] bsv;
}

void StatistikView::SetTextRotation(float rotation)

{
	int			index;
	if (balken != NULL) {
		for (index = 0; index < max_index; balken[index++]->SetTextRotation(rotation)) ;
	}
}

void StatistikView::AttachedToWindow(void)

{
	Initialise(true);
}

void StatistikView::FrameResized(float width, float height)

{
	Initialise(false);
}

// Diese Funktion setzt die Breite der Balken

void StatistikView::Initialise(int neu) 

{
	font_height		f_h;
	int				fh, height;
	int				i, width, left_offset;
	BRect 			r, rect;
	BScrollBar		*bsb;
	
	r = rect = Bounds();

	GetFontHeight(&f_h);
	fh = (f_h.ascent + f_h.descent);
	max_height = ((r.Height()) - fh);
   
	bsb = ScrollBar(B_HORIZONTAL);
	width = (rect.right - rect.left) / max_x;
	if (width < MIN_BALKEN_WIDTH) {
		width = MIN_BALKEN_WIDTH;
		if (bsb != NULL) {
			bsb->SetRange(0, (width * max_x)-(rect.Width()));
			bsb->SetProportion((rect.Width()) / (width * max_x));
			bsb->SetSteps(MIN_BALKEN_WIDTH / 2, rect.Width() - (MIN_BALKEN_WIDTH / 2));
		}
		left_offset = 0 - LeftTop().x;
	} else {
		if (bsb != NULL) bsb->SetRange(0,0);
		left_offset = ((rect.Width()) - (width * max_x)) / 2;
	}
	
	if (!neu) SetDivisor();

	Hide();
	for (i = 0; i < max_index; i++) {
		r.Set(left_offset+2, 0, left_offset - 1 + width, max_height);
		left_offset += width;
		if (neu) {
			balken[i] = new myShadowBox(r, NULL, NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP);
			balken[i]->SetViewColor(240, 0, 0);
			AddChild(balken[i]);
		} else {
			height = (balken[i]->wert / balken[i]->view_div) / divisor;
			r.top = max_height - height;
			balken[i]->SetRect(r);
		}
		// Stringview unterhalb des Balken adjustieren
		r.top = r.bottom + 2;
		r.bottom = r.top + fh;
		if (neu) {
			bsv[i] = new BStringView(r, NULL, "0");
			(bsv[i])->SetAlignment(B_ALIGN_CENTER);
			AddChild(bsv[i]);
		} else {
			(bsv[i])->MoveTo(r.left, r.top);
			(bsv[i])->ResizeTo(r.right - r.left, r.bottom - r.top);
		}
	}
	Show();
}

// Eine Funktion um beliebige Werte zu setzen!
// wert_div <= 0 veränder den div-Wert nicht

void StatistikView::SetWert(unsigned long wert, unsigned long wert_div, 
							char *text, const int x, const int y)

{
	int i;
	int height;
	float old_div = divisor;
	int index = x + (max_y * y);
	BRect rect;
	char string[50];

	if (index >= max_index) {
		printf("ERROR! index %d i bigger then maxindex %d\n", index, max_index);
		return;
	}

	old_div = divisor;
	if (x <= max_x && y <= max_y && x >= 0 && y >= 0) 	{
		balken[index]->wert = wert;
		if (wert_div > 0) balken[index]->view_div = wert_div;
	}
	
	Hide();
	if (text != NULL) balken[index]->SetText(text);
	if (old_div != divisor) {			// Fenstergröße hat sich geändert! also alle Balken zeichnen
		for (i = 0; i < max_index; i++) {
			height = (balken[i]->wert / balken[i]->view_div) / divisor;
			rect = balken[i]->Frame();
			rect.top = max_height - height;
			balken[i]->SetRect(rect);
			SetDispString(balken[i]->wert / balken[i]->view_div, string);
			bsv[i]->SetText(string);
		}
	} else {							// ansonsten nur den einzelnen Balken!
		height = (wert / wert_div) / divisor;
		rect = balken[index]->Frame();
		rect.top = max_height - height;
		balken[index]->SetRect(rect);
		SetDispString(balken[index]->wert / balken[index]->view_div, string);
		bsv[index]->SetText(string);
	}
	Show();
}

// Eine Funktion um alle Werte zu setzen!
// Es ist wichtig auch alle Werte zu übergeben!

void StatistikView::SetWerte(unsigned long *wert, unsigned long *wert_div, char **text)

{
	int i, height;
	BRect rect, r;
	char string[50];
	
	for (i = 0; i < max_index; i++ ) {
		balken[i]->wert = wert[i];
		balken[i]->view_div = (wert_div == NULL ? 1 : wert_div[i]);
		if (text != NULL) balken[i]->SetText(text[i]);
	}

	SetDivisor();

	Hide();

	for (i = 0; i < max_index; i++) {
		height = (balken[i]->wert / balken[i]->view_div) / divisor;
		rect = balken[i]->Frame();
		rect.top = max_height - height;

		balken[i]->SetRect(rect);

		SetDispString(balken[i]->wert / balken[i]->view_div, string);
		bsv[i]->SetText(string);


	}
	
	Show();
}

// Setzt den Divisor für die Textanzeige und die Rotation des textes

void StatistikView::SetTextDisp(int disp)
{
	display = disp;
}

void StatistikView::SetDispString(unsigned long wert, char *string)

{
	int s,m;
	long h;
	s = wert % 60;
	m = (wert % 3600) / 60;
	h = wert / 3600;
	
	switch (display) {
		case DISPLAY_MINUTES:
			sprintf(string, "%ld:%.2d", h, m);
			break;
		case DISPLAY_SECONDS:
			sprintf(string, "%ld:%.2d:%.2d", h, m, s);
			break;
		case DISPLAY_NO_HOUR:
			m = wert / 60;
			sprintf(string, "%d:%.2d", m, s);
			break;			
		case DISPLAY_ORGINAL:
			sprintf(string, "%ld", wert);
			break;
	}
}

// Die folgende Funkion errechnet den Divisor
// Das Fenster muß bereits gelockt sein!

void StatistikView::SetDivisor(void)

{
	int i;
	BRect rect;
	unsigned long max = 0;
	
	for (i = 0; i < max_index; i++) {
		max = max > (balken[i]->wert / balken[i]->view_div) ? max : (balken[i]->wert / balken[i]->view_div);
	}
	
	rect = this->Frame();
	divisor = (float )max / (max_height - 10);
	if (divisor <= 0) divisor = 1;
}
