#pragma once

#include <vector>
#include <memory>
#include <Shader.h>

class ShaderManager {
public:

	static void init(); 
	static std::shared_ptr<Shader> get(int index);

private:

	static std::vector<std::shared_ptr<Shader>> shaders;
};
