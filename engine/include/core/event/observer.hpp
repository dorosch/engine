#ifndef __EVENT_OBSERVER_HPP__
#define __EVENT_OBSERVER_HPP__

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


        class EventObserver {
        private:
            static EventObserver *_singleton;
            std::unique_ptr<Logger> logger = std::make_unique<Logger>("eobsr");

        public:
            void Publish(Event *);
            void Subscribe();
            void Unsubscribe();
            static EventObserver *GetInstance();
        };
    }
}


#endif
