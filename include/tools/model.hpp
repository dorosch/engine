#ifndef __MODEL__
#define __MODEL__

#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Tool {
    class Model {
    public:
        std::filesystem::path path;
        std::vector<glm::vec4> vertices;
        std::vector<uint32_t> indices;
        std::vector<glm::vec2> textures;

        Model(std::filesystem::path);
        virtual void Load() = 0;
    };


    class ObjModel : public Model {
    public:
        ObjModel(std::filesystem::path path) : Model(path) {};
        void Load();
    };
}

#endif
