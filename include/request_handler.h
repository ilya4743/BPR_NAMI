#pragma once

#include <string>
#include <iostream>
#include "data_unpacker.h"
#include "pathfinder.h"

class PathFinder;

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
        auto p=unpacker.ExtractMapProperties();
        auto o=unpacker.ExtractObject();
        pathfinder.UpdateData(p.width, p.height, p.resolution, p.center,p.x, p.y, p.theta,p.speed,p.n,std::move(o));
        auto path=pathfinder.Find();
        std::stringstream stream;
        stream<<(unsigned char)0x44<<(unsigned char)0x48<<ConvertToBytes(path.size()*2);
        for(auto it=path.begin(); it!=path.end(); ++it)
        {
          stream<<ConvertToBytes(-(*it).x)<<ConvertToBytes((*it).z);
        }
        std::cout<<"send\n";
        send(stream.str());
        pathfinder.Clear();
      }
    };
  private:
    DataUnpacker unpacker;
    PathFinder pathfinder;
};