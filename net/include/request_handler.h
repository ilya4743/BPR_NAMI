#include <string>
#include <iostream>
#include "data_unpacker.h"

class RequestHandler
{
  public:
    explicit RequestHandler() {};

    RequestHandler(const RequestHandler&) = delete;
    RequestHandler& operator=(const RequestHandler&) = delete;

    template <typename Send>
    void operator()(std::string&& req, Send&& send) 
    {
      unpacker.unpack(std::move(req));
      if (unpacker.isCompleted())
      {

        std::stringstream ss;
        ss<<(unsigned char)0x44<<(unsigned char)0x48<<(unsigned char)0x00<<(unsigned char)0x00<<(unsigned char)0x00<<(unsigned char)0x00;
        std::cout<<"send\n";
        send(ss.str());
      }
    };
  private:
    DataUnpacker unpacker;
    
};