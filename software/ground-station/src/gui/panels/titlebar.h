#pragma once

// top of application

#include "src/gui/panels/panel.h"

class Titlebar : public Panel {
public:
	Titlebar(const std::string name);

	void draw(void) override;
private:

};
