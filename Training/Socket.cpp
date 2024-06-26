#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <cstring>
#include <sys/types.h>
//#include <winsock.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
/*
*/
#include <iostream>
using namespace std;
const unsigned int MAX_DATA_SIZE = 100;

int recieving(int sock);
int sending(string message, int sock);

typedef enum type{
    TCP,UDP
} TYPE;


typedef enum type1{
    CLIENT, SERVER
} TYPE_CL_SER;


class Socket{
    protected:
        string m_ip;    
        string m_port;
        TYPE m_type;
        TYPE_CL_SER m_type_cl_ser;
        struct addrinfo *m_information;
        int m_socket_fd; 
    public:
        Socket(TYPE type, string port, TYPE_CL_SER type_cl_ser, string ip = nullptr);
        virtual int init() = 0;
        virtual int send(string message);
        virtual int recieve();
};


Socket :: Socket(TYPE type, string port, TYPE_CL_SER type_cl_ser, string ip) : m_ip(ip) ,m_port(port), m_type(type), m_type_cl_ser(type_cl_ser) {
	int numbytes, res;  
	struct addrinfo hints, *p;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = m_type == TCP ? SOCK_STREAM : SOCK_DGRAM;
    hints.ai_flags = m_type == TCP ? AI_PASSIVE : 0; // use my IP in server
    res = getaddrinfo(m_ip.c_str(), m_port.c_str(), &hints, &m_information);
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



/// @brief /////////////////
class client : public Socket{
    public:
        client(TYPE type, string port, TYPE_CL_SER type_cl_ser, string ip) : Socket(type, port, type_cl_ser, ip) {}
        int init() override;
        string get_ip();
};
string client :: get_ip(){
    return this->m_ip;
}

int client :: init(){    
    for ( ; m_information != NULL; m_information = m_information->ai_next) {
        if ((m_socket_fd = socket(m_information->ai_family, m_information->ai_socktype, m_information->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }
        if (connect(m_socket_fd, m_information->ai_addr, m_information->ai_addrlen) == -1) {
            close(m_socket_fd);//close the file descriptor
            perror("connect");
            continue;
        }
        break; // If we get here, we must have connected successfully
    }
}

/////////////////////////////////



string extract_ip_address(struct sockaddr *their_addr) {
    // Assuming IPv4
    struct sockaddr_in *ipv4_addr = (struct sockaddr_in *)their_addr;
    char ip_address[INET_ADDRSTRLEN];

    // Convert network address to presentation format
    inet_ntop(AF_INET, &(ipv4_addr->sin_addr), ip_address, INET_ADDRSTRLEN);
    return  string(ip_address);
}
//////////////////////////
class server : public Socket{
    public:
        server(TYPE type, string port, TYPE_CL_SER type_cl_ser, string ip) : Socket(type, port, type_cl_ser, ip) {}
        virtual int init() override;
};
int server:: init(){    
    int yes = 1;
    for ( ; m_information != NULL; m_information = m_information->ai_next) {
        if ((m_socket_fd = socket(m_information->ai_family, m_information->ai_socktype, m_information->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }
        if (m_type == TCP && setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        if (bind(m_socket_fd, m_information->ai_addr, m_information->ai_addrlen) == -1) {
            close(m_socket_fd);//close the file descriptor
            perror("connect");
            continue;
        }
        break; // If we get here, we must have connected successfully
    }
}

////////////////////////
class TCP_server : public server{
    public:
    TCP_server(TYPE type, string port, TYPE_CL_SER type_cl_ser, string ip) : server(type, port, type_cl_ser, ip) {}
    void traffic(string ip_adress);
    int make_connection();

};
void TCP_server :: traffic(string ip_adress){
    char res;
    while(1){
        std :: cout << "do you want to [R] recieve from or [S] send to, or [T] terminate the traffic with "<< ip_adress << std :: endl;
        std :: cin >> res;
        if(res == 'R'){
            this->recieve();
        }else if(res == 'S'){
            std :: cout << "What to send? (max is " << MAX_DATA_SIZE << " )\n";
            string s;
            std :: cin >> s ;
            this->send(s);
        }else if(res == 'T'){
            break;
        }else{
            std :: cout << "type again\n";
        }
    }
}

int TCP_server::make_connection(){
    struct addrinfo their_addr;
    socklen_t sin_size;
    while(1){
        int new_fd = accept(m_socket_fd, (struct sockaddr *)&their_addr, &sin_size);
        if(new_fd == -1){
            perror("error : accepeion");
            exit(1);
        }
        if(fork() == 0){
            close(m_socket_fd);
            m_socket_fd =  new_fd;
            this->traffic(extract_ip_address((struct sockaddr *)&their_addr));
            close(m_socket_fd);
            exit(1);
        }
        close(new_fd);
    }
}



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