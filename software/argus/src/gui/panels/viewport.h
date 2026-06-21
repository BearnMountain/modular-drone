#pragma once

#include "src/assets/image_loader.h"
#include "src/gui/panels/panel.h"
#include "src/gui/widgets/Map/map.h"
#include <vector>

// Types:
// 	- map
// 	- 3d
// 	- camera
// 	- thermal

class Viewport : public Panel {
public:
	Viewport(const std::string name, Renderer::BackendInitDesc* desc);
	~Viewport(void);
	void draw(void) override;
	void configure(void) override;
private:
	Renderer::BackendInitDesc* desc_;
	// std::unique_ptr<Map> map_;

};
