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

#include "CameraMgr.h"

/****************************************************************/
/*                        Global                                */
/****************************************************************/

MainCamera* MgrCamera;

/****************************************************************/
/*                       Functions								*/
/****************************************************************/

MainCamera::MainCamera(bool Stereo)
{
	StereoCam = Stereo;

	CamPos = vector3df(0.0f,150,0);
	ViewVector = vector3df(1.0f,-1.0f,1.0f);

	cameraRight = NULL;
	cameraLeft = NULL;

	CameraAux[0] = NULL;
	CameraAux[1] = NULL;
}

bool MainCamera::CreateCameras(ISceneManager* irrSmgr)
{
	if(irrSmgr == NULL)
		return false;

	if(SceneMgr != NULL)
		SceneMgr = irrSmgr;
	
	//Add Camera in the Scene
	cameraRight =irrSmgr->addCameraSceneNode(0, CamPos, ViewVector);
	cameraRight->setFOV(DEFAULT_FOV_Y);
	cameraRight->setNearValue(DEFAULT_NEAR_Z);
	cameraRight->setFarValue(DEFAULT_FAR_Z);

	cameraLeft = irrSmgr->addCameraSceneNode(0, CamPos, ViewVector);
	cameraLeft->setFOV(DEFAULT_FOV_Y);
	cameraLeft->setNearValue(DEFAULT_NEAR_Z);
	cameraLeft->setFarValue(DEFAULT_FAR_Z);

	if(cameraRight != NULL && cameraLeft != NULL)
		return true;
	else
		return false;
}

bool MainCamera::CreateCamerasAux(ISceneManager* irrSmgr)
{
	if(irrSmgr == NULL)
		return false;

	if(SceneMgr != NULL)
		SceneMgr = irrSmgr;
	
	// add a camera scene node
	CameraAux[0] = irrSmgr->addCameraSceneNodeMaya();
	CameraAux[0]->setFarValue(20000.f);
	// Maya cameras reposition themselves relative to their target, so target the location
	// where the mesh scene node is placed.
	CameraAux[0]->setTarget(core::vector3df(0,30,0));

	CameraAux[1] = irrSmgr->addCameraSceneNodeFPS();
	CameraAux[1]->setFarValue(20000.f);
	CameraAux[1]->setPosition(core::vector3df(0,0,-70));
	CameraAux[1]->setTarget(core::vector3df(0,30,0));

	return true;
}

void MainCamera::ActiveCameraMaya()
{
	setActiveCamera(CameraAux[0]);
}

void MainCamera::ActiveCameraFPS()
{
	setActiveCamera(CameraAux[1]);
}

/*
Toggle between various cameras
*/
void MainCamera::setActiveCamera(scene::ICameraSceneNode* newActive)
{
	if (SceneMgr == NULL)
		return;

	scene::ICameraSceneNode * active = SceneMgr->getActiveCamera();
	active->setInputReceiverEnabled(false);

	newActive->setInputReceiverEnabled(true);
	SceneMgr->setActiveCamera(newActive);
}

