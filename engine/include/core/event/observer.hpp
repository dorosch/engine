#ifndef __EVENT_OBSERVER_HPP__
#define __EVENT_OBSERVER_HPP__


namespace Engine {
    namespace Event {
        enum EventType {

        };


        class Event {
        public:
            EventType type;
        };


        class EventObserver {
        private:
            static EventObserver *_singleton;

        public:
            void Publish(Event *);
            void Subscribe();
            void Unsubscribe();
            static EventObserver *GetInstance();
        };
    }
}


#endif
