/* NetworkSocket.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*    All rights reserved.
*
*   This software may be modified and distributed under the terms
*   of the LGPL license. See the LICENSE file for details.
*/


#include "algorithm/char_to_byte.h"
#include "io/network/socket.h"

#include "gtest/gtest.h"

#include <thread>
#include <chrono>


namespace test
{
    namespace network
    {

        class NetworkSocket : public ::testing::Test
        {
        public:

            static ::io::network::Socket* client;
            static ::io::network::Socket* server;
            static ::io::network::Socket* accept;

            NetworkSocket()
            {
                // initialization code here
            }

            static void SetUpTestCase()
            {
                client = new ::io::network::Socket();
                server = new ::io::network::Socket();
                accept = new ::io::network::Socket();
            }

            static void TearDownTestCase()
            {
                delete client;
                delete server;
                delete accept;
            }

            void SetUp( )
            {
                // code here will execute just before every test
            }

            void TearDown( )
            {
                // code here will be called just after the test completes
                // ok to through exceptions from here if need be
            }

            ~NetworkSocket()
            {
                // cleanup any pending stuff, but no exceptions allowed
            }
        };

        ::io::network::Socket* NetworkSocket::client = nullptr;
        ::io::network::Socket* NetworkSocket::server = nullptr;
        ::io::network::Socket* NetworkSocket::accept = nullptr;



        TEST_F(NetworkSocket, Server)
        {
            ASSERT_TRUE( server->create() );
            ASSERT_TRUE( server->bind(13337) );
            ASSERT_TRUE( server->listen() );
        }


        TEST_F(NetworkSocket, Client)
        {
            ASSERT_TRUE( client->create() );
        }

        TEST_F(NetworkSocket, Connect)
        {
            ASSERT_TRUE( client->connect("127.0.0.1", 13337) );

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            ASSERT_TRUE( server->accept(*accept) );
        }


        TEST_F(NetworkSocket, SendRecv)
        {
            auto fServer = [](){
                auto a = ::io::network::Socket::waitFor_read({ &(*NetworkSocket::accept) }, 0);

                //ASSERT_EQUAL(a.size(), 1) << "no message recieved";

                auto msg = a[0]->recv(true);

                //ASSERT_EQUAL(msg.size(), 5) << "message not the correct size";

                //ASSERT_STREQ(msg.c_str(), "Test\0") << "message content not correct";

                accept->send(::algorithm::char_to_byte("Test2\0").data(), 6);
            };

            std::thread thread(fServer);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            client->send(::algorithm::char_to_byte("Test\0").data(), 5);

            auto msg = client->recv(false);

            ASSERT_EQ(msg.size(), 6) << "message not the correct size";
            ASSERT_STREQ(reinterpret_cast<const char*>( msg.data()  ), "Test2\0") << "message content not correct";

            thread.join();

        }



        TEST_F(NetworkSocket, Close)
        {
            server->close();
            client->close();
            accept->close();
        }
    }
}
