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

#include "NetworkMgr.h"
#include "ModelMgr.h"

/****************************************************************/
/*                        Global                                */
/****************************************************************/

NetworkManager* MgrNetwork = 0;

/****************************************************************/
/*                       Functions								*/
/****************************************************************/

NetworkManager::NetworkManager()
{
	m_mode           = NW_CLIENT;
    m_state          = NS_MAIN_MENU;
    m_host           = NULL;

    m_num_clients    = 0;
    m_host_id        = 0;

    if (enet_initialize () != 0)
    {
    fprintf (stderr, "An error occurred while initializing ENet.\n");
    exit(-1);
    }
}   // NetworkManager

NetworkManager::~NetworkManager()
{
     if(m_mode==NW_SERVER || m_mode==NW_CLIENT) enet_host_destroy(m_host);
     enet_deinitialize();
}   // ~NetworkManager

void NetworkManager::update(float dt)
{
    ENetEvent event;
    int result = enet_host_service (m_host, &event, 0);

    if(result==0)	//No hay evento
		return;

    if(result<0)	//Error en el Mensaje
    {
        fprintf(stderr, "Error while receiving messages -> ignored.\n");
        return;
    }

    switch (event.type)
    {
		case ENET_EVENT_TYPE_CONNECT:
			//handleNewConnection(&event);
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			handleMessageAtClient(&event);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			//handleDisconnection(&event);
			break;
		case ENET_EVENT_TYPE_NONE:
			break;
    }
}   // update

// -----------------------------------------------------------------------------
/** Initialises the client. This function tries to connect to the server.
 */
bool NetworkManager::initClient(const char* hostName, int portHost)
{
    m_host = enet_host_create (NULL /* create a client host */,
                               1    /* only allow 1 outgoing connection */,
                               0    /* channel limit */,
                               0    /* downstream bandwidth unlimited   */,
                               0    /*  upstream bandwidth unlimited    */ );

    if (m_host == NULL)
    {
        fprintf (stderr,
            "An error occurred while trying to create an ENet client host.\n");
        return false;
    }

    ENetAddress address;
    ENetEvent event;
    ENetPeer *peer;

    enet_address_set_host (& address, hostName);
    address.port = portHost;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect (m_host, &address, 2, 0);

    if (peer == NULL)
    {
        fprintf(stderr,
                "No available peers for initiating an ENet connection.\n");
        return false;
    }

    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service (m_host, & event, 5000) <= 0 ||
        event.type != ENET_EVENT_TYPE_CONNECT)
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset (peer);

        fprintf(stderr, "Connection to '%s:%d' failed.\n",
                "loclhost", 2305);
        return false;
    }
    m_server = peer;
    return true;
}  // initClient

void NetworkManager::handleMessageAtClient(ENetEvent *event)
{
	Message::MessageType TypeMsg;
	
	TypeMsg = Message::getPeekType(event->packet);

	switch(TypeMsg)
	{
	case Message::MT_LOADMODEL:
		if(m_state == NS_WORLD_LOADING)
		{
			Msgloadmodel m(event->packet);
			MgrScene->loadModel(m.getObjScene());
		}
		else if(m_state == NS_MODEL_LOADING)
		{
			Msgloadmodel m(event->packet);
			MgrScene->loadModel(m.getObjScene());
		}
		break;
	case Message::MT_OBJS_UPDATE:
		if(m_state == NS_UPDATING)
		{
			MsgUpdateObj m(event->packet);
			MgrScene->UpdateModel(m.getObjScene());
		}
		break;
	case Message::MT_END_LOAD:
		if(m_state == NS_WORLD_LOADING)
			m_state = NS_MODEL_LOADING;
		else if(m_state == NS_MODEL_LOADING)
			m_state = NS_UPDATING;

		break;
	}
}   // handleMessageAtClient

