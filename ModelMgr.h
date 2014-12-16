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

class SceneMgr
{
private:
	std::vector<ISceneNode*> Models;
	std::vector<std::string> IdNames;
	int numModels;

	IrrlichtDevice * Device;

	stringw Caption;
	bool Octree;
	bool UseLight;

public:
	bool InitScene(IrrlichtDevice *irrDevice);
	bool loadModel(ObjeScene* Object);
	bool UpdateModel(ObjeScene* Object);

	//Propiedades
	int getnumModels() {return numModels;}
};

/****************************************************************/
/*                        Global                                */
/****************************************************************/

extern SceneMgr* MgrScene;

/****************************************************************/
/*                       Functions								*/
/****************************************************************/


#endif // __C_MODELMGR_H_INCLUDED__