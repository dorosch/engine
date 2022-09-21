#include <memory>
#include <iostream>

#include "radian.hpp"


class Application : public Radian::Application {
public:
    void startup() override {
        std::cout << "startup" << std::endl;
    }

    void run() override {
        std::cout << "run" << std::endl;
    }

    void shutdown() override {
        std::cout << "shutdown" << std::endl;
    }
};


int main() {
    std::unique_ptr<Application> app = std::make_unique<Application>();

    Radian::engine->run(std::move(app));
}
