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
	Driver = irrDevice->getVideoDriver();
	Smgr = irrDevice->getSceneManager();
	Guienv = irrDevice->getGUIEnvironment();

	Octree = false;

	if(irrDevice == NULL)
		return false;

	SunPos = vector3df(200,200,200);
	SunColor = SColorf(1.0f,1.0f,1.0f);

	return true;
}

/*
Function RenderScene() Render the scene
*/
bool SceneMgr::RenderScene()
{
	if(MgrCamera->isActStereoCam())
	{
		//Inicio la Escena a Renderizar
		Driver->beginScene(true, true, SColor(255,255,255,255));
		// draw camera
		Smgr->drawAll();
		Guienv->drawAll();

		Driver->endScene();
	}
	else
	{
		// Left Eye
		Driver->beginScene(true, true, SColor(255,255,255,255));
		
		// draw left camera
		Smgr->setActiveCamera(MgrCamera->cameraLeft);
		
		Smgr->drawAll();		// Render Obj
		Guienv->drawAll();		// Render GUI

		Driver->endScene();

		//Sync
		MgrHMD->VRStereo->SynchronizeEye(IWRSTEREO_LEFT_EYE);

		// Right Eye
		Driver->beginScene(true, true, SColor(255,255,255,255));

		// draw right camera
		Smgr->setActiveCamera(MgrCamera->cameraRight);
		
		Smgr->drawAll();		// Render Obj
		Guienv->drawAll();		// Render GUI

		Driver->endScene();

		MgrHMD->VRStereo->SynchronizeEye(IWRSTEREO_RIGHT_EYE);

	}

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

	// Mejorar
	if(extension == ".b3dz")
		AuxModel.Traceable = true;
	else
		AuxModel.Traceable = false;

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
			if(Object->getIdName() == "beagle")
				Models[x];

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

	/**************** Models ****************/

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

	/*************** Light **************/
	//Smgr->addLightSceneNode(0, SunPos, SunColor,2000);
	//Smgr->setAmbientLight(video::SColorf(0.3f,0.3f,0.3f));

	/*************** for Cameras ************/
	searchTrackObj();				// Busco los obj a seguir

	return true;
}

bool SceneMgr::Loadpredata()
{
	unsigned int x, y;
	
	for(y=0; y < MgrNetwork->preloadfiles.size(); y++)
	{
		for(x=0; x < MgrNetwork->preloadfiles[y].files.size(); x++)
		{
			//Validation for Empty file
			if(MgrNetwork->preloadfiles[y].files[x] != "")
			{
				//remove the extension
				std::string Idname = MgrNetwork->preloadfiles[y].files[x];
				std::size_t found = Idname.rfind('.');
				Idname.erase(found, Idname.length());

				ObjeScene Obj(MgrNetwork->preloadfiles[y].files[x], ObjeScene::MMT_PERMANENT);
				Obj.setIdName(Idname);
				Obj.setAddFolder(MgrNetwork->preloadfiles[y].folder);

				//Load Obbj
				loadModel(Obj);
			}
		}
	}
	
	return true;
}

bool SceneMgr::LoadModelinScene(std::vector<std::string> inmodels)
{
	unsigned int x, y;
	unsigned int Numfiles = inmodels.size();
	unsigned int NumModels = Models.size();

	for(x=0; x < Numfiles; x++)
	{
		for(y=0; y < NumModels; y++)
		{
			if(inmodels[x] == Models[y].Object.getIdName())
			{
				Models[y].Object.setinScene(true);
				break;
			}
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

/*
Function searchTrackObj() search the traceable objects from the cam
*/
void SceneMgr::searchTrackObj()
{
	unsigned int x;

	for(x = 0; x < Models.size(); x++)
	{
		if((Models[x].Traceable) && Models[x].Object.getinScene())
			MgrCamera->TrackObj.push_back(x);
	}
}

/*
Function getModelPosition() return the position von the selected Model
*/
vector3df SceneMgr::getModelPosition(unsigned int Idx)
{
	return Models[Idx].Model->getPosition();
}

/*
Function getModelDirection() return the vector Direction von the selected Model
*/
vector3df SceneMgr::getModelDirection(unsigned int Idx)
{
	return Models[Idx].Model->getRotation();
}
