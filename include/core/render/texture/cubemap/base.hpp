#ifndef __CUBEMAP_BASE__
#define __CUBEMAP_BASE__

#include <memory>
#include <vector>
#include <filesystem>
#include <fmt/format.h>

#include "tools/logger.hpp"
#include "core/render/texture/base.hpp"


namespace Engine {
    namespace Render {
        class Cubemap : public Texture {
        protected:
            std::unique_ptr<Tool::Logger::Logger> logger = 
                std::make_unique<Tool::Logger::Logger>("cubemap");

        public:
            uint32_t object = 0;

            static Cubemap *GetInstance();
            void Build(std::filesystem::path) {};
            virtual void Build(std::vector<std::filesystem::path>) = 0;
            virtual void Bind() = 0;
        };
    }
}

#endif
