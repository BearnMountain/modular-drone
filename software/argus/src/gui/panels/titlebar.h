#pragma once

// top of application

#include "src/gui/panels/panel.h"
#include "src/assets/image_loader.h"

class Titlebar : public Panel {
public:
	Titlebar(const std::string name);
	~Titlebar(void);

	void draw(void) override;
	void configure(void) override;


	// each mission stores data that can be saved
	void set_mission_name(const char* mission_name_);
	void save_mission(void);
private:
	std::string mission_name_;
};
