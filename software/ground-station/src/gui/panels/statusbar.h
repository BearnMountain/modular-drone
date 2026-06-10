#pragma once

#include "panel.h"

class Statusbar : public Panel {
public:
	Statusbar(const std::string name);
	void draw(void) override;
	void configure(void) override;
private:

};
