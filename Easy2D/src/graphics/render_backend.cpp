#include <easy2d/graphics/render_backend.h>
#include <easy2d/graphics/opengl/gl_renderer.h>

namespace easy2d {

UniquePtr<RenderBackend> RenderBackend::create(BackendType type) {
    switch (type) {
        case BackendType::OpenGL:
            return makeUnique<GLRenderer>();
        default:
            return nullptr;
    }
}

} // namespace easy2d
