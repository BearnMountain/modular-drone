#pragma once

#include "panel.h"

class Navbar : public Panel {
public:
	Navbar(const std::string name);
	void draw(void) override;
private:

};
