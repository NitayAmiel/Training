#ifndef CLIENT_HEADER
#define CLIENT_HEADER 
#include "Connection.h"

class client : public Socket{
    public:
        client(TYPE type, string port, TYPE_CL_SER type_cl_ser, string ip) : Socket(type, port, type_cl_ser, ip) {}
        int init() override;
        string get_ip() const;
};

#endif // CLIENT
