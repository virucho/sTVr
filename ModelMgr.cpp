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
bool SceneMgr::InitSceneMgr(IrrlichtDevice *irrDevice)
{
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
bool SceneMgr::loadModel(ObjeScene Object)
{
	ISceneNode* Model = NULL;
	// modify the name if it a .pk3 file
	// File name = Media/gamename/lokation/modelname
	std::string Completname = "Media/" + RootFolder + Object.getAddfolder() + Object.getModelName();

	io::path filename(Completname.c_str());

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
	scene::IAnimatedMesh* m;

	if(extension == ".b3dz")
		m = getb3dzfile( filename.c_str() );
	else
		m = Device->getSceneManager()->getMesh( filename.c_str() );
	
	if (!m)
	{
		// model could not be loaded
		/*Device->getGUIEnvironment()->addMessageBox(
		Caption.c_str(), L"The model could not be loaded. " \
		L"Maybe it is not a supported file format.");*/
		fprintf(stderr, "Error loading the file : %s\n", filename.c_str());
		return false;
	}

	ModelinScene AuxModel;
	// Copy the Data to Buffer from Objects
	AuxModel.Mesh = m;
	AuxModel.Object = Object;
	AuxModel.Octree = false;
	Models.push_back(AuxModel);

	return true;
}

/*
Function UpdateModel() Search in the Model List to Update the Coords for a wanted Model
*/
bool SceneMgr::UpdateModel(ObjeScene* Object)
{
	unsigned int x;

	for(x = 0; x < Models.size(); x++)
	{
		if(Object->getIdName().compare(Models[x].Object.getIdName()) == 0)
		{
			Models[x].Model->setPosition(Object->getPosition());
			Models[x].Model->setRotation(Object->getRotation());
			Models[x].Model->setScale(Object->getScale());
		}
	}

	return true;
}

/*
Function InitWorld() Create the World to render - Add the Model to the Scene
*/
bool SceneMgr::InitWorld()
{
	unsigned int x;

	for(x=0; x < Models.size(); x++)
	{
		
		//Validation if the Object musst be render
		if(Models[x].Object.getinScene())
		{
			// set default material properties
			if (Models[x].Octree)
				Models[x].Model = Device->getSceneManager()->addOctreeSceneNode(Models[x].Mesh->getMesh(0));
			else
			{
				scene::IAnimatedMeshSceneNode* animModel;
				animModel= Device->getSceneManager()->addAnimatedMeshSceneNode(Models[x].Mesh);
				animModel->setAnimationSpeed(30);
				Models[x].Model = animModel;

				//Seteo Posiciones
				Models[x].Model->setPosition(Models[x].Object.getPosition());
				Models[x].Model->setRotation(Models[x].Object.getRotation());
				Models[x].Model->setScale(Models[x].Object.getScale());
			}
			Models[x].Model->setMaterialFlag(video::EMF_LIGHTING, UseLight);
			Models[x].Model->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, UseLight);
			//	Models[x].Model->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
			Models[x].Model->setDebugDataVisible(scene::EDS_OFF);
		}
	}

	return true;
}

bool SceneMgr::Loadpredata()
{
	unsigned int x, y;
	
	for(y=0; y < MgrNetwork->preloadfiles.size(); y++)
	{
		for(x=0; x < MgrNetwork->preloadfiles[y].files.size(); x++)
		{
			ObjeScene Obj(MgrNetwork->preloadfiles[y].files[x], ObjeScene::MMT_PERMANENT);
			Obj.setAddFolder(MgrNetwork->preloadfiles[y].folder);

			//Load Obbj
			loadModel(Obj);
		}
	}
	
	return true;
}

IAnimatedMesh* SceneMgr::getb3dzfile(const std::string &filename)
{
    scene::IAnimatedMesh *m  = NULL;

    // compressed file
    io::IFileSystem* file_system = Device->getFileSystem();
    if (!file_system->addFileArchive(filename.c_str(),
                                        /*ignoreCase*/false,
                                        /*ignorePath*/true, io::EFAT_ZIP))
    {
        fprintf(stderr, "irr_driver : getMesh: Failed to open zip file <%s>\n",
                    filename.c_str());
        return NULL;
    }

    // Get the recently added archive
    io::IFileArchive* zip_archive =
    file_system->getFileArchive(file_system->getFileArchiveCount()-1);
    io::IReadFile* content = zip_archive->createAndOpenFile(0);
    m = Device->getSceneManager()->getMesh(content);
    content->drop();

    file_system->removeFileArchive(file_system->getFileArchiveCount()-1);

    if(!m) return NULL;

    return m;
}

