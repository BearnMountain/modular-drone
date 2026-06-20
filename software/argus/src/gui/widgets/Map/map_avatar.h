#pragma once

#include "src/util/defines.h"
#include <vector>

struct GeoCoord {
	f32 latitude, longitude;
};

class MapAvatar {
public:
	MapAvatar(GeoCoord location);
	~MapAvatar(void);

	const std::vector<GeoCoord>& get_track_log(void) const;
	void dump_track_log(void);

	void move(GeoCoord location);

private:
	GeoCoord location_;
	std::vector<GeoCoord> track_log_;
};
