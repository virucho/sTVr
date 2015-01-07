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

	TotalCamPos = CamPos = vector3df(0.0f,0.5f,0);
	TotalViewVector = ViewVector = vector3df(1.0f,-1.0f,1.0f);

	cameraRight = NULL;
	cameraLeft = NULL;

	CameraAux[0] = NULL;
	CameraAux[1] = NULL;

	CurrentObjCam = 0;
}

bool MainCamera::CreateCameras(ISceneManager* irrSmgr)
{
	if(irrSmgr == NULL)
		return false;

	if(SceneMgr != NULL)
		SceneMgr = irrSmgr;

	float m_fov = DEGREE_TO_RAD*75.0f;
	
	//Add Camera in the Scene
	cameraRight =irrSmgr->addCameraSceneNode(0, CamPos, ViewVector);
	//cameraRight->setFOV(DEFAULT_FOV_Y);
	cameraRight->setFOV(m_fov);
	//cameraRight->setNearValue(DEFAULT_NEAR_Z);
	//cameraRight->setFarValue(DEFAULT_FAR_Z);
	cameraRight->setFarValue(1000.0f);

	cameraLeft = irrSmgr->addCameraSceneNode(0, CamPos, ViewVector);
	cameraLeft->setFOV(m_fov);
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

void MainCamera::followObj()
{
	vector3df Pos;
	float totalPitch;
	float Totalyaw;
	
	// Calculo Posicion
	Pos = MgrScene->getModelPosition(TrackObj[CurrentObjCam]);
	TotalCamPos = Pos + CamPos;

	// Calculo angulo de la camara
	Pos = MgrScene->getModelDirection(TrackObj[CurrentObjCam]);

	Pos += ViewVector;

	TotalViewVector = vector3df(tan(Pos.Y), tan(Pos.Z), 1.0f);

	//TotalViewVector = Viewdir + ViewVector;
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

