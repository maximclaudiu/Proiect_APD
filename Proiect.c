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

int acqPanel = 0;
int wavePanel=0;
double min, max, sum, median;
double alpha;
static int minInd, maxInd;
int count=0, filtered=0, filtered2=0;
int nrPoints=0;
int opt;
//==============================================================================
// Static functions

//==============================================================================
// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;

double *waveData = 0; //
double *waveFiltered = 0;
double *freqFiltered=0;
double *specterFiltered =0;
double *CosArray=0;

double ripple=0;
double searchFreq=0, df1=0;
double  freqPeak=0, powerPeak=0,
	freqPeak2=0, powerPeak2=0,*convertedSpectrum=NULL;

WindowConst winConst;
//==============================================================================
// Global functions

void MinMaxMed (int panel) {
	/// Calculul Min, Max, Medie, Dispersie, Mediana.
	
	/// Minim si Maxim.
	MaxMin1D (waveData+count*(int)sampleRate, sampleRate, &max, &maxInd, &min, &minInd);
	SetCtrlVal(panel, AcqPanel_MAX, max);
	SetCtrlVal(panel, AcqPanel_MIN, min);
	/// Medie si Dispersie.
	Sum1D (waveData+count*(int)sampleRate, sampleRate, &sum);
	double dispersie = pow(sum, 2)/npoints;
	double medie=sum/npoints;
	SetCtrlVal(panel, AcqPanel_MEDIE, medie);
	dispersie = dispersie - pow(medie, 2);
	SetCtrlVal (panel, AcqPanel_DISPERSIA, dispersie);
	/// Mediana.
	Median(waveData+count*(int)sampleRate, sampleRate, &median);
	SetCtrlVal(panel, AcqPanel_MEDIANA, median);
}

//==============================================================================

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;
	
	/// initialize and load resources 
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (acqPanel = LoadPanel (0, "Proiect.uir", AcqPanel));
	errChk (wavePanel = LoadPanel (0, "Proiect.uir", WavePanel));
	/// display the panel and run the user interface 
	errChk (DisplayPanel (acqPanel));
	errChk (RunUserInterface ());

Error:
	/// clean up 
	if (acqPanel > 0)
		DiscardPanel (acqPanel);
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

