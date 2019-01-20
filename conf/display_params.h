/* Graphics settings*/

#ifndef DISPLAY_PARAMS_H
#define DISPLAY_PARAMS_H

#define COORD_PIXEL_RATIO 	2		// spatial ticks per pixel

#define VISUAL_UNIT (SPACE_UNIT / COORD_PIXEL_RATIO)

#define FIELD_VIS_W 		VISUAL_UNIT * HREF_POINTS
#define FIELD_VIS_H  		VISUAL_UNIT * VREF_POINTS

#define STATUS_W  			200
#define TOOLBAR_H 			100

#define DISP_WIDTH  		FIELD_VIS_W	+ STATUS_W  	// [pixels] width of the screen
#define DISP_HEIGHT 		FIELD_VIS_H + TOOLBAR_H		// [pixels] height of the screen

#define N_ICONS     		7
#define ICON_SIZE   		30	


#define COLOR_TOOLBAR_BORDER 	COLOR_BLACK
#define COLOR_TOOLBAR       	COLOR_GREYBLUE
#define COLOR_ICON_BORDER   	COLOR_BLACK
#define COLOR_STATUS_BORDER  	COLOR_BLACK
#define COLOR_STATUS_PANEL  	COLOR_GREYBLUE
#define COLOR_TEXT          	COLOR_WHITE



#endif	// DISPLAY_PARAMS_H

