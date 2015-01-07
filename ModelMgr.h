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

#ifndef __C_MODELMGR_H_INCLUDED__
#define __C_MODELMGR_H_INCLUDED__

/****************************************************************/
/*                        Defines                               */
/****************************************************************/

/****************************************************************/
/*                        Include                               */
/****************************************************************/

#include <irrlicht.h>
#include <string>
#include <vector>

#include "ObjectsMgr.h"
#include "NetworkMgr.h"
#include "CameraMgr.h"

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
/*                       Class									*/
/****************************************************************/

struct ModelinScene
{
	ISceneNode* Model;							// Node of the loaded Model
	IAnimatedMesh* Mesh;						// Mesh of the loaded Model
	ObjeScene Object;							// Data from Object in scene
	bool Octree;								// If the model use Octree or Not
	bool Traceable;								// if the Camera should follow the Obj
};

class SceneMgr
{
private:
	// Device
	IrrlichtDevice * Device;
	IVideoDriver* Driver;						//Video Driver para trabajar
	ISceneManager* Smgr;						//Manejador de Escena
	IGUIEnvironment* Guienv;					//Manejador de GUI

	/***** Wold *****/
	// Models
	std::vector<ModelinScene> Models;			// vector with all Models and characteristics
	vector3df SunPos;							// Position of Light in the Scene
	SColorf SunColor;							// Color of Light

	// Files manager
	std::string RootFolder;

	// Scene Manager
	bool Octree;
	bool UseLight;

	stringw Caption;

public:
	bool InitSceneMgr(IrrlichtDevice *irrDevice);
	bool RenderScene();
	bool Loadpredata();
	bool InitWorld();
	bool loadModel(ObjeScene Object);
	bool LoadModelinScene(std::vector<std::string> models);
	IAnimatedMesh* getb3dzfile(const std::string &filename);
	bool UpdateModel(ObjeScene* Object);
	void searchTrackObj();

	//Propiedades
	int getnumModels() {return Models.size();}
	void setRootfolder(std::string folder)				{RootFolder = folder + "/";}
	std::string getRootfolder()							{return RootFolder;}
	vector3df getModelPosition(unsigned int Idx);
	vector3df getModelDirection(unsigned int Idx);
};

/****************************************************************/
/*                        Global                                */
/****************************************************************/

extern SceneMgr* MgrScene;

/****************************************************************/
/*                       Functions								*/
/****************************************************************/


#endif // __C_MODELMGR_H_INCLUDED__