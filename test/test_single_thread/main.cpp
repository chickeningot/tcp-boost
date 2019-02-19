#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include "medianet.h"

using namespace std;
using namespace medianet;

#define PORT 0

class client_peer : public remote_peer
{
    public:
        client_peer(session *sess)
            : remote_peer(sess)
        {

        }

        virtual void on_message(packet *msg) override
        {
            auto a = msg->pop_int32();
            std::cout << a << std::endl;
        }
};

void on_connected(session *sess);
void on_client_connected(session *sess);

void run_server()
{
    auto *net = new network_service();
    net->start_listen(PORT, 10, on_client_connected);
}

void run_client(short port)
{
    auto *net = new network_service();
    net->connect("127.0.0.1", port, on_connected);
}

void on_connected(session *sess)
{
    int a = 0;
    while (true)
    {
        auto packet = packet::create(0);
        packet->push_int32(a++);
        sess->send(packet);
        std::cout << a << std::endl;
    }
}

void on_client_connected(session *sess)
{
    auto peer = new client_peer(sess);
}

int main(int argc, char **argv)
{
    string option(argv[1]);

    if (option == "-s")
    {
        run_server();
    }
    else if (option == "-c")
    {
        unsigned short port = PORT;
        if (port == 0 && argc == 3)
        {
            port = boost::lexical_cast<unsigned short>(std::string(argv[2]));
        }
        run_client(port);
    }

    cin.get();
}