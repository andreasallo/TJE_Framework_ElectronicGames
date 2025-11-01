#pragma once

#include "framework/entities/entity.h"

#include "graphics/material.h"

#include <map>

class SceneParser {

	std::map<std::string, sRenderData> meshes_to_load;

public:
	bool parse(const char* filename, Entity* root); // le paso el root en puntero pk me pones todo dentro de root.
};