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

#ifndef M_PI
	#define M_PI 3.14159265358979323846f  /* As in Linux's math.h */
#endif

#ifndef DEGREE_TO_RAD
	#define DEGREE_TO_RAD      (M_PI/180.0f)
#endif

#ifndef RAD_TO_DEGREE
	#define RAD_TO_DEGREE      (180.0f/M_PI)
#endif

/****************************************************************/
/*                        Include                               */
/****************************************************************/

//Haupt Bibliotheken von Irrlicht
#include <irrlicht.h>
#include <vector>
#include "ModelMgr.h"

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

	vector3df TotalCamPos;
	vector3df TotalViewVector;

	/***** For Cameras *****/
	std::vector<int> TrackObj;					// List with the obects to follow
	unsigned int CurrentObjCam;					// Current Object to follow

	ICameraSceneNode* cameraRight;
	ICameraSceneNode* cameraLeft;

public:
	MainCamera(bool Stereo);
	bool CreateCameras(ISceneManager* irrSmgr);
	bool CreateCamerasAux(ISceneManager* irrSmgr);
	void ActiveCameraMaya();
	void ActiveCameraFPS();

	void followObj();

private:
	void setActiveCamera(scene::ICameraSceneNode* newActive);

	//Propiedades
public:
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