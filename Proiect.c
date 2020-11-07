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

#include <advanlys.h>
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

int panelHandle = 0;
double min, max, sum, median;
double alpha;
static int minInd, maxInd, count=0, filtered=0;



//==============================================================================
// Static functions

//==============================================================================
// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;
double *waveFiltered = 0;

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
			LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			Delay(1);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//afisare pe grapf
			PlotY(panel, PANEL_RAW_DATA_GRAPH, waveData+count*(int)sampleRate, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			if (filtered)
				PlotY(panel, PANEL_FILTERED_DATA_GRAPH, waveFiltered+count*(int)sampleRate, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
		
			MaxMin1D (waveData, npoints, &max, &maxInd, &min, &minInd);
			SetCtrlVal(panel, PANEL_MAX, max);
			SetCtrlVal(panel, PANEL_MIN, min);
			
			Sum1D (waveData, npoints, &sum);
			double dispersie = pow(sum, 2)/npoints;
			double medie=sum/npoints;
			SetCtrlVal(panel, PANEL_MEDIE, medie);
			dispersie = dispersie - pow(medie, 2);
			SetCtrlVal (panel, PANEL_DISPERSIA, dispersie);
			
			Median(waveData, npoints, &median);
			SetCtrlVal(panel, PANEL_MEDIANA, median);
			
			break;
	}
	return 0;
}


int CVICALLBACK ApplyCB (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int choice;
			filtered=1;
			DeleteGraphPlot(panel, PANEL_FILTERED_DATA_GRAPH, -1, VAL_IMMEDIATE_DRAW);
			waveFiltered = (double *) calloc(npoints, sizeof(double));
			GetCtrlVal(panel, PANEL_Filtru, &choice);
			if (choice ==16 || choice ==32)
			{	
				for (int i=0;i<npoints-choice;i++)
				{
					int sum2=0;
					for (int j=i;j<i+choice;j++)
						sum2=sum2+waveData[j];
					sum2=sum2/choice;
					waveFiltered[i]=sum2;
				}
			}
			else if (choice ==0){
				GetCtrlVal(panel, PANEL_ALPHA, &alpha);
	            for(int i = 1; i<npoints; i++)
	                waveFiltered[i]=(1-alpha)*waveFiltered[i-1]+alpha*waveData[i];
	      	}
				PlotY(panel, PANEL_RAW_DATA_GRAPH, waveData+count*(int)sampleRate, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			if (filtered)
				PlotY(panel, PANEL_FILTERED_DATA_GRAPH, waveFiltered+count*(int)sampleRate, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			break;
	}
	return 0;
}

int CVICALLBACK PrevCB (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				GetCtrlVal (panel, PANEL_Start, &count);
				if (count>1)
				{	
					SetCtrlVal(panel, PANEL_Start, count-1 );
					SetCtrlVal(panel, PANEL_Stop, count);
				}
				count=count-1;
				DeleteGraphPlot(panel, PANEL_RAW_DATA_GRAPH, -1, VAL_IMMEDIATE_DRAW);
				DeleteGraphPlot(panel, PANEL_FILTERED_DATA_GRAPH, -1, VAL_IMMEDIATE_DRAW);
				PlotY(panel, PANEL_RAW_DATA_GRAPH, waveData+count*(int)sampleRate, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				if (filtered)
					PlotY(panel, PANEL_FILTERED_DATA_GRAPH, waveFiltered+count*(int)sampleRate, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			break;
	}
	return 0;
}

int CVICALLBACK NextCB (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				GetCtrlVal(panel, PANEL_Stop, &count);
				if (count<5)
				{
					SetCtrlVal(panel, PANEL_Start, count );
					SetCtrlVal(panel, PANEL_Stop, count+1);
				}
				DeleteGraphPlot(panel, PANEL_RAW_DATA_GRAPH, -1, VAL_IMMEDIATE_DRAW);
				DeleteGraphPlot(panel, PANEL_FILTERED_DATA_GRAPH, -1, VAL_IMMEDIATE_DRAW);
				PlotY(panel, PANEL_RAW_DATA_GRAPH, waveData+count*(int)sampleRate, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				if (filtered)
					PlotY(panel, PANEL_FILTERED_DATA_GRAPH, waveFiltered+count*(int)sampleRate, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			break;
	}
	return 0;
}
