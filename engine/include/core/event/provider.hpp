#ifndef __EVENT_PROVIDER_HPP__
#define __EVENT_PROVIDER_HPP__


namespace Engine {
    namespace Event {
        class EventProvider {
        public:
            virtual void MouseEventCallback() = 0;
            virtual void KeyboardEventCallback() = 0;

            static void MouseEventCallbackStatic();
            static void KeyboardEventCallbackStatic();
        };
    }
}


#endif
