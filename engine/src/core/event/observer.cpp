#include "core/event/observer.hpp"


namespace Engine {
    namespace Event {
        Event::Event(EventType type) {
            this->type = type;
        }


        void EventObserver::Publish(Event *event) {
            logger->trace("Publish");

            for(
                auto subscriber = this->_subscribers.begin();
                subscriber < this->_subscribers.end();
                subscriber++
            ) {
                (*subscriber)(event);
            }
        }


        void EventObserver::Subscribe(EventCallback subscriber) {
            logger->trace("Subscribe");

            this->_subscribers.push_back(subscriber);
        }


        void EventObserver::Unsubscribe(EventCallback subscriber) {
            logger->trace("Unsubscribe");

            this->_subscribers.erase(
                std::remove(
                    this->_subscribers.begin(),
                    this->_subscribers.end(),
                    subscriber
                ), this->_subscribers.end()
            );
        }


        EventObserver* EventObserver::_singleton = nullptr;

        EventObserver *EventObserver::GetInstance() {
            if(_singleton == nullptr){
                _singleton = new EventObserver();
            }

            return _singleton;
        }
    }
}
