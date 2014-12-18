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

#ifndef __C_NETWORKMGR_H_INCLUDED__
#define __C_NETWORKMGR_H_INCLUDED__

/****************************************************************/
/*                        Defines                               */
/****************************************************************/

/****************************************************************/
/*                        Include                               */
/****************************************************************/

#include "enet/enet.h"
#include "MessageMgr.h"
#include "AllMsg.h"

#include <string>
#include <vector>

/****************************************************************/
/*                        Namespace                             */
/****************************************************************/

/****************************************************************/
/*                       Class									*/
/****************************************************************/

class NetworkManager
{
public:
    // The mode the network manager is operating in
    enum NetworkMode {NW_SERVER, NW_CLIENT, NW_NONE};

    // States for the finite state machine. First for server:
    enum NetworkState {NS_MAIN_MENU,						// Estado Inicial
                       NS_CONNECTING,						// Conectadon con el servidor
					   NS_DATA_LOADING,						// Esperando lista de Modelos a Usar
                       NS_WORLD_LOADING,					// Esperando Modelo del Mundo
					   NS_MODEL_LOADING,					// Esperando Modelos variables (Karts)
					   NS_UPDATING,							// Esperando Actializaciones de la Pista
					   NS_ENDING_GAME						// Esperendo Fin del Juego
    };
private:

    bool						m_active;
	std::string					m_Gamename;					// Nombre del juego para Instanciar
    NetworkState                m_state;
	ENetHost*                   m_host;						// Client
    int                         m_client_id;
    ENetPeer*                   m_server;					// Connection to Server

public:
	NetworkManager();
	~NetworkManager();

	//Properties
	void         setState(NetworkState s)			{m_state = s;			}
	NetworkState getState() const					{return m_state;		}
	void Active_sTVr()								{m_active = true;		}
	void setGamename(std::string name)				{m_Gamename = name;		}
	int          getMyClientId() const				{return m_client_id;	}
    void         setClientId(int host_id)			{m_client_id = host_id;	}
           
	void update(float dt);
	bool initClient(const char* hostName, int portHost);
	void sendToServer(STVRMessage &m);
	void handleMessagefromServer(ENetEvent *event);
	void handleDisconnection(ENetEvent *inevent);
    void disableNetworking();
};


/****************************************************************/
/*                        Global                                */
/****************************************************************/

extern NetworkManager* MgrNetwork;

/****************************************************************/
/*                       Functions								*/
/****************************************************************/


#endif // __C_NETWORKMGR_H_INCLUDED__