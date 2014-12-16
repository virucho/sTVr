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
#include "message.hpp"
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
    enum NetworkState {NS_MAIN_MENU,                       // before char sel gui
                       NS_ACCEPT_CONNECTIONS,				// server: accept connections
                       NS_MODEL_TO_USE,						// Enviando Modelos usados
					   NS_MODEL_LOADING,					// Cargando los modelos de la Pista
					   NS_UPDATING							// Enviando Actializaciones de la Pista
    };
private:

    NetworkMode                 m_mode;
    NetworkState                m_state;
    unsigned int                m_num_clients;
    int                         m_host_id;

    ENetHost                   *m_host;    // me
    ENetPeer                   *m_server;  // (clients only)
    std::vector<ENetPeer*>      m_clients; // (server only) pos in vector is client host_id

    

    void         handleNewConnection(ENetEvent *event);
    void         handleMessageAtServer(ENetEvent *event);
    void         handleMessageAtClient(ENetEvent *event);
    void         handleDisconnection(ENetEvent *event);

    // the first cast to long avoid compiler errors on 64 bit systems
    // about lost precision, then cast long to int to get the right type
    /*unsigned int getHostId(ENetPeer *p) const {return (int)(long)p->data; }

    void         sendToServer(Message &m);
    void         broadcastToClients(Message &m);*/

public:
                 NetworkManager();
                ~NetworkManager();
	bool         initClient(const char* hostName, int portHost);

    void         setMode(NetworkMode m)            {m_mode = m;              }
    NetworkMode  getMode() const                   {return m_mode;           }
    void         becomeServer();
    void         becomeClient();
    void         setState(NetworkState s)          {m_state = s;             }
    NetworkState getState() const                  {return m_state;          }
    int          getMyHostId() const               {return m_host_id;        }
    void         setHostId(int host_id)            {m_host_id = host_id;     }
    unsigned int getNumClients() const             {return m_num_clients;    }

    bool         initialiseConnections();
    void         update(float dt);

    void         disableNetworking();
    void         sendConnectMessage();  // client send initial info to server
    void         initCharacterDataStructures();
    void         sendCharacterSelected(int player_id, const std::string &kartid);
    void         waitForRaceInformation();
    void         worldLoaded();
    void         setupPlayerKartInfo();
    void         beginReadySetGoBarrier();
    void         sendRaceInformationToClients();
    void         sendUpdates();
    void         receiveUpdates();
    void         waitForClientData();
    void         sendRaceResults();
    void         beginRaceResultBarrier();
    void         sendRaceResultAck(char menu_selection=-1);
};


/****************************************************************/
/*                        Global                                */
/****************************************************************/

extern NetworkManager* MgrNetwork;

/****************************************************************/
/*                       Functions								*/
/****************************************************************/


#endif // __C_NETWORKMGR_H_INCLUDED__