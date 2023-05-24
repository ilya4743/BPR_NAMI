#pragma once

#include <iostream>
#include <string>

#include "data_unpacker.h"
#include "pathfinder.h"
class PathFinder;

namespace handler {

class RequestHandler {
   public:
    explicit RequestHandler() : unpacker(), pathfinder(){};

    RequestHandler(const RequestHandler&) = delete;
    RequestHandler& operator=(const RequestHandler&) = delete;

    template <typename Send>
    void operator()(tcp::endpoint&& endpoint, std::string&& req, Send&& send) {
        unpacker.Unpack(std::move(req));
        if (unpacker.IsCompleted()) {
            auto p = unpacker.ExtractMapProperties();
            auto objects = unpacker.ExtractObjects();
            pathfinder.UpdateData(p.width, p.height, p.resolution, p.center, p.x, p.y, p.theta, p.speed, p.n, std::move(objects));
            std::vector<Eigen::Vector3f> path;
            if (BPR_NAMI::Constants::GetInstance().IS_FIND_PATH())
                path = pathfinder.FindPath();
            std::stringstream stream;
            stream << (unsigned char)0x44 << (unsigned char)0x48 << ConvertToBytes<int>(path.size() * 2);
            for (auto it = path.begin(); it != path.end(); ++it) {
                stream << ConvertToBytes<float>(-(*it)(0)) << ConvertToBytes<float>((*it)(2));
            }
            send(std::move(stream.str()));
            pathfinder.Clear();
        }
    };

   private:
    DataUnpacker unpacker;
    PathFinder pathfinder;
};

}  // namespace handler