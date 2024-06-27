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
    CLIENT, SERVER
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
    string respone;
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


Socket :: Socket(TYPE type, string port, TYPE_CL_SER type_cl_ser, string ip) : m_ip(ip) ,m_port(port), m_type(type), m_type_cl_ser(type_cl_ser) {
	int numbytes, res;  
	struct addrinfo hints, *p;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = m_type == TCP ? SOCK_STREAM : SOCK_DGRAM;//datagram or stream
    hints.ai_flags = m_type_cl_ser == SERVER ? AI_PASSIVE : 0; // use my IP in server
    res = m_type_cl_ser == SERVER ? getaddrinfo(NULL, m_port.c_str(), &hints, &m_information) :getaddrinfo(m_ip.c_str(), m_port.c_str(), &hints, &m_information);
	if (res != 0) {
		perror("get address problem");
		exit(1);
	}
}

int Socket::recieve(){
    return recieving(m_socket_fd);
}

int Socket :: send(string message){
    return sending(message, m_socket_fd);
}

void Socket :: traffic(string ip_adress){
    string res;
    while(1){
        res.clean();
        std :: cout << "do you want to [R] recieve from or [S] send to, or [T] terminate the traffic with "<< ip_adress << std :: endl;
        getline(cin, res);
        if(res[0] == 'S'){
            res = GetMessage();
            if(res.empty() == false) {
                this->send(res);
            }
        }
        else if(res[0] == 'R'){
            this->recieve();
        }else if(res[0] == 'T'){
            break;
        }else{
            std :: cout << "type again\n";
        }
    }
}


/// @brief /////////////////



int recieving(int sock){
    char buffer[MAX_DATA_SIZE];
    int recieved = recv(sock, buffer, MAX_DATA_SIZE, 0);
    if(recieved == -1){
        perror("error : recieve");
        exit(1);
    }
    std :: cout << "recieved " << buffer << "\n size of " << recieved << " (max is " << MAX_DATA_SIZE << " ) \n" ;
    return recieved;
}

int sending(string message, int sock){
    if(message.size()  > MAX_DATA_SIZE){
        perror("The message is too large");
    }
    int sent = send(sock, message.c_str(),message.size(), 0); 
    if(sent == -1){
        perror("error : sending");
        exit(1);
    }
    std :: cout << "sent succesfully " << sent << "number of bytes\n";
    return sent;
}




