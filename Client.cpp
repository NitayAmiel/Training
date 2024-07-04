#include "Client.h"



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
    if((m_information) == NULL){
        perror("connect");
        exit(1);
    }
    return 0;
}

string client :: get_ip() const{
    return this->m_ip;
}
