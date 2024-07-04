
#ifndef SERVER_HEADER
#define SERVER_HEADER
#include "Connection.h"

class server : public Socket{
    public:
        server(TYPE type, string port, TYPE_CL_SER type_cl_ser, string ip) : Socket(type, port, type_cl_ser, ip) {}
        virtual int init() override;
};

class UDP_server : public server{
    struct sockaddr_in* m_cliaddr;
    public:
        UDP_server(TYPE type, string port, TYPE_CL_SER type_cl_ser, string ip) : server(type, port, type_cl_ser, ip) , m_cliaddr(nullptr){}
        int recieve() override;
        virtual int send(string message) override;
        void replying();
};

class TCP_server : public server{
    public:
    TCP_server(TYPE type, string port, TYPE_CL_SER type_cl_ser, string ip) : server(type, port, type_cl_ser, ip) {}
    int make_connection();

};


#endif // SERVER
