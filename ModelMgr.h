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

/****************************************************************/
/*                        Namespace                             */
/****************************************************************/

using namespace irr;			//Haupt Namespace von Irrlicht

using namespace core;			//Zusätzliche Namespace
using namespace scene;
using namespace video;
using namespace io;

/****************************************************************/
/*                       Class									*/
/****************************************************************/

struct ModelinScene
{
	ISceneNode* Model;							// Node of the loaded Model
	IAnimatedMesh* Mesh;						// Mesh of the loaded Model
	ObjeScene Object;							// Data from oBject in scene
	bool Octree;								// If the model use Octree or Not
};

class SceneMgr
{
private:
	// Driver
	IrrlichtDevice * Device;

	// Models
	std::vector<ModelinScene> Models;			// vector with all Models and characteristics

	// Files manager
	std::string RootFolder;

	// Scene Manager
	bool Octree;
	bool UseLight;

	stringw Caption;

public:
	bool InitSceneMgr(IrrlichtDevice *irrDevice);
	bool InitWorld();
	bool loadModel(ObjeScene Object);
	bool UpdateModel(ObjeScene* Object);

	//Propiedades
	int getnumModels() {return Models.size();}
	void setRootfolder(std::string folder)				{RootFolder = folder + "/";}
	std::string getRootfolder()							{return RootFolder;}
};

/****************************************************************/
/*                        Global                                */
/****************************************************************/

extern SceneMgr* MgrScene;

/****************************************************************/
/*                       Functions								*/
/****************************************************************/


#endif // __C_MODELMGR_H_INCLUDED__