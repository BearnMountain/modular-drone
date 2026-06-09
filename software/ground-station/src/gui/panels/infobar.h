#pragma once

#include "panel.h"

class Infobar : public Panel {
public:
	Infobar(const std::string name);
	void draw(void) override;
private:

};