void FrequencyFilter (int mainPanel, double *waveData, int npoints)
{
	char unit[32]="V";
	double *autoSpectrum=NULL, *autoSpectrum2=NULL;
	double df=0;
	
	ScaledWindowEx(waveData,npoints,RECTANGLE_, 0, &winConst);
	if (autoSpectrum != NULL) 
		free(autoSpectrum);
	autoSpectrum = (double *) calloc(npoints, sizeof(double));
	if (convertedSpectrum != NULL)
		free(convertedSpectrum);
	convertedSpectrum = (double *) calloc(npoints, sizeof(double));
	
	AutoPowerSpectrum (waveData, npoints, 1, autoSpectrum, &df);
	PowerFrequencyEstimate( autoSpectrum, npoints, searchFreq, winConst, df, 100, &freqPeak, &powerPeak);
	SpectrumUnitConversion(autoSpectrum,npoints, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);
	PlotWaveform( mainPanel, WavePanel_Specter_Graph, convertedSpectrum+count*(int)sampleRate, sampleRate/4 ,VAL_DOUBLE, 1.0, 0.0, 0.0, 1, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
	
	if (filtered2) {
		df=0;
		if (autoSpectrum2 != NULL) 
			free(autoSpectrum2);
		autoSpectrum2 = (double *) calloc(npoints, sizeof(double));
		if (specterFiltered != NULL)
			free (specterFiltered);
		specterFiltered = (double *) calloc(npoints, sizeof(double));
		
		AutoPowerSpectrum(freqFiltered, npoints, 1, autoSpectrum2,&df);
		PowerFrequencyEstimate( autoSpectrum2, npoints, searchFreq, winConst, df, 100, &freqPeak2, &powerPeak2);
		SpectrumUnitConversion(autoSpectrum2,npoints, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, specterFiltered, unit);
		PlotWaveform( mainPanel, WavePanel_Specter_Graph_Filter, specterFiltered+count*(int)sampleRate, sampleRate/4 ,VAL_DOUBLE, 1.0, 0.0, 0.0, 1, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
	}
}

//==============================================================================

void updateWavePanel() {
	DeleteGraphPlot(wavePanel, WavePanel_Freq_Graph, -1, VAL_IMMEDIATE_DRAW);
	DeleteGraphPlot(wavePanel, WavePanel_Specter_Graph, -1, VAL_IMMEDIATE_DRAW);
	if (filtered2)
	{
		DeleteGraphPlot(wavePanel, WavePanel_Freq_Graph_Filter, -1, VAL_IMMEDIATE_DRAW);
		DeleteGraphPlot(wavePanel, WavePanel_Specter_Graph_Filter, -1, VAL_IMMEDIATE_DRAW);
	}
	FrequencyFilter(wavePanel,waveData,npoints);
	PlotY(wavePanel, WavePanel_Freq_Graph, waveData+count*(int)sampleRate, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
	if (filtered2)
		PlotY(wavePanel, WavePanel_Freq_Graph_Filter, freqFiltered+count*(int)sampleRate, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
}

//==============================================================================

void updateAcqPanel (){
	DeleteGraphPlot(acqPanel, AcqPanel_RAW_DATA_GRAPH, -1, VAL_IMMEDIATE_DRAW);
	if (filtered)
		DeleteGraphPlot(acqPanel, AcqPanel_FILTERED_DATA_GRAPH, -1, VAL_IMMEDIATE_DRAW);
	PlotY(acqPanel, AcqPanel_RAW_DATA_GRAPH, waveData+count*(int)sampleRate, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
	if (filtered)
		PlotY(acqPanel, AcqPanel_FILTERED_DATA_GRAPH, waveFiltered+count*(int)sampleRate, sampleRate,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
}

//==============================================================================

int CVICALLBACK OnLoadButtonAcqCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			/// Executa script python pentru conversia unui fisierului .wav in .txt
			LaunchExecutable("python main.py");
			
			/// Astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			Delay(1);
			
			/// Incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			/// Alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			
			/// Incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			for (int i=count*(int)sampleRate;i<sampleRate;i++)
				if ((waveData[i]>=0&&waveData[i+1]<=0)||(waveData[i]<=0&&waveData[i+1]>=0))
					nrPoints++;
			SetCtrlVal(wavePanel, WavePanel_NPoints, nrPoints);
			/// Afisare pe grapf
			updateAcqPanel();
			updateWavePanel();
			/// Afisare Min, Max, Medie, Dispersie, Mediana.
			MinMaxMed (acqPanel);
			
			break;
	}
	return 0;
}

//==============================================================================

int CVICALLBACK ApplyCB (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			int filtru;
			filtered=1;
			waveFiltered = (double *) calloc(npoints, sizeof(double));
			GetCtrlVal(acqPanel, AcqPanel_Filtru, &filtru);
			/// Alegerea optiunii de filtrare.
			if (filtru ==16 || filtru ==32)	{	
				/// Filtrele de mediere pe 16 si 32.
				for (int i=0;i<npoints-filtru;i++) {
					int sum2=0;
					for (int j=i;j<i+filtru;j++)
						sum2=sum2+waveData[j];
					sum2=sum2/filtru;
					waveFiltered[i]=sum2;
				}
			}
			else if (filtru ==0) {
				/// Filtrul de ordin I.
				GetCtrlVal(panel, AcqPanel_ALPHA, &alpha);
	            for(int i = 1; i<npoints; i++)
	                waveFiltered[i]=(1-alpha)*waveFiltered[i-1]+alpha*waveData[i];
	      	}
			/// Reafisarea noilor grafice.
			if (panel==acqPanel)
				updateAcqPanel();
			else updateWavePanel();
			break;
	}
	return 0;
}

//==============================================================================

int CVICALLBACK ApplyFreqCB (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int choice, window;
			filtered2=1;
			freqFiltered = (double *) calloc(npoints, sizeof(double));
			CosArray = (double *) calloc(100, sizeof(double));
			for (int i=0;i<100;i++)
				CosArray[i]=1;
			GetCtrlVal(wavePanel, WavePanel_Filtru_freq, &choice);
			if (choice==1) {
				Bw_LPF(waveData, npoints, sampleRate, sampleRate/6, 5, freqFiltered);
			}
			else {
				GetCtrlVal(wavePanel, WavePanel_Ripple, &ripple);
				Ch_LPF(waveData, npoints, sampleRate, sampleRate/8, ripple, 5, freqFiltered);
			}
			GetCtrlVal(wavePanel, WavePanel_Fereastra, &window);
			if (window==2){
				GenCosWin(freqFiltered,npoints, CosArray, 100);
			}
			else {
				KsrWin (freqFiltered,npoints, 3.86);
			}
			if (panel==acqPanel)
				updateAcqPanel();
			else updateWavePanel();
			break;
	}	
	return 0;
}

//==============================================================================

int CVICALLBACK PrevCB (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
				/// Verificare si actualizare pagina actuala.
				GetCtrlVal (panel, AcqPanel_Start, &count);
				if (count>=1) {	
					SetCtrlVal(acqPanel, AcqPanel_Start, count-1 );
					SetCtrlVal(acqPanel, AcqPanel_Stop, count);
					SetCtrlVal(wavePanel, WavePanel_Start, count-1 );
					SetCtrlVal(wavePanel, WavePanel_Stop, count);
					count=count-1;
				}
				/// Stergerea vechilor grafice.
				if (panel==acqPanel)
					updateAcqPanel();
				else updateWavePanel();
				/// Afisarea noilor Min, Max, Medie, Dispersie, Mediana. 
				MinMaxMed (acqPanel);
				nrPoints=0;
				
				for (int i=count*(int)sampleRate;i<count*(int)sampleRate+sampleRate;i++)
					if ((waveData[i]>=0&&waveData[i+1]<=0)||(waveData[i]<=0&&waveData[i+1]>=0))
						nrPoints++;
				SetCtrlVal(wavePanel, WavePanel_NPoints, nrPoints);
			break;
	}
	return 0;
}

//==============================================================================

int CVICALLBACK NextCB (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			/// Verificare si actualizare pagina actuala.
			GetCtrlVal(panel, AcqPanel_Stop, &count);
			if (count<5) {
				SetCtrlVal(acqPanel, AcqPanel_Start, count );
				SetCtrlVal(acqPanel, AcqPanel_Stop, count+1);
				SetCtrlVal(wavePanel, WavePanel_Start, count );
				SetCtrlVal(wavePanel, WavePanel_Stop, count+1);
			}
			/// Stergerea vechilor grafice.
			if (panel==acqPanel)
				updateAcqPanel();
			else updateWavePanel();
			/// Afisarea noilor Min, Max, Medie, Dispersie, Mediana. 
			MinMaxMed (acqPanel);
			nrPoints=0;
			for (int i=count*(int)sampleRate;i<count*(int)sampleRate+sampleRate;i++)
				if ((waveData[i]>=0&&waveData[i+1]<=0)||(waveData[i]<=0&&waveData[i+1]>=0))
					nrPoints++;
			SetCtrlVal(wavePanel, WavePanel_NPoints, nrPoints);
			break;
	}
	return 0;
}

int CVICALLBACK OnSwitchCB (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			if (panel==acqPanel) {
				SetCtrlVal(wavePanel,AcqPanel_Switch_Tabs,1);
				DisplayPanel(wavePanel);
				HidePanel(acqPanel);
				updateWavePanel();
			}
			else {
				SetCtrlVal(acqPanel,AcqPanel_Switch_Tabs,0);
				DisplayPanel(acqPanel);
				HidePanel(wavePanel);
				updateAcqPanel();
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnChangeRipple (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (wavePanel, WavePanel_Ripple, &ripple);
			SetCtrlVal (wavePanel, WavePanel_Ripple, ripple);
			break;
	}
	return 0;
}

int CVICALLBACK OnChangeFilter (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(wavePanel, WavePanel_Filtru_freq, &opt);
			if (opt==2)
				SetCtrlAttribute(wavePanel, WavePanel_Ripple, ATTR_VISIBLE, 1);
			else SetCtrlAttribute(wavePanel, WavePanel_Ripple, ATTR_VISIBLE, 0);
			
			break;
	}
	return 0;
}

