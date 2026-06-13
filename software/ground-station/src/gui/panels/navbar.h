#pragma once

#include "panel.h"
#include "src/assets/image_loader.h"
#include <vector>

#define ICON_COUNT 6
/*
Navigation List:
- dashboard.png
- map.png
- plan.png
- drone.png
- log.png
- setting.png
	- popup window that allows for changing 


*/

class Navbar : public Panel {
public:
	Navbar(const std::string name);
	~Navbar(void);
	void draw(void) override;
	void configure(void) override;
private:
	Texture icons[ICON_COUNT];
	bool active[ICON_COUNT];

	// changes other panes to fit nav change
	void nav_change(u32 identifier);
};
