#include "drone_state.h"

static f32 link_quality = 0.84f;

f32 DroneState::get_link_quality(void) {
	return link_quality;
}
