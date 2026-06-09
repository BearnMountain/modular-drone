#pragma once

#include "src/gui/panels/panel.h"

class Toolbar : public Panel {
public:
	Toolbar(const std::string name);
	void draw(void) override;
private:
};
