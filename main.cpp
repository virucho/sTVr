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
/*                        Defines                               */
/****************************************************************/
//#define _WINSOCKAPI_		//Alguien esta cargando Winsock, por eso esto elimina las def

#define INT_FULLSCRENN false
#define USE_CONSOLE
#define STVR_VERSION 0.40

#define _DEFAULT_STEP_ 0.5f

/****************************************************************/
/*                        Include                               */
/****************************************************************/

//Haupt Bibliotheken von Irrlicht
#include <irrlicht.h>

//Zusätzliche Bibliotheken
#include <tchar.h>

//Eigene Bibliotheken
#include "NetworkMgr.h"
#include "irrlichtMgr.h"
#include "CameraMgr.h"
#include "ModelMgr.h"

#include "inputHMD.h"


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
/*                        Extra                                 */
/****************************************************************/

//Adicion de parametros en la compilacion apra la libreria (Solo en Windows)
#ifdef _IRR_WINDOWS_
	#pragma comment(lib, "Irrlicht.lib")
	#ifndef USE_CONSOLE
		#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
	#endif
#endif

/****************************************************************/
/*                       Mis Clases                             */
/****************************************************************/

class CAppReciever : public IEventReceiver
{
	private:
		bool KeyDown[KEY_KEY_CODES_COUNT];
	public:
		CAppReciever()
		{
			//Reset All Keys
			for (int i = 0; i < KEY_KEY_CODES_COUNT; i++)
			{
				KeyDown[i] = false;
			}
		}

		virtual bool OnEvent (const SEvent & evento)
		{
			switch (evento.EventType)
			{
				case EET_KEY_INPUT_EVENT:
				{
					KeyDown[evento.KeyInput.Key] = evento.KeyInput.PressedDown;
					break;
				}
				default:
					break;
			}
			return false;
		}

		virtual bool isKeyDown(EKEY_CODE keyCode) const
		{
			return KeyDown[keyCode];
		}

		virtual bool isKeyUp(EKEY_CODE keyCode) const
		{
			return !KeyDown[keyCode];
		}
};

/****************************************************************/
/*                       Extra Funktions                        */
/****************************************************************/

bool MoveCameraArrow (vector3df &CPos, vector3df ViewCamera, CAppReciever* Keys)
{
	vector3df UpVector = vector3df(0.0f,1.0f,0.0f);

	vector3df RightVector = UpVector.crossProduct(ViewCamera);
	
	if(Keys->isKeyDown(KEY_UP))
	{
		CPos += ViewCamera * _DEFAULT_STEP_;
		return true;
	}

	if(Keys->isKeyDown(KEY_DOWN))
	{
		CPos -= ViewCamera * _DEFAULT_STEP_;
		return true;
	}

	if(Keys->isKeyDown(KEY_RIGHT))
	{
		CPos += RightVector * _DEFAULT_STEP_;
		return true;
	}

	if(Keys->isKeyDown(KEY_LEFT))
	{
		CPos -= RightVector * _DEFAULT_STEP_;
		return true;
	}

	return false;

}

void InitReset()
{
	//Inicializo Todas las Variables globales

	irrMgr = new irrManager();
	MgrCamera = new MainCamera(true);
	MgrScene = new SceneMgr();
	MgrNetwork = new NetworkManager();

	MgrHMD = new InputHMD(false, true);
}

/****************************************************************/
/*                        Main                                  */
/****************************************************************/

