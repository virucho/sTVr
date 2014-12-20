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
    m_state          = NS_START;
    m_host           = NULL;

    m_client_id        = 0;

    if (enet_initialize () != 0)
    {
		fprintf (stderr, "An error occurred while initializing ENet.\n");
		exit(-1);
    }
}   // NetworkManager

NetworkManager::~NetworkManager()
{
     enet_host_destroy(m_host);
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

	fprintf(stderr, "Event : %d\n", event.type);

    switch (event.type)
    {
		case ENET_EVENT_TYPE_CONNECT:
			//handleNewConnection(&event);
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			handleMessagefromServer(&event);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			handleDisconnection(&event);
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

	m_state= NetworkManager::NS_PRELOAD_DATA;
    return true;
}  // initClient

void NetworkManager::handleMessagefromServer(ENetEvent *event)
{
	STVRMessage::MessageType TypeMsg;
	
	TypeMsg = STVRMessage::getPeekType(event->packet);

	fprintf(stderr, "MessageType : %d\n", TypeMsg);

	switch(TypeMsg)
	{
	case STVRMessage::MT_LOADMODEL:
		if(m_state == NS_WORLD_LOADING)
		{
			Msgloadmodel m(event->packet);
			MgrScene->loadModel(m.getObjScene());
		}
		break;
	case STVRMessage::MT_OBJS_UPDATE:
		if(m_state == NS_UPDATING)
		{
			MsgUpdateObj m(event->packet);
			MgrScene->UpdateModel(m.getObjScene());
		}
		break;
	case STVRMessage::MT_END_LOAD:
		if(m_state == NS_WORLD_LOADING)
		{
			MgrScene->InitWorld();
			m_state = NS_UPDATING;
		}
		break;
	case STVRMessage::MT_GAME_NAME:		MsgGameName m(event->packet);
		MgrNetwork->setGamename(m.getGameName());
		MgrNetwork->setClientId(m.getIdClient());
		MgrScene->setRootfolder(m.getGameName());	//Set the root folder = Gamename

		fprintf(stderr, "Starting Game : %s\n", MgrNetwork->getGamename().c_str());
		break;
	}
}   // handleMessageAtClient

void NetworkManager::sendToServer(STVRMessage &m)
{
    enet_peer_send(m_server, 0, m.getPacket());
    enet_host_flush(m_host);
}   // sendToServer

void NetworkManager::handleDisconnection(ENetEvent *event)
{
    fprintf(stderr, "%x:%d disconnected (host id %d).\n", event->peer->address.host,
        event->peer->address.port, (int)(long)event->peer->data );
}   // handleDisconnection


