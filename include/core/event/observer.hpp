#ifndef __INPUT_OBSERVER__
#define __INPUT_OBSERVER__

#include <vector>
#include <algorithm>


namespace Engine {
    namespace Event {
        typedef void (*Callback)();

        class Observer {
        private:
            static Observer *singleton;
            std::vector<Callback> subscribers;

        public:
            static Observer *GetInstance();
            void Publish();
            void Subscribe(Callback);
            void Unsubscribe(Callback);
        };
    }
}

#endif
