/* server_test.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include "io/network/server.h"
#include "io/network/socket.h"

#include "algorithm/char_to_byte.h"

#include <thread>
#include <future>

#include <chrono>
#include <iostream>



namespace test
{
    namespace network
    {

        class NetworkServer : public ::testing::Test
        {
        public:

            static ::io::network::Server* server;
            static std::future<bool> trReturn;

            NetworkServer()
            {
                // initialization code here
            }

            static void SetUpTestCase()
            {
            	trReturn = std::async(std::launch::async, []() -> bool {
                   ::io::network::Socket client;

                   client.create();

                   std::this_thread::sleep_for(std::chrono::milliseconds(100));

                   if( !client.connect("127.0.0.1", 9558) )
                   {
                	   return false;
                   }

                   auto a = ::io::network::Socket::waitFor_read({ &client }, 1000000);

                   if(a.size() == 0)
                   {
                	   return false;
                   }

                   auto data = client.recv();
                   client.send( data.data(), data.size());

                   client.close();
                   return true;
                });
            }

            static void TearDownTestCase()
            {

            }

            void SetUp( )
            {
            }

            void TearDown( )
            {
            }

            ~NetworkServer()
            {
                // cleanup any pending stuff, but no exceptions allowed
            }
        };

        ::io::network::Server* NetworkServer::server = nullptr;
        std::future<bool> NetworkServer::trReturn;

        TEST_F(NetworkServer, Create)
        {
            server = new ::io::network::Server(9558);
        }


        TEST_F(NetworkServer, BindConnectionFunction)
        {
            server->setNewConFunc([](unsigned int id, ::io::network::Socket* sock)->bool
            {
                (void)(id);
                (void)(sock);

                return true;
            });
        }

        TEST_F(NetworkServer, BindRecvFuntion)
        {
            server->setRecvFunc([s=server](unsigned int id, std::vector<uint8_t> data)->void
            {
                (void)(id);
                (void)(data);
                s->send(id, data.data(), data.size());

            });
        }

        TEST_F(NetworkServer, Broadcast)
        {
        	std::this_thread::sleep_for(std::chrono::milliseconds(500));
            server->broadcast( ::algorithm::char_to_byte("Test\0").data(), 5);
        }

        TEST_F(NetworkServer, Close)
        {        
            delete server;
        }
    }
}
