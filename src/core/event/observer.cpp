#include "core/event/observer.hpp"

using namespace Engine::Event;


Observer* Observer::singleton = nullptr;


Observer *Observer::GetInstance() {
    if(singleton == nullptr) {
        singleton = new Observer();
    }

    return singleton;
}


void Observer::Publish() {
    for(auto subscriber : subscribers) {
        (*subscriber)();
    }
}


void Observer::Subscribe(Callback subscriber) {
    subscribers.push_back(subscriber);
}


void Observer::Unsubscribe(Callback subscriber) {
    std::remove(subscribers.begin(), subscribers.end(), subscriber);
}
