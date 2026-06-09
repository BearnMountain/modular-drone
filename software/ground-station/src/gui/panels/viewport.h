#pragma once

#include "src/gui/panels/panel.h"

// Types:
// 	- map
// 	- 3d
// 	- camera
// 	- thermal

class Viewport : public Panel {
public:
	Viewport(const std::string name);
	void draw(void) override;
private:

};
