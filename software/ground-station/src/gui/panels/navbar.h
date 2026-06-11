#pragma once

#include "panel.h"
#include <vector>

#define ICON_COUNT 6
/*
Icon List:
 - dashboard.svg
 - info.svg
 - log.svg
 - map.svg
 - settings.svg
 - vehicle.svg
*/

class Navbar : public Panel {
public:
	Navbar(const std::string name);
	~Navbar(void);
	void draw(void) override;
	void configure(void) override;
private:
	// ImTextureID icons[ICON_COUNT];
};
