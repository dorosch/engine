#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__


namespace Library {
    namespace Pattern {
        class Singleton {
        protected:
            static Singleton *_singleton;

        public:
            void operator=(const Singleton &) = delete;

            static Singleton *GetInstance() {
                if(_singleton == nullptr){
                    _singleton = new Singleton();
                }

                return _singleton;
            }
        };
    }
}


#endif
