#include "DataUnpacker.h"

void DataUnpacker::unpackMapProperties(std::string data)
{
    const int s1=sizeof(float);
    unsigned char byte4[s1];

    auto itData=data.begin()+2;

    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    width = *reinterpret_cast<float*>(byte4);

    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    height = *reinterpret_cast<float*>(byte4);

    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    resolution = *reinterpret_cast<float*>(byte4);

    std::copy(itData, itData+sizeof(uint32_t),byte4);
    itData+=sizeof(uint32_t);
    center = *reinterpret_cast<uint32_t*>(byte4);

    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    x = *reinterpret_cast<float*>(byte4);

    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    y = *reinterpret_cast<float*>(byte4);

    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    theta = *reinterpret_cast<float*>(byte4);
    
    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    speed = *reinterpret_cast<float*>(byte4);

    const int s2=sizeof(uint64_t);
    unsigned char byte8[s2];
    
    std::copy(itData, itData+sizeof(uint64_t),byte8);
    itData+=sizeof(uint64_t);
    n = *reinterpret_cast<uint64_t*>(byte8);
    int d=data.size();
    isMapPropertiesSet=true;
}

void DataUnpacker::unpackObstacle(std::string data, int n)
{
    unsigned char id_in_bytes[8];
    unsigned char *byte_mat4;
    float mat4[16];
    byte_mat4 = reinterpret_cast<unsigned char *>(mat4);

    for (size_t i = 0; i < n; i++)
    {
        size_t shift = sizeof(size_t);
        size_t size_of_mat4 = i * sizeof(Obstacle);
        std::copy(data.begin() + size_of_mat4, data.begin() + shift + size_of_mat4, id_in_bytes);
        std::copy(data.begin() + shift + size_of_mat4, data.begin() + size_of_mat4 + sizeof(Obstacle), byte_mat4);
        const size_t *id = reinterpret_cast<const size_t *>(id_in_bytes);

        map_mat4_.emplace(std::make_pair(*id, Ogre::Matrix4(
                                                    mat4[0], mat4[1], mat4[2], mat4[3],
                                                    mat4[4], mat4[5], mat4[6], mat4[7],
                                                    mat4[8], mat4[9], mat4[10], mat4[11],
                                                    mat4[12], mat4[13], mat4[14], mat4[15])));
    }
}

bool DataUnpacker::isCompleted()
{
    return isComplete;
}

void DataUnpacker::unpack(std::string&& data)
{
    if((unsigned char)data[0]==0x44 && (unsigned char)data[1]==0x47)
    {
        unpackMapProperties(data);
        isMapPropertiesSet=true; 
    }    

    if(isMapPropertiesSet)
        obstacle_str.append(data.begin()+42,data.end());
    else
        obstacle_str.append(data.begin(),data.end());

    if(obstacle_str.size()-1>=n*sizeof(Obstacle))
    {
        isComplete=true;
        unpackObstacle(obstacle_str,n);
        obstacle_str.clear();
        isMapPropertiesSet=false;
    }
    else
        isComplete=false;
}