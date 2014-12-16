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

#ifndef __C_INPUTHMD_H_INCLUDED__
#define __C_INPUTHMD_H_INCLUDED__

/****************************************************************/
/*                        Defines                               */
/****************************************************************/

//Iwear Defines
#define _IWEAR_ACTIVED_

/****************************************************************/
/*                        Include                               */
/****************************************************************/

//Haupt Bibliotheken von Iwear VR920
#include "IWear_Tracking.h"		//Bibliothek zu Tracking

using namespace Iwear;
using namespace Tracking;
using namespace Stereo3D;

/****************************************************************/
/*                         Class                                */
/****************************************************************/

class InputHMD
{
public:
	iWearTracker *VRTracker;
	iWearStereo3D *VRStereo;

private:
	bool ActTracking;
	bool ActStereo;

public:
	InputHMD(bool onStereo, bool onTracking);
	bool InitHMD();

	//Propiedades
	bool isActTracking()
	{
		return ActTracking;
	}
	bool isActStereo()
	{
		return ActStereo;
	}
};

/****************************************************************/
/*                        Global                                */
/****************************************************************/

extern InputHMD* MgrHMD;

/****************************************************************/
/*                       Functions								*/
/****************************************************************/


#endif // __C_INPUTHMD_H_INCLUDED__
