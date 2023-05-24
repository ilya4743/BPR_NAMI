#pragma once

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <cmath>
#include <string>

namespace pt = boost::property_tree;

namespace BPR_NAMI {

class Constants {
   private:
    inline static bool isLoad;
    inline static std::string config_filename;

    Constants() {
        pt::ptree tree;
        pt::read_json(config_filename, tree);
        ip = tree.get<std::string>("IP");
        port = tree.get<unsigned short>("PORT");
        is_print_grid = tree.get<bool>("IS_PRINT_GRID");
        is_print_log = tree.get<bool>("IS_PRINT_LOG");
        is_print_log_to_file = tree.get<bool>("IS_PRINT_LOG_TO_FILE");
        is_find_path = tree.get<bool>("IS_FIND_PATH");
        reconnect_time = tree.get<size_t>("RECONNECT_TIME");
    };

    std::string ip;
    unsigned short port;
    bool is_print_grid;
    bool is_print_log;
    bool is_print_log_to_file;
    bool is_find_path;
    size_t reconnect_time;

   public:
    Constants(const Constants&) = delete;
    Constants& operator=(const Constants&) = delete;
    Constants(Constants&&) = delete;
    Constants& operator=(Constants&&) = delete;

    static Constants& GetInstance() {
        if (isLoad) {
            static Constants obj;
            return obj;
        } else
            throw;
    }

    static void SetConstatsFromFile(std::string filename) {
        config_filename = filename;
        isLoad = true;
    }

    inline std::string IP() const { return ip; }
    inline unsigned short PORT() const { return port; }
    inline auto IS_PRINT_GRID() { return is_print_grid; }
    inline auto IS_PRINT_LOG() { return is_print_log; }
    inline auto IS_FIND_PATH() { return is_find_path; }
    inline auto RECONNECT_TIME() { return reconnect_time; }
};

}  // namespace BPR_NAMI