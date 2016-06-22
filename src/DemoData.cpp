#include <DemoData.h>

namespace Acidrain {

std::vector<std::shared_ptr<Texture>>   DemoData::textures;
std::vector<std::shared_ptr<Shader>>    DemoData::shaders;
std::vector<std::shared_ptr<Material>>  DemoData::materials;
std::vector<std::shared_ptr<Mesh>>      DemoData::meshes;
std::vector<std::shared_ptr<Fbo>>       DemoData::fbos;

int DemoData::WIDTH     = 1024;
int DemoData::HEIGHT    = 768;

} // namespace Acidrain