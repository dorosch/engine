#ifndef __TEXTURE_BASE__
#define __TEXTURE_BASE__

#include <memory>
#include <filesystem>

#include <GL/glew.h>

#include "tools/logger.hpp"


namespace Engine {
    namespace Render {
        class Texture {
        protected:
            std::unique_ptr<Tool::Logger::Logger> logger = 
                std::make_unique<Tool::Logger::Logger>("texture");

        public:
            uint32_t object = 0;

            static Texture *GetInstance();
            virtual void Build(std::filesystem::path) = 0;
            virtual void Bind() = 0;
        };
    }
}

#endif
