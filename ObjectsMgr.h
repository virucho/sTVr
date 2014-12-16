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
private:
	std::string ModelName;
	std::string IdName;
	vector3df ObjPosition;
	vector3df ObjRotation;
	vector3df ObjScale;

public:
	ObjeScene() {IdName = "NONE"; ObjScale = vector3df(1.0f, 1.0f, 1.0f);};

	void setModelName(std::string Name) {ModelName = Name;}
	void setIdName(std::string Id) {IdName = Id;}
	void setPosition(vector3df Pos) {ObjPosition = Pos;}
	void setRotation(vector3df Rot) {ObjRotation = Rot;}
	void setScale(vector3df Scale) {ObjScale = Scale;}
	std::string getModelName() { return ModelName;}
	std::string getIdName() { return IdName;}
	vector3df getPosition() {return ObjPosition;}
	vector3df getRotation() {return ObjRotation;}
	vector3df getScale() {return ObjScale;}

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