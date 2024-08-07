
#include "Server.h"

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
    return 0;
}


int UDP_server :: send(string message) {
    socklen_t len = sizeof(*m_cliaddr);
    int sent = sendto(m_socket_fd, message.c_str(), message.size(), 0, (const struct sockaddr *)m_cliaddr, len);
    std :: cout << "sent  " << sent << " number of bytes\n";
    return sent;
}

int UDP_server :: recieve() {
    struct sockaddr_in* cliaddr = new struct sockaddr_in;
    socklen_t len = sizeof(*cliaddr);
    char buffer[MAX_DATA_SIZE];
    int number_of_bytes = recvfrom(m_socket_fd,buffer,MAX_DATA_SIZE,0, (struct sockaddr *)cliaddr, &len);
    buffer[number_of_bytes] = '\0';
    string ip_address_client = extract_ip_address((struct sockaddr *)cliaddr);
    std::cout << "Client " << ip_address_client << "sent " << buffer << std::endl;
    this->m_cliaddr = cliaddr;   
    this->replying();
    return number_of_bytes;
}

void UDP_server :: replying(){
    string response;
    while(1){
        response.clear();
        std :: cout << "do you want to reply something to " << extract_ip_address((struct sockaddr *)m_cliaddr) << " [Y or n]?" << endl;
        getline(cin, response);
        if(response.empty() == true || (response[0] != 'Y' && response[0] != 'n')) continue;
        if(response[0] == 'n') break;
        response = GetMessage();
        this->send(response);
    }
}
////////////////////////

int TCP_server::make_connection(){
    cout << "make_connection\n" ;
    if (listen(m_socket_fd, 10) == -1) {
        perror("listen");
        exit(1);
    }
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

