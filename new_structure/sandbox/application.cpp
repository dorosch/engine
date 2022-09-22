#include <memory>

#include "radian.hpp"
#include "tools/logger.hpp"


class Application : public Radian::Application {
public:
    void startup() override {
        logger->info("Application startup");
    }

    void run() override {
        logger->info("Application run");
    }

    void shutdown() override {
        logger->info("Application shutdown");
    }
};


int main() {
    std::unique_ptr<Application> app = std::make_unique<Application>();

    Radian::engine->run(std::move(app));
}
