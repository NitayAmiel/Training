#ifndef CONNECTION_H
#define CONNECTION_H






#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <vector>
#include <cstring>
#include <sys/types.h>
//#include <winsock.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
using namespace std;

const unsigned int MAX_DATA_SIZE = 100;

typedef enum type{
    TCP,UDP
} TYPE;


typedef enum type1{
    CLIENT,
SERVER
} TYPE_CL_SER;



int recieving(int sock);
int sending(string message, int sock);

string extract_ip_address(struct sockaddr *their_addr) {
    // Assuming IPv4
    struct sockaddr_in *ipv4_addr = (struct sockaddr_in *)their_addr;
    char ip_address[INET_ADDRSTRLEN];

    // Convert network address to presentation format
    inet_ntop(AF_INET, &(ipv4_addr->sin_addr), ip_address, INET_ADDRSTRLEN);
    return  string(ip_address);
}

string GetMessage(){
    std :: cout << "What to send? (max is " << MAX_DATA_SIZE << " )\n";
    string response;
    getline(cin, response);
    if(response.size() > MAX_DATA_SIZE){
        std :: cout << "too big message\n";
        return "";
    }
    return response;
}

class Socket{
    protected:
        string m_ip;    
        string m_port;
        TYPE m_type;
        TYPE_CL_SER m_type_cl_ser;
        struct addrinfo *m_information;
        int m_socket_fd; 
    public:
        Socket(TYPE type, string port, TYPE_CL_SER type_cl_ser, string ip = "");
        virtual int init() = 0;
        virtual int send(string message);
        virtual int recieve();
        virtual void traffic(string ip_adress);
};







#endif // CONNECTION_H
