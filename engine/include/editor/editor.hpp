#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include <memory>

#include "engine.hpp"
#include "tools/logger.hpp"

using namespace Tool::Logger;


namespace Engine {
    class EngineApplication;


    namespace Editor {
        class EngineEditor {
        private:
            EngineApplication *app = nullptr;

        public:
            std::unique_ptr<Logger> logger = std::make_unique<Logger>("editor");

            EngineEditor(EngineApplication *);
            void Init();
            void Update();
            void Shutdown();
        };
    }
}


#endif
