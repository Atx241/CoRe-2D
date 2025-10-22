#include "Renderer.h"
extern "C" {
    Core::Texture* CreateTexture(unsigned char* data, int width, int height, bool linear) {
        return Core::Texture::CreateTexture(data, width, height, linear);
    }
    void CreateRenderEntity(Core::Entity* e) {
        Core::Entity::CreateRenderEntity(e);
    }
    void CreateAnchoredRenderEntity(Core::AnchoredEntity* e) {
        Core::Entity::CreateRenderEntity(e);
    }
}