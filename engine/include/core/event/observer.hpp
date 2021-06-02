#ifndef __EVENT_OBSERVER_HPP__
#define __EVENT_OBSERVER_HPP__

#include <vector>
#include <algorithm>

#include "tools/logger.hpp"

using namespace Tool::Logger;


namespace Engine {
    namespace Event {
        enum EventType {
            Mouse,
            Keyboard
        };


        class Event {
        public:
            EventType type;
            Event(EventType);
        };


        typedef void (*EventCallback)(Event *);


        class EventObserver {
        private:
            static EventObserver *_singleton;
            std::vector<EventCallback> _subscribers;
            std::unique_ptr<Logger> logger = std::make_unique<Logger>("eobsr");

        public:
            void Publish(Event *);
            void Subscribe(EventCallback);
            void Unsubscribe(EventCallback);
            static EventObserver *GetInstance();
        };
    }
}


#endif
