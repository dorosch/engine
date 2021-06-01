#include "core/event/observer.hpp"


namespace Engine {
    namespace Event {
        Event::Event(EventType type) {
            this->type = type;
        }


        void EventObserver::Publish(Event *event) {
            logger->info("Need to publish new event!!1");
        }


        void EventObserver::Subscribe() {

        }


        void EventObserver::Unsubscribe() {

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
