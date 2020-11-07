/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_LoadButton                 2       /* control type: command, callback function: OnLoadButtonCB */
#define  PANEL_Prev_Button                3       /* control type: command, callback function: (none) */
#define  PANEL_Next_Button                4       /* control type: command, callback function: (none) */
#define  PANEL_Stop                       5       /* control type: numeric, callback function: (none) */
#define  PANEL_Start                      6       /* control type: numeric, callback function: (none) */
#define  PANEL_Interval                   7       /* control type: textMsg, callback function: (none) */
#define  PANEL_Mean                       8       /* control type: numeric, callback function: (none) */
#define  PANEL_Filtered_Data_Graph        9       /* control type: graph, callback function: (none) */
#define  PANEL_Raw_Data_Graph             10      /* control type: graph, callback function: (none) */
#define  PANEL_Filtru                     11      /* control type: ring, callback function: (none) */
#define  PANEL_Dimensiune                 12      /* control type: numeric, callback function: (none) */
#define  PANEL_Aplica_Button              13      /* control type: command, callback function: (none) */
#define  PANEL_TEXTMSG                    14      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif