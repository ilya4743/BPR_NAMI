#ifndef DATA_UNPACKER_H
#define DATA_UNPACKER_H
#include <string>
#include <OGRE/OgreMatrix4.h>
#include <map>

struct Obstacle
{
    uint64_t id;
    float mat4[16];
};

template<typename T> 
std::string convertToBytes(T data)
{
    std::string out;
    out.resize(sizeof(T));
    std::memcpy(&out[0], &data, sizeof(T));
    return out;
}

class DataUnpacker
{
    private:
    bool isComplete;
    bool isMapPropertiesSet;
    std::string obstacle_str;
    
    void unpackMapProperties(std::string data);
    void unpackObstacle(std::string data, int n);

    public:
    float width;
    float height;
    float resolution;
    uint32_t center;
    float x;
    float y;
    float theta;
    float speed;
    size_t n;
    std::map<uint64_t,Ogre::Matrix4> map_mat4_;

    DataUnpacker():isComplete(false), isMapPropertiesSet(false){};
    bool isCompleted();
    void unpack(std::string&& data);
};

#endif