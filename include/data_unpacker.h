#pragma once

#include <string>
#include "object.h"
#include <map>

struct Obstacle
{
    uint64_t id;
    float mat4[16];
};

struct MapProperties
{
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

class DataUnpacker
{
    private:
    bool isComplete;
    bool isMapPropertiesSet;
    std::string object_str;
    void unpackMapProperties(std::string data);
    void unpackObject(std::string&& data, int n);
    
    MapProperties map_properties;
    std::map<uint64_t,TransformMatrix> map_mat4_;
    
    public:
    MapProperties ExtractMapProperties();
    std::map<uint64_t,TransformMatrix> ExtractObject();

    DataUnpacker():isComplete(false), isMapPropertiesSet(false){};
    bool isCompleted();
    void unpack(std::string&& data);
};
#include <cstring>

template<typename T> 
std::string ConvertToBytes(T data)
{
    std::string out;
    out.resize(sizeof(T));
    std::memcpy(&out[0], &data, sizeof(T));
    return out;
}