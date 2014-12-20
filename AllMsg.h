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

#include "MessageMgr.h"
#include "ObjectsMgr.h"

#include <string>
#include <vector>

/****************************************************************/
/*                        Namespace                             */
/****************************************************************/

/****************************************************************/
/*                       Class									*/
/****************************************************************/

class Msgloadmodel : public STVRMessage
{
private:
	ObjeScene Object;

public:
	Msgloadmodel(ObjeScene* Obj): STVRMessage(STVRMessage::MT_LOADMODEL)
	{
		//Mensaje ID, Nombre, Address Folder, IdName, Type, inScene, 3*Float Position, 3*Float Rotation, 3*Float Scale
		int LenMsg = 0;
		LenMsg += getStringLength(Obj->getModelName());
		LenMsg += getStringLength(Obj->getAddfolder());
		LenMsg += getStringLength(Obj->getIdName());
		LenMsg += getCharLength();
		LenMsg += getBoolLength();
		LenMsg += 3 * getFloatLength() * 3;
		
		allocate(LenMsg);
		addString(Obj->getModelName());
		addString(Obj->getAddfolder());
		addString(Obj->getIdName());

		addChar((char)Obj->getModeltype());
		addBool(Obj->getinScene());

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
    Msgloadmodel(ENetPacket* pkt): STVRMessage(pkt)
	{
		Object.setModelName(getString());
		Object.setAddFolder(getString());
		Object.setIdName(getString());

		Object.setModeltype((ObjeScene::MyModeltype) this->getChar());
		Object.setinScene(getBool());

		Object.setPosition(vector3df(getFloat(), getFloat(), getFloat()));
		Object.setRotation(vector3df(getFloat(), getFloat(), getFloat()));
		Object.setScale(vector3df(getFloat(), getFloat(), getFloat()));
	}

	//Propiedades
	ObjeScene getObjScene() { return Object; }
};   // Msgloadmodel

/***********************************************************************************/

class MsgUpdateObj : public STVRMessage
{
private:
	ObjeScene Object;

public:
	MsgUpdateObj(ObjeScene* Obj): STVRMessage(STVRMessage::MT_OBJS_UPDATE)
	{
		//Mensaje ID, IdName, 3*Float Position, 3*Float Rotation, 3*Float Scale
		int LenMsg = 0;
		LenMsg = getStringLength(Obj->getIdName());
		LenMsg += 3 * getFloatLength() * 3;
		
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
    MsgUpdateObj(ENetPacket* pkt): STVRMessage(pkt)
	{
		Object.setIdName(getString());
		Object.setPosition(vector3df(getFloat(), getFloat(), getFloat()));
		Object.setRotation(vector3df(getFloat(), getFloat(), getFloat()));
		Object.setScale(vector3df(getFloat(), getFloat(), getFloat()));
	}

	//Propiedades
	ObjeScene* getObjScene() { return &Object; }
};   // MsgUpdateObj

/***********************************************************************************/

class MsgEndLoad : public STVRMessage
{
private:
	int NumeroObjetos;

public:
	MsgEndLoad(int NumObj): STVRMessage(STVRMessage::MT_END_LOAD)
	{
		//Mensaje ID, Numero de Modelos
		int LenMsg = 0;
		LenMsg = getIntLength();
		
		allocate(LenMsg);
		addInt(NumObj);
	}
    MsgEndLoad(ENetPacket* pkt): STVRMessage(pkt)
	{
		NumeroObjetos = getInt();
	}

	//Propiedades
	int getNumObj() { return NumeroObjetos; }
};   // MsgEndLoad

/***********************************************************************************/

class MsgGameName : public STVRMessage
{
private:
	std::string GameName;
	int IdClient;

public:
	MsgGameName(std::string Name, int Id): STVRMessage(STVRMessage::MT_GAME_NAME)
	{
		//Mensaje ID, Game Name
		int LenMsg = 0;
		LenMsg = LenMsg = getStringLength(Name) + getIntLength();
		
		allocate(LenMsg);

		addString(Name);
		addInt(Id);
	}
    MsgGameName(ENetPacket* pkt): STVRMessage(pkt)
	{
		GameName = getString();
		IdClient = getInt();
	}

	//Propiedades
	std::string getGameName() { return GameName; }
	int getIdClient() { return IdClient; }
};   // MsgGameName

/****************************************************************/
/*                        Global                                */
/****************************************************************/

/****************************************************************/
/*                       Functions								*/
/****************************************************************/


#endif // __C_ALLMSG_H_INCLUDED__