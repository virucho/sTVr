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

#ifndef __C_ALLMSG_H_INCLUDED__
#define __C_ALLMSG_H_INCLUDED__

/****************************************************************/
/*                        Defines                               */
/****************************************************************/

/****************************************************************/
/*                        Include                               */
/****************************************************************/

#include "message.hpp"
#include "ObjectsMgr.h"

#include <string>
#include <vector>

/****************************************************************/
/*                        Namespace                             */
/****************************************************************/

/****************************************************************/
/*                       Class									*/
/****************************************************************/

class Msgloadmodel : public Message
{
private:
	ObjeScene Object;

public:
	Msgloadmodel(ObjeScene* Obj): Message(Message::MT_LOADMODEL)
	{
		//Mensaje ID, Nombre, IdName, 3*Float Position, 3*Float Rotation, 3*Float Scale
		int LenMsg = 0;
		LenMsg += getStringLength(Obj->getModelName());
		LenMsg += getStringLength(Obj->getIdName());
		LenMsg += 3*getFloatLength();
		LenMsg += 3*getFloatLength();
		LenMsg += 3*getFloatLength();
		
		allocate(LenMsg);
		addString(Obj->getModelName());
		addString(Obj->getIdName());

		addFloat(Obj->getPosition().X);
		addFloat(Obj->getPosition().Y);
		addFloat(Obj->getPosition().Z);

		addFloat(Obj->getRotation().X);
		addFloat(Obj->getRotation().Y);
		addFloat(Obj->getRotation().Z);

		addFloat(Obj->getScale().X);
		addFloat(Obj->getScale().Y);
		addFloat(Obj->getScale().Z);
	}
    Msgloadmodel(ENetPacket* pkt): Message(pkt, true)
	{
		Object.setModelName(getString());
		Object.setIdName(getString());
		Object.setPosition(vector3df(getFloat(), getFloat(), getFloat()));
		Object.setRotation(vector3df(getFloat(), getFloat(), getFloat()));
		Object.setScale(vector3df(getFloat(), getFloat(), getFloat()));
	}

	//Propiedades
	ObjeScene* getObjScene() { return &Object; }
};   // Msgloadmodel

class MsgUpdateObj : public Message
{
private:
	ObjeScene Object;

public:
	MsgUpdateObj(ObjeScene* Obj): Message(Message::MT_OBJS_UPDATE)
	{
		//Mensaje ID, IdName, 3*Float Position, 3*Float Rotation, 3*Float Scale
		int LenMsg = 0;
		LenMsg = getStringLength(Obj->getIdName());
		LenMsg += 3*getFloatLength();
		LenMsg += 3*getFloatLength();
		LenMsg += 3*getFloatLength();
		
		allocate(LenMsg);
		addString(Obj->getIdName());

		addFloat(Obj->getPosition().X);
		addFloat(Obj->getPosition().Y);
		addFloat(Obj->getPosition().Z);

		addFloat(Obj->getRotation().X);
		addFloat(Obj->getRotation().Y);
		addFloat(Obj->getRotation().Z);

		addFloat(Obj->getScale().X);
		addFloat(Obj->getScale().Y);
		addFloat(Obj->getScale().Z);
	}
    MsgUpdateObj(ENetPacket* pkt): Message(pkt, true)
	{
		Object.setIdName(getString());
		Object.setPosition(vector3df(getFloat(), getFloat(), getFloat()));
		Object.setRotation(vector3df(getFloat(), getFloat(), getFloat()));
		Object.setScale(vector3df(getFloat(), getFloat(), getFloat()));
	}

	//Propiedades
	ObjeScene* getObjScene() { return &Object; }
};   // MsgUpdateObj

/****************************************************************/
/*                        Global                                */
/****************************************************************/

/****************************************************************/
/*                       Functions								*/
/****************************************************************/


#endif // __C_ALLMSG_H_INCLUDED__