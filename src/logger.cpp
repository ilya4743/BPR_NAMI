#include "logger.h"

#include <boost/date_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "constants_app.h"

namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)

void MyFormatter(logging::record_view const& rec, logging::formatting_ostream& strm) {
    auto ts = rec[timestamp];
    strm << to_iso_extended_string(*ts) << ' ';
    // выводим уровень сообщения
    strm << '[' << rec[logging::trivial::severity] << "] ";
    // выводим само сообщение
    strm << rec[expr::smessage];
}

void InitLogger() {
    logging::add_common_attributes();

    // logging::add_file_log(
    //     keywords::file_name = "sample.log",
    //     keywords::format = &MyFormatter,
    //     keywords::open_mode = std::ios_base::app | std::ios_base::out
    // );

    if (!BPR_NAMI::Constants::GetInstance().IS_PRINT_LOG()) {
        boost::log::core::get()->set_filter(boost::log::trivial::severity == boost::log::trivial::error);
    }
    logging::add_console_log(std::cout, logging::keywords::format = &MyFormatter,
                             logging::keywords::auto_flush = true);
}