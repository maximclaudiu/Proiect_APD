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

#define  AcqPanel                         1       /* callback function: panelCB */
#define  AcqPanel_Switch_Tabs             2       /* control type: binary, callback function: OnSwitchCB */
#define  AcqPanel_LoadButton              3       /* control type: command, callback function: OnLoadButtonAcqCB */
#define  AcqPanel_Prev_Button             4       /* control type: command, callback function: PrevCB */
#define  AcqPanel_Next_Button             5       /* control type: command, callback function: NextCB */
#define  AcqPanel_Stop                    6       /* control type: numeric, callback function: (none) */
#define  AcqPanel_Start                   7       /* control type: numeric, callback function: (none) */
#define  AcqPanel_Interval                8       /* control type: textMsg, callback function: (none) */
#define  AcqPanel_DISPERSIA               9       /* control type: numeric, callback function: (none) */
#define  AcqPanel_MEDIANA                 10      /* control type: numeric, callback function: (none) */
#define  AcqPanel_MEDIE                   11      /* control type: numeric, callback function: (none) */
#define  AcqPanel_MAX                     12      /* control type: numeric, callback function: (none) */
#define  AcqPanel_MIN                     13      /* control type: numeric, callback function: (none) */
#define  AcqPanel_FILTERED_DATA_GRAPH     14      /* control type: graph, callback function: (none) */
#define  AcqPanel_RAW_DATA_GRAPH          15      /* control type: graph, callback function: (none) */
#define  AcqPanel_Filtru                  16      /* control type: ring, callback function: (none) */
#define  AcqPanel_ALPHA                   17      /* control type: numeric, callback function: (none) */
#define  AcqPanel_Aplica_Button           18      /* control type: command, callback function: ApplyCB */

#define  WavePanel                        2       /* callback function: panelCB */
#define  WavePanel_Switch_tabs            2       /* control type: binary, callback function: OnSwitchCB */
#define  WavePanel_Freq_Graph_Filter      3       /* control type: graph, callback function: (none) */
#define  WavePanel_Prev_Button            4       /* control type: command, callback function: PrevCB */
#define  WavePanel_Next_Button            5       /* control type: command, callback function: NextCB */
#define  WavePanel_Stop                   6       /* control type: numeric, callback function: (none) */
#define  WavePanel_Start                  7       /* control type: numeric, callback function: (none) */
#define  WavePanel_Interval               8       /* control type: textMsg, callback function: (none) */
#define  WavePanel_Specter_Graph_Filter   9       /* control type: graph, callback function: (none) */
#define  WavePanel_Specter_Graph          10      /* control type: graph, callback function: (none) */
#define  WavePanel_Freq_Graph             11      /* control type: graph, callback function: (none) */
#define  WavePanel_NPoints                12      /* control type: numeric, callback function: (none) */
#define  WavePanel_Ripple                 13      /* control type: numeric, callback function: OnChangeRipple */
#define  WavePanel_Aplica_Button          14      /* control type: command, callback function: ApplyFreqCB */
#define  WavePanel_Fereastra              15      /* control type: ring, callback function: (none) */
#define  WavePanel_Filtru_freq            16      /* control type: ring, callback function: OnChangeFilter */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ApplyCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ApplyFreqCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NextCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnChangeFilter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnChangeRipple(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonAcqCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PrevCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif