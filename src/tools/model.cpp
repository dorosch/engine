#include "tools/model.hpp"

using namespace Tool;


Model::Model(std::filesystem::path path) {
    this->path = path;
}


void ObjModel::Load() {
    std::ifstream file(this->path);

    if (file.is_open()) {
        std::string line;

        while (std::getline(file, line)) {
            if (line.substr(0, 2) == "v ") {
                std::istringstream stream(line.substr(2));

                glm::vec4 v;
                stream >> v.x;
                stream >> v.y;
                stream >> v.z;
                v.w = 1.0f;

                vertices.push_back(v);
            }
            else if (line.substr(0, 2) == "f ") {
                // f line can also have normals v/vn/n

                // std::istringstream stream(line.substr(2));

                int a,b,c; //to store mesh index
                int A,B,C; //to store texture index
                //std::istringstream v;
                //v.str(line.substr(2));
                const char* chh=line.c_str();
                sscanf (chh, "f %i/%i %i/%i %i/%i",&a,&A,&b,&B,&c,&C);

                //v>>a;v>>b;v>>c;
                a--;b--;c--;
                A--;B--;C--;
                //std::cout<<a<<b<<c<<A<<B<<C;
                indices.push_back(a); textures.push_back(A);
                indices.push_back(b); textures.push_back(B);
                indices.push_back(c); textures.push_back(C);

                // uint32_t a, b, c;
                // stream >> a;
                // stream >> b;
                // stream >> c;

                // std::cout << line << std::endl;
                // std::cout << a << " " << b << " " << c << std::endl;

                // indices.push_back(--a);
                // indices.push_back(--b);
                // indices.push_back(--c);
            }
        }

        file.close();
    }
    else {
        // TODO: Replace on logger
        std::cout << "Error: cannot open file: " << this->path << std::endl;
    }
}
