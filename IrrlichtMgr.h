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

#ifndef __C_IRRLICHTMGR_H_INCLUDED__
#define __C_IRRLICHTMGR_H_INCLUDED__

/****************************************************************/
/*                        Defines                               */
/****************************************************************/

/****************************************************************/
/*                        Include                               */
/****************************************************************/

//Haupt Bibliotheken von Irrlicht
#include <irrlicht.h>

/****************************************************************/
/*                        Namespace                             */
/****************************************************************/

using namespace irr;			//Haupt Namespace von Irrlicht

using namespace core;			//Zusätzliche Namespace
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/****************************************************************/
/*                         Class                                */
/****************************************************************/

class irrManager
{
private:
	SIrrlichtCreationParameters DeviceParam;		//Parametros de creacion de Dispositivo
public:
	IrrlichtDevice *irrDevice;						//Dispositivo de Irrlicht
	IVideoDriver* irrVDriver;						//Video Driver para trabajar
	ISceneManager* irrSmgr;							//Manejador de Escena
	IGUIEnvironment* irrGuienv;						//Manejador de GUI

public:
	irrManager();
	bool InitIrrlicht(IEventReceiver* appReceiver);
};

/****************************************************************/
/*                        Global                                */
/****************************************************************/

extern irrManager* irrMgr;
extern bool irrExit;


/****************************************************************/
/*                       Functions								*/
/****************************************************************/


#endif // __C_IRRLICHTMGR_H_INCLUDED__
