#ifndef __MANAGER_HPP__
#define __MANAGER_HPP__


namespace Engine {
    namespace Manager {
        class EngineManager {
            /**
             * Engine manager interface.
             * 
             * The engine has several areas of responsibility. For example 
             * window system, event system, rendering, and so on. The engine 
             * delegates each of these zones to the manager responsible 
             * for this zone.
             * 
             * For example, a window manager is responsible only for creating 
             * and working with a window. All managers are controlled by 
             * engine specifying correct order of their usage and interaction. 
             */

        public:
            virtual ~EngineManager() {}
            virtual void Init() = 0;
            virtual void Update() = 0;
            virtual void Shutdown() = 0;
        };
    }
}


#endif
