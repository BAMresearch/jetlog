#include <iostream>
#include <string>

#include <jetlog/jetlog_json.hpp>

namespace magic_forest {

struct WonderfulDataType
{
    bool fairytale_mode;
    std::string title;
    int wizard_count;
};

void to_json(jetlog::json& j, const WonderfulDataType& data)
{
    j = jetlog::json{{"fairytale_mode", data.fairytale_mode},
                     {"title", data.title},
                     {"wizard_count", data.wizard_count}};
}

} // namespace magic_forest

#include <jetlog/jetlog.hpp>

int main()
{
    std::cout << ">>> Jetlog C++ example <<<" << std::endl;

    magic_forest::WonderfulDataType data;
    data.fairytale_mode = true;
    data.title = "Gourmet Island";
    data.wizard_count = 3;

    std::cout << "\nLogging:" << std::endl;
    std::cout << jetlog::log("Hello world") << std::endl;
    std::cout << jetlog::log("Hello world", jetlog::Notice) << std::endl;
    std::cout << jetlog::log("Hello world", jetlog::Error, "main") << std::endl;
    std::cout << jetlog::log("Hello world", jetlog::Error, "main", data) << std::endl;

    std::cout << "\nISO 8601 timestamp:" << std::endl;
    std::cout << jetlog::getEntry("wishlist", data) << std::endl;
    std::cout << jetlog::getEntry<std::chrono::microseconds>("wishlist", data) << std::endl;
    std::cout << jetlog::getEntryNow("wishlist", {{"receiver", "Santa Claus"}}) << std::endl;
    std::cout << jetlog::getEntryNow("wishlist", data) << std::endl;
    std::cout << jetlog::getEntryNow("wishlist", {{"receiver", "Santa Claus"}}, data) << std::endl;

    std::cout << "\nUnix timestamp:" << std::endl;
    std::cout << jetlog::getEntryUnixTime("wishlist", data) << std::endl;;
    std::cout << jetlog::getEntryUnixTime<std::chrono::seconds, std::chrono::milliseconds>("wishlist", data) << std::endl;;
    std::cout << jetlog::getEntryUnixTime<std::chrono::milliseconds>("wishlist", data) << std::endl;;

    std::cout << "\nSystem timestamp:" << std::endl;
    std::cout << jetlog::getEntrySysTime("wishlist", data) << std::endl;
    std::cout << jetlog::getEntrySysTime<std::chrono::seconds, std::chrono::milliseconds>("wishlist", data) << std::endl;
    std::cout << jetlog::getEntrySysTime<std::chrono::microseconds>("wishlist", data) << std::endl;
    std::cout << jetlog::getEntrySysTime<std::chrono::milliseconds, std::chrono::microseconds>("wishlist", data) << std::endl;

    std::cout << "\nSystem timestamp with custom time source:" << std::endl;
    double system_time = M_PI; // [s]
    std::cout << jetlog::getEntrySysTime("wishlist", data, system_time) << std::endl;
    std::cout << jetlog::getEntrySysTime<std::chrono::milliseconds, std::chrono::nanoseconds>("wishlist", data, system_time) << std::endl;

    return 0;
}
