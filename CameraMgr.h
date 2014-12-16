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

#ifndef __C_CAMERAMGR_H_INCLUDED__
#define __C_CAMERAMGR_H_INCLUDED__

/****************************************************************/
/*                        Defines                               */
/****************************************************************/

/****************************************************************/
/*                        Include                               */
/****************************************************************/

//Haupt Bibliotheken von Irrlicht
#include <irrlicht.h>
#include "IWear_Tracking.h"		//Bibliothek zu Tracking

/****************************************************************/
/*                        Namespace                             */
/****************************************************************/

using namespace irr;			//Haupt Namespace von Irrlicht

using namespace core;			//Zusätzliche Namespace
using namespace scene;

/****************************************************************/
/*                       Class									*/
/****************************************************************/

class MainCamera
{
private:
	bool StereoCam;
	ISceneManager* SceneMgr;

	ICameraSceneNode* CameraAux[2];

public:
	//Variables de manejo de la camara
	vector3df CamPos;
	vector3df ViewVector;

	ICameraSceneNode* cameraRight;
	ICameraSceneNode* cameraLeft;

public:
	MainCamera(bool Stereo);
	bool CreateCameras(ISceneManager* irrSmgr);
	bool CreateCamerasAux(ISceneManager* irrSmgr);
	void ActiveCameraMaya();
	void ActiveCameraFPS();

private:
	void setActiveCamera(scene::ICameraSceneNode* newActive);

	//Propiedades
	bool isActStereoCam()
	{
		return StereoCam;
	}
};

/****************************************************************/
/*                        Global                                */
/****************************************************************/

extern MainCamera* MgrCamera;

/****************************************************************/
/*                       Functions								*/
/****************************************************************/


#endif // __C_CAMERAMGR_H_INCLUDED__