#ifndef __SCENE__
#define __SCENE__

#include <memory>
#include <vector>

#include "core/object/object.hpp"
#include "core/graphics/lighting/base.hpp"


namespace Engine {
    namespace Scene {
        class Node {
        public:
            std::string name;
            std::unique_ptr<Node> parent;
            std::vector<std::shared_ptr<Node>> children;
            std::vector<std::shared_ptr<Object>> entities;

            Node(std::string name) {
                this->name = name;
            }
        };


        class Scene {
        public:
            std::shared_ptr<Node> root;
            std::vector<std::shared_ptr<Graphics::Lighting::Light>> lighting;

            Scene();
            virtual ~Scene() {};
        };
    }
}

#endif
