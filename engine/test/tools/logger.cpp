#include <catch2/catch.hpp>

#include "tools/logger.hpp"


Tool::Logger::Logger logger("test");


TEST_CASE("Logger", "[tool][logger]") {
    SECTION("default log level") {
        REQUIRE(logger.GetLevel() == Tool::Logger::Level::info);
    }

    SECTION("custom log level") {
        logger.SetLevel(Tool::Logger::Level::trace);

        REQUIRE(logger.GetLevel() == Tool::Logger::Level::trace);
    }
}
