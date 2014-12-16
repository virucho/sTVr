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
     m_mode           = NW_NONE;
     m_state          = NS_MODEL_LOADING;
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
	Message MsgRec(event->packet, false);

	switch(MsgRec.getType())
	{
	case Message::MT_LOADMODEL:
		if(m_state == NS_MODEL_LOADING)
		{
			Msgloadmodel m(event->packet);
			MgrScene->loadModel(m.getObjScene());
			//m_state = NS_UPDATING;
		}
		break;
	}

	MsgRec.clear();


//    switch(m_state)
//    {
//    case NS_WAIT_FOR_AVAILABLE_CHARACTERS:
//        {
//            CharacterInfoMessage m(event->packet);
//            // FIXME: handle list of available characters
//            m_state = NS_CHARACTER_SELECT;
//            break;
//        }
//    case NS_CHARACTER_SELECT:
//        {
//            CharacterConfirmMessage m(event->packet);
//            kart_properties_manager->selectKartName(m.getKartName());
//            // TODO - karts selection screen in networking
//            /*
//            CharSel *menu = dynamic_cast<CharSel*>(menu_manager->getCurrentMenu());
//            if(menu)
//                menu->updateAvailableCharacters();
//             */
//            break;
//        }
//    case NS_WAIT_FOR_KART_CONFIRMATION:
//        {
//            CharacterConfirmMessage m(event->packet);
//            kart_properties_manager->selectKartName(m.getKartName());
//
//            // If the current menu is the character selection menu,
//            // update the menu so that the newly taken character is removed.
//            // TODO - kart selection screen and networking
//            /*
//            CharSel *menu = dynamic_cast<CharSel*>(menu_manager->getCurrentMenu());
//            if(menu)
//                menu->updateAvailableCharacters();*/
//            // Check if we received a message about the kart we just selected.
//            // If so, the menu needs to progress, otherwise a different kart
//            // must be selected by the current player.
//            if(m.getKartName()==m_kart_to_confirm)
//            {
//                int host_id = m.getHostId();
//                m_state = (host_id == getMyHostId()) ? NS_KART_CONFIRMED
//                                                     : NS_CHARACTER_SELECT;
//            }   // m.getkartName()==m_kart_to_confirm
//            break;
//        }   // wait for kart confirmation
//    case NS_WAIT_FOR_RACE_DATA:
//        {
//            // It is possible that character confirm messages arrive at the
//            // client when it has already left the character selection screen.
//            // In this case the messages can simply be ignored.
//            if(Message::peekType(event->packet)==Message::MT_CHARACTER_CONFIRM)
//            {
//                // Receiving it will automatically free the memory.
//                CharacterConfirmMessage m(event->packet);
//                return;
//            }
//            RaceInfoMessage m(event->packet);
//            // The constructor actually sets the information in the race manager
//            m_state = NS_LOADING_WORLD;
//            break;
//        }
//    case NS_READY_SET_GO_BARRIER:
//        {
//            // Not actually needed, but the destructor of RaceStartMessage
//            // will free the memory of the event.
//            RaceStartMessage m(event->packet);
//            m_state = NS_RACING;
//            break;
//        }
//    case NS_RACING:
//        {
//            assert(false);  // should never be here while racing
//            break;
//        }
//    case NS_RACE_RESULT_BARRIER:
//        {
//            RaceResultAckMessage message(event->packet);
//            // TODO - race results menu in networking
//            /*
//            RaceResultsGUI *menu = dynamic_cast<RaceResultsGUI*>(menu_manager->getCurrentMenu());
//            if(menu)
//                menu->setSelectedWidget(message.getSelectedMenu());*/
//            m_state = NS_RACE_RESULT_BARRIER_OVER;
//            break;
//        }
//    default:
//        {
//            printf("received unknown message: type %d\n",
//                Message::peekType(event->packet));
//         //   assert(0);   // should not happen
//        }
//    }   // switch m_state
}   // handleMessageAtClient