int main(int argc, char *argv[] )
{
	//Instancia Evento Presion de Tecla
	CAppReciever* appReceiver = new CAppReciever();

	int lastFPS = -1;
	int fps = -1;

	fprintf(stderr, "Starting sTVr Version : %f\n\n", STVR_VERSION);
	core::stringw strName = L"Streaming Virtual reality sTVr";

	InitReset();
	
	if(!irrMgr->InitIrrlicht(appReceiver))
		return false;
	
	if(!MgrHMD->InitHMD())
		return false;

	MgrNetwork->setState(NetworkManager::NS_CONNECTING);
	if(!MgrNetwork->initClient("localhost", 2305))
	{
		fprintf(stderr, "Error with the connection to : %d", 2305);
		irrMgr->irrDevice->drop();
		return false;
	}

	//Creando la Scena
	MgrCamera->CreateCameras(irrMgr->irrSmgr);
	//MgrCamera->CreateCamerasAux(irrMgr->irrSmgr);
	//MgrCamera->ActiveCameraMaya();

	//Inicializo la Scena
	MgrScene->InitSceneMgr(irrMgr->irrDevice);

	irrMgr->irrSmgr->addLightSceneNode(0, core::vector3df(200,200,200),
		video::SColorf(1.0f,1.0f,1.0f),2000);
	irrMgr->irrSmgr->setAmbientLight(video::SColorf(0.3f,0.3f,0.3f));

	MgrNetwork->setState(NetworkManager::NS_WORLD_LOADING);

	/*ObjeScene Obj;
	Obj.setModelName("farm_track.b3d");
	Obj.setAddFolder("tracks/farm/");
	Obj.setinScene(true);
	Obj.setScale(vector3df(1.0f, 1.0f, 1.0f));
	MgrScene->setRootfolder("SuperTuxKart");*/
	
	//MgrScene->loadModel(Obj);
	//MgrScene->InitWorld();

	while(irrMgr->irrDevice->run() && !irrExit)
	//while(!irrExit)
	{
		//Valido si la Ventana esta en el Foco
		//if(irrMgr->irrDevice->isWindowActive())
		if(true)
		{
			//Valido la tecla presionada
			if(appReceiver->isKeyDown(KEY_ESCAPE))
				irrExit = true;

			//Actualizo conexiones
			MgrNetwork->update(0);

			//Change Position from Camera
			MoveCameraArrow(MgrCamera->CamPos, MgrCamera->ViewVector, appReceiver);

			if(appReceiver->isKeyDown(KEY_KEY_Q))
				MgrHMD->VRTracker->setStereoSeparation(MgrHMD->VRTracker->getStereoSeparation() - SEPARATION_STEP);
			if(appReceiver->isKeyDown(KEY_KEY_W))
				MgrHMD->VRTracker->setStereoSeparation(MgrHMD->VRTracker->getStereoSeparation() + SEPARATION_STEP);

#ifdef _IWEAR_ACTIVED_

			if(MgrHMD->isActTracking())
			{
				//Update Tracking Sensor
				MgrHMD->VRTracker->UpdateSensor();

				MgrCamera->ViewVector = MgrHMD->VRTracker->CalViewVector(MgrHMD->VRTracker->getRadPitch(), MgrHMD->VRTracker->getRadYaw());

				//Update Position stereo Camera
				MgrHMD->VRTracker->CalcCameraVectors(MgrCamera->cameraLeft, MgrCamera->CamPos, MgrCamera->ViewVector, LEFT_EYE);
				MgrHMD->VRTracker->CalcCameraVectors(MgrCamera->cameraRight, MgrCamera->CamPos, MgrCamera->ViewVector, RIGHT_EYE);
			}

#endif

			/****************** RENDER ***************/

			//Inicio la Escena a Renderizar
			irrMgr->irrVDriver->beginScene(true, true, SColor(255,255,255,255));

			// draw left camera
			//irrMgr->irrSmgr->setActiveCamera(MgrCamera->cameraLeft);
			irrMgr->irrGuienv->drawAll();
			irrMgr->irrSmgr->drawAll();

			irrMgr->irrVDriver->endScene();
#ifdef _IWEAR_ACTIVED_

	#ifdef _IWEAR_STEREO_
			VRStereo->SynchronizeEye(IWRSTEREO_LEFT_EYE);

			/************** ojo *******/

			driver->beginScene(true, true, SColor(255,255,255,255));

			// draw right camera
			smgr->setActiveCamera(cameraRight);
			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();

			VRStereo->SynchronizeEye(IWRSTEREO_RIGHT_EYE);
	#endif
#endif
			fps = irrMgr->irrVDriver->getFPS();

			if (lastFPS != fps)
			{
				strName = L"MedienProjekt - IrrlichtVR [FPS: ";
				strName += fps;
				strName += "] -> ";
				strName += MgrNetwork->getState();

				irrMgr->irrDevice->setWindowCaption(strName.c_str());
				lastFPS = fps;
			}
		}
		else
			irrMgr->irrDevice->yield();
	}

	irrMgr->irrDevice->drop(); //Eliminar el dispositivo de video

#ifdef _IWEAR_ACTIVED_
	if(MgrHMD->VRTracker != NULL)
		MgrHMD->VRTracker->iWearDispose();
#endif

	return true;
}