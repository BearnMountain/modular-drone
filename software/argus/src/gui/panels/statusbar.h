#pragma once

#include "panel.h"
#include <vector>
#include "src/state/clock.h"

class Statusbar : public Panel {
public:
	Statusbar(const std::string name);
	void draw(void) override;
	void configure(void) override;

	// stops previous timestamp and creates a new one
	void add_timestep(
		const std::string& label // TODO: add timestep descriptor for verbose logging
	);
	
	// prints timestamps_ info
	void print_debug_status(void);
private:
	struct Timestep {
		std::string label;
		Clock::log timestamp;
	};
	std::vector<Timestep> timeline_;
	
	void draw_chevron(ImDrawList* dl, ImVec2 pos, float w, float h);
};
