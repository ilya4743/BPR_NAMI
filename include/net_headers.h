#include <utility>

namespace NetHeaders {
using byte = unsigned char;
constexpr static std::pair<byte, byte> CONNECT_HEADER_BPR = {0x79, 0x93};
constexpr static std::pair<byte, byte> CONNECT_HEADER_MONITOR = {0x79, 0x93};
constexpr static std::pair<byte, byte> PATHFINDING_REQUEST_HEADER = {0x79, 0x93};
constexpr static std::pair<byte, byte> PATHFINDING_RESPONSE_HEADER = {0x79, 0x93};
};  // namespace NetHeaders