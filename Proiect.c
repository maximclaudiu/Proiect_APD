//==============================================================================
//
// Title:		Proiect
// Purpose:		A short description of the application.
//
// Created on:	20-Oct-20 at 4:26:52 PM by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include <cvirte.h>		
#include <formatio.h>
#include <userint.h>
#include "Proiect.h"
#include "toolbox.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle = 0;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;

//==============================================================================
// Global functions

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (panelHandle = LoadPanel (0, "Proiect.uir", PANEL));
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (panelHandle));
	errChk (RunUserInterface ());

Error:
	/* clean up */
	if (panelHandle > 0)
		DiscardPanel (panelHandle);
	return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
	{
		QuitUserInterface (0);	
		
		free(waveData);
	}
	return 0;
}

//==============================================================================
// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1

//==============================================================================

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		
	case EVENT_COMMIT:
				//executa script python pentru conversia unui fisierului .wav in .txt
				LaunchExecutable("main.py");
				
				//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
				Delay(4);
				
				//incarc informatiile privind rata de esantionare si numarul de valori
				FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
				sampleRate = waveInfo[SAMPLE_RATE];
				npoints = waveInfo[NPOINTS];
				
				//alocare memorie pentru numarul de puncte
				waveData = (double *) calloc(npoints, sizeof(double));
				
				//incarcare din fisierul .txt in memorie (vector)
				FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
				
				//afisare pe grapf
				PlotY(panel, PANEL_Raw_Data_Graph, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
			break;
	}
	return 0;
}