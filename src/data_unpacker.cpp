#include "data_unpacker.h"
#include "net_headers.h"

void DataUnpacker::unpackMapProperties(std::string data)
{
    const int s1=sizeof(float);
    unsigned char byte4[s1];

    auto itData=data.begin()+2;

    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    map_properties.width = *reinterpret_cast<float*>(byte4);
    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    map_properties.height = *reinterpret_cast<float*>(byte4);

    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    map_properties.resolution = *reinterpret_cast<float*>(byte4);

    std::copy(itData, itData+sizeof(uint32_t),byte4);
    itData+=sizeof(uint32_t);
    map_properties.center = *reinterpret_cast<uint32_t*>(byte4);

    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    map_properties.x = *reinterpret_cast<float*>(byte4);

    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    map_properties.y = *reinterpret_cast<float*>(byte4);

    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    map_properties.theta = *reinterpret_cast<float*>(byte4);
    
    std::copy(itData, itData+sizeof(float),byte4);
    itData+=sizeof(float);
    map_properties.speed = *reinterpret_cast<float*>(byte4);

    const int s2=sizeof(uint64_t);
    unsigned char byte8[s2];
    
    std::copy(itData, itData+sizeof(uint64_t),byte8);
    itData+=sizeof(uint64_t);
    map_properties.n = *reinterpret_cast<uint64_t*>(byte8);

    isMapPropertiesSet=true;
}

void DataUnpacker::unpackObject(std::string&& data, int n)
{
    int o_size=sizeof(Obstacle);
    int data_size=data.size();
    unsigned char id_in_bytes[8];
    unsigned char *byte_mat4;
    float mat4[16];
    byte_mat4 = reinterpret_cast<unsigned char *>(mat4);
    map_mat4_.clear();
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
    if((unsigned char)data[0]==0x44 && (unsigned char)data[1]==0x47 && isMapPropertiesSet==false)
    {
        unpackMapProperties(data);
        isMapPropertiesSet=true; 
        object_str.append(data.begin()+sizeof(MapProperties)+2,data.end());
    }    
    else
    if(isMapPropertiesSet)
        object_str.append(data.begin(),data.end());     

    if(object_str.size()-1>=map_properties.n*sizeof(Obstacle))
    {
        isComplete=true;
        unpackObject(std::move(object_str),map_properties.n);
        object_str.clear();
        isMapPropertiesSet=false;
    }
    else
        isComplete=false;
}

MapProperties DataUnpacker::ExtractMapProperties()
{
    return map_properties;
}

std::map<uint64_t,Ogre::Matrix4> DataUnpacker::ExtractObject()
{
    return map_mat4_;
}