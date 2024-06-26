#include "Socket.cpp"
#include <iostream>


int main(int argc, char *argv[]){
    if(fork() == 0){
        sleep(5000);
        client* cl_1 = new client(TCP, "49152", CLIENT, "127.0.0.1");
        std::cout << "got here1" << std::endl;
        cl_1->init();
        cl_1->send("Hello World");
        cl_1->recieve();
        return 0;
    }
    TCP_server* ser1 = new TCP_server(TCP, "49152", SERVER, nullptr);
    ser1->init();
    ser1->make_connection();
    std::cout << "got here2" << std::endl;
    return 0;
    if(argv[1] == "TCP"){
        //
    }
    if(argv[1] == "UDP"){
        //

    }

}