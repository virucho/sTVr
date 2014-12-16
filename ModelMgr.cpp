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

#include "ModelMgr.h"

#include "ObjectsMgr.h"

#include <string>
//#include <vector>

/****************************************************************/
/*                        Global                                */
/****************************************************************/

SceneMgr* MgrScene;

/****************************************************************/
/*                       Functions								*/
/****************************************************************/

/*
Function InitScene() Init the Class and Load the Irrlicht Device
*/
bool SceneMgr::InitScene(IrrlichtDevice *irrDevice)
{
	numModels = 0;

	Device = irrDevice;

	Octree = false;

	if(irrDevice == NULL)
		return false;

	return true;
}

/*
Function loadModel() loads a model and displays it using an
addAnimatedMeshSceneNode and the scene manager. Nothing difficult. It also
displays a short message box, if the model could not be loaded.
*/
bool SceneMgr::loadModel(ObjeScene* Object)
{
	ISceneNode* Model = NULL;
	// modify the name if it a .pk3 file

	io::path filename(Object->getModelName().c_str());

	io::path extension;
	core::getFileNameExtension(extension, filename);
	extension.make_lower();

	// if a texture is loaded apply it to the current model..
	if (extension == ".jpg" || extension == ".pcx" ||
		extension == ".png" || extension == ".ppm" ||
		extension == ".pgm" || extension == ".pbm" ||
		extension == ".psd" || extension == ".tga" ||
		extension == ".bmp" || extension == ".wal" ||
		extension == ".rgb" || extension == ".rgba")
	{
		video::ITexture * texture =
			Device->getVideoDriver()->getTexture( filename );
		if ( texture && Model )
		{
			// always reload texture
			Device->getVideoDriver()->removeTexture(texture);
			texture = Device->getVideoDriver()->getTexture( filename );

			Model->setMaterialTexture(0, texture);
		}
		return true;
	}
	// if a archive is loaded add it to the FileArchive..
	else if (extension == ".pk3" || extension == ".zip" || extension == ".pak" || extension == ".npk")
	{
		Device->getFileSystem()->addFileArchive(filename.c_str());
		return true;
	}

	// load a model into the engine

	if (Model)
		Model->remove();

	Model = 0;

	if (extension==".irr")
	{
		core::array<scene::ISceneNode*> outNodes;
		Device->getSceneManager()->loadScene(filename);
		Device->getSceneManager()->getSceneNodesFromType(scene::ESNT_ANIMATED_MESH, outNodes);
		if (outNodes.size())
			Model = outNodes[0];
		return true;
	}

	scene::IAnimatedMesh* m = Device->getSceneManager()->getMesh( filename.c_str() );
	if (!m)
	{
		// model could not be loaded
		Device->getGUIEnvironment()->addMessageBox(
		Caption.c_str(), L"The model could not be loaded. " \
		L"Maybe it is not a supported file format.");
		return true;
	}

	// set default material properties

	if (Octree)
		Model = Device->getSceneManager()->addOctreeSceneNode(m->getMesh(0));
	else
	{
		scene::IAnimatedMeshSceneNode* animModel = Device->getSceneManager()->addAnimatedMeshSceneNode(m);
		animModel->setAnimationSpeed(30);
		Model = animModel;

		//Seteo Posiciones
		Model->setPosition(Object->getPosition());
		Model->setRotation(Object->getRotation());
		Model->setScale(Object->getScale());
	}
	Model->setMaterialFlag(video::EMF_LIGHTING, UseLight);
	Model->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, UseLight);
//	Model->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	Model->setDebugDataVisible(scene::EDS_OFF);

	Models.push_back(Model);
	IdNames.push_back(Object->getIdName());

	numModels++;

	return true;
}

bool SceneMgr::UpdateModel(ObjeScene* Object)
{
	int x;

	for(x = 0; x < IdNames.size(); x++)
	{
		if(Object->getIdName().compare(IdNames[x]) == 0)
		{
			Models[x]->setPosition(Object->getPosition());
			Models[x]->setRotation(Object->getRotation());
			Models[x]->setScale(Object->getScale());
		}
	}

	return true;
}

