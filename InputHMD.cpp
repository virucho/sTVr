/************************************************************************************ 
 * Copyright (c) 2014-2015, TU Ilmenau
 * 
 * Build with Irrlicht framework
 * STVR use:
 *                  Vuzix SDK 3.3
 *					Irrlicht 1.8
 *                  IrrEdit 4.5
 *					DirectX 9
 * Viel Dank guys
 * ===================================================================================
 * Authors:  Luis Rojas (luis-alejandro.rojas-vargas@tu-ilmenau.de) 
 *************************************************************************************/

/****************************************************************/
/*                        Include                               */
/****************************************************************/

#include "InputHMD.h"

/****************************************************************/
/*                        Global                                */
/****************************************************************/

InputHMD* MgrHMD;

/****************************************************************/
/*                       Functions								*/
/****************************************************************/

InputHMD::InputHMD(bool onStereo, bool onTracking)
{
	this->ActStereo = onStereo;
	this->ActTracking = onTracking;

	VRTracker = NULL;
	VRStereo = NULL;
}

bool InputHMD::InitHMD()
{
	if(ActTracking)
	{
		//Inicio las gafas
		VRTracker = new iWearTracker();
		VRTracker->Initialize();
		VRTracker->SetFilterState(true);
	}

	if(ActStereo)
	{
		//Start Stereo
		VRStereo = new iWearStereo3D();
		VRStereo->ActiveStereo();
	}

	return true;
}



