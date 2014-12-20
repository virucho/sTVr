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

#ifndef __C_OBJMGR_H_INCLUDED__
#define __C_OBJMGR_H_INCLUDED__

/****************************************************************/
/*                        Defines                               */
/****************************************************************/

/****************************************************************/
/*                        Include                               */
/****************************************************************/

#include <irrlicht.h>
#include <string>

/****************************************************************/
/*                        Namespace                             */
/****************************************************************/

using namespace irr;			//Haupt Namespace von Irrlicht

using namespace core;			//Zusätzliche Namespace

/****************************************************************/
/*                       Class									*/
/****************************************************************/

class ObjeScene
{
public:
	enum MyModeltype
	{
		MMT_PERMANENT=1,					// Model is always in memory until the end from the App
		MMT_TEMP							// Model is destroy when the scene finish
	};

private:
	std::string ModelName;
	std::string IdName;
	std::string AddFolder;

	MyModeltype Modeltype;					// Type of Load (in memory) from the model
	bool inScene;

	vector3df ObjPosition;
	vector3df ObjRotation;
	vector3df ObjScale;

public:
	ObjeScene(std::string name, MyModeltype type)
	{
		IdName = "NA";
		inScene = false;
		ObjScale = vector3df(1.0f, 1.0f, 1.0f);
		ModelName = name;
		Modeltype = type;
	};
	ObjeScene()
	{
		IdName = "NA";
		inScene = false;
		ObjScale = vector3df(1.0f, 1.0f, 1.0f);
		Modeltype = MMT_TEMP;
	};

	//Properties
	void setModelName(std::string Name) {ModelName = Name;}
	void setIdName(std::string Id) {IdName = Id;}
	void setAddFolder(std::string folder) {AddFolder = folder;}
	void setPosition(vector3df Pos) {ObjPosition = Pos;}
	void setRotation(vector3df Rot) {ObjRotation = Rot;}
	void setScale(vector3df Scale) {ObjScale = Scale;}
	void setModeltype(MyModeltype type) {Modeltype = type;}
	void setinScene(bool isinscene) {inScene = isinscene;}
	std::string getModelName() { return ModelName;}
	std::string getIdName() { return IdName;}
	std::string getAddfolder() {return AddFolder;}
	vector3df getPosition() {return ObjPosition;}
	vector3df getRotation() {return ObjRotation;}
	vector3df getScale() {return ObjScale;}
	MyModeltype getModeltype() {return Modeltype;}
	bool getinScene() {return inScene;}

	//Funciones
	void UpdateCoords(vector3df Position, vector3df Rotation,	vector3df Scale)
	{
		ObjPosition = Position;
		ObjRotation = Rotation;
		ObjScale = Scale;
	};
};

/****************************************************************/
/*                        Global                                */
/****************************************************************/

/****************************************************************/
/*                       Functions								*/
/****************************************************************/


#endif // __C_OBJMGR_H_INCLUDED__