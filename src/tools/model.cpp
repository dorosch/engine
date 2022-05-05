#include "tools/model.hpp"

using namespace Tool;


Model::Model(std::filesystem::path path) {
    this->path = path;
}


void ObjModel::Load() {
    std::ifstream file(this->path);

    std::vector<uint32_t> vertexIndices;
    std::vector<glm::vec4> temp_vertices;
    std::vector<uint32_t> textureIndices;
    std::vector<glm::vec2> temp_textures;

    if (file.is_open()) {
        std::string line;

        while (std::getline(file, line)) {
            if (line.substr(0, 2) == "v ") {
                float x, y, z;
                glm::vec4 v;

                sscanf (line.c_str(), "v %f %f %f", &x, &y, &z);

                v.x = x;
                v.y = y;
                v.z = z;
                v.w = 1.0f;

                temp_vertices.push_back(v);
            }
            else if (line.substr(0, 3) == "vt ") {
                float x, y, z;
                glm::vec2 v;

                if (std::count(line.begin(), line.end(), ' ') == 2) {
                    sscanf (line.c_str(), "vt %f %f", &x, &y);
                }
                else {
                    sscanf (line.c_str(), "vt %f %f %f", &x, &y, &z);
                }

                v.x = x;
                v.y = y;

                temp_textures.push_back(v);
            }
            else if (line.substr(0, 2) == "f ") {
                std::istringstream stream(line.substr(2));
                std::string token;
                int vertice, texture, normal;

                while (std::getline(stream, token, ' ')) {
                    if (token.length() <= 1) {
                        continue;
                    }

                    if (token.find("//") != std::string::npos) {
                        sscanf(token.c_str(), "%i//%i", &vertice, &normal);
                    }
                    else if (token.find('/') != std::string::npos) {
                        if (std::count(token.begin(), token.end(), '/') == 1) {
                            sscanf(token.c_str(), "%i/%i", &vertice, &texture);
                            textureIndices.push_back(texture);
                        }
                        else {
                            sscanf(token.c_str(), "%i/%i/%i", &vertice, &texture, &normal);
                            textureIndices.push_back(texture);
                        }
                    }
                    else {
                        sscanf(token.c_str(), "%i", &vertice);
                    }

                    vertexIndices.push_back(vertice);
                }
            }
        }

        file.close();
    }
    else {
        // TODO: Replace on logger
        std::cout << "Error: cannot open file: " << this->path << std::endl;
    }

    for( uint32_t i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		uint32_t vertexIndex = vertexIndices[i];
        glm::vec4 a = temp_vertices[vertexIndex - 1];

		// Put the attributes in buffers
		this->vertices.push_back(a);
    }

    for (uint32_t i=0; i < textureIndices.size(); i++) {
		uint32_t textureIndex = textureIndices[i];
        glm::vec2 b = temp_textures[textureIndex - 1];

		this->textures.push_back(b);
    }	
}


std::vector<float> ObjModel::Data() {
    std::vector<float> data;

    for (size_t i = 0; i < vertices.size(); ++i) {
        glm::vec4 vertice = vertices[i];
        glm::vec2 texture = textures[i];

        data.push_back(vertice.x);
        data.push_back(vertice.y);
        data.push_back(vertice.z);
        data.push_back(texture.x);
        data.push_back(texture.y);
    }

    return data;
}
