#include <iostream>
#include "Server.h"
#include "Client.h"



int main(int argc, char *argv[]){
    
    std :: vector<string> arguments;
    for(unsigned int i = 0; i < argc; i++){
        arguments.push_back(string(argv[i]));
    }
    if(arguments.size() < 3){
        perror("missing arguments\n");
        return 0;
    }

    type Type_Net = (arguments[2].compare("TCP") == 0) ? TCP : UDP; 
    if(arguments.at(1) == "CL"){
        std::cout << "starting initalizing client\n";
        client* cl_1 = new client(Type_Net, argv[3], CLIENT, argv[4]);
        cl_1->init();
        cl_1->traffic(cl_1->get_ip());
        delete cl_1;
        return 0;
    }
    else {
        if(Type_Net == TCP){
            TCP_server* ser1 = new TCP_server(TCP, argv[3], SERVER, "");
            ser1->init();
            ser1->make_connection();
            delete ser1;
            return 0;
        }else {
            UDP_server *ser2 = new UDP_server(UDP, argv[3], SERVER, "");
            ser2->init();
            ser2->traffic("someone");
            delete ser2;
        }
    }
}
