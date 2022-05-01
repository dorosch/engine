#ifndef __RENDER_API__
#define __RENDER_API__


namespace Engine {
    namespace Render {
        enum Backend {
            OPENGL,
        };


        static Backend BackendAPI = Backend::OPENGL;


        Backend GetBackendAPI();
        void SetBackendAPI(Backend);
    }
}

#endif
