#pragma once

#include "src/assets/image_loader.h"
#include "src/gui/panels/panel.h"
#include <vector>

// Types:
// 	- map
// 	- 3d
// 	- camera
// 	- thermal

class Viewport : public Panel {
public:
	Viewport(const std::string name);
	~Viewport(void);
	void draw(void) override;
	void configure(void) override;
private:
	std::vector<Texture> textures;
};
