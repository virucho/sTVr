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

#include "IrrlichtMgr.h"

/****************************************************************/
/*                        Global                                */
/****************************************************************/

irrManager* irrMgr;
bool irrExit;

/****************************************************************/
/*                       Functions								*/
/****************************************************************/

irrManager::irrManager()
{
	//Constructor
	irrExit = false;
}

bool irrManager::InitIrrlicht(IEventReceiver* appReceiver)
{
	DeviceParam.DriverType = EDT_DIRECT3D9;
	DeviceParam.Vsync = false;
	DeviceParam.Stencilbuffer = true;
	DeviceParam.Stereobuffer = false;
	DeviceParam.Fullscreen = false;
	DeviceParam.EventReceiver = appReceiver;
	DeviceParam.WindowSize = dimension2d<u32>(800, 600);

	irrDevice = createDeviceEx( DeviceParam );

	if (!irrDevice)
		return false;

	//Adquiero los punteros de trabajo de Irrlicht
	irrVDriver = irrDevice->getVideoDriver();
	irrSmgr = irrDevice->getSceneManager();
	irrGuienv = irrDevice->getGUIEnvironment();

	//Defino el Directorio de Assets
	irrDevice->getFileSystem()->addFileArchive("Media/");

	return true;
}

