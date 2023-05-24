#pragma once

#include <cstring>
#include <map>
#include <string>

#include "object.h"

struct Obstacle {
    uint64_t id;
    float mat4[16];
};

struct MapProperties {
    float width;
    float height;
    float resolution;
    uint32_t center;
    float x;
    float y;
    float theta;
    float speed;
    size_t n;
};

class DataUnpacker {
   private:
    bool isComplete;
    bool isMapPropertiesSet;
    int size_of_map_prop = sizeof(MapProperties) + 2;
    std::string object_str;
    void UnpackMapProperties(std::string data);
    void UnpackObjects(std::string&& data, int n);

    MapProperties map_properties;
    std::map<uint64_t, Eigen::Matrix4f> map_mat4_;
    std::string data;

   public:
    MapProperties ExtractMapProperties();
    std::map<uint64_t, Eigen::Matrix4f> ExtractObjects();

    DataUnpacker() : isComplete(false), isMapPropertiesSet(false){};
    bool IsCompleted();
    void Unpack(std::string&& data);
};

template <typename T>
std::string ConvertToBytes(T data) {
    std::string out;
    out.resize(sizeof(T));
    std::memcpy(&out[0], &data, sizeof(T));
    return out;
}