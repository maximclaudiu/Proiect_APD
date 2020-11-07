#include <Proiect.c>

//==============================================================================
// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1

//==============================================================================
// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;

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
				PlotY(panel, PANEL_Raw_Data, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
			break;
	}
	
	return 0;
}

