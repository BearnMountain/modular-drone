#pragma once

#include "panel.h"
#include <queue>

enum FlightMode {
	MANUAL,
	HOVER,
	RETURN
};

enum GPSStatus {
	NO_FIX, // cannot find 4x satellites to communicate with
	SINGLE, // USING SINGLE FREQUENCY signal
	FLOAT, // narrowing down its own location
	RTX_FIX, // locked onto satellite
	INITIALIZING // trying to find those 4x satellites to communicate with
};

enum SystemStatus {
	// status type
	// subsystem
};

class Infobar : public Panel {
public:
	Infobar(const std::string name);
	void draw(void) override;
	void configure(void) override;

	// set/get information
	void change_flight_mode(FlightMode mode);
	void update_altitude(f32 dt, f32 altitude); // each new time is appeneded to altitude queue and graphed
	void update_ground_speed_ms(f32 dt, f32 altitude); // each new time is appeneded to altitude queue and graphed
	void update_heading(f32 heading); // 0-360
	void update_battery(u8 battery); // 0-100 integer
	void update_link_quality(u8 signal); // 0-100 integer
	void update_gps_status(GPSStatus status);
	void update_system_status(SystemStatus status);
private:
	std::deque<std::pair<f32, f32>> altitude_; // time, altitude
	std::deque<std::pair<f32, f32>> ground_speed_ms_;
	f32 heading_;
	u8 battery_;
	u8 link_quality_;
	GPSStatus gps_status_;

	// void 
	void draw_vehicle(void);
	void draw_telemetry(u32 win_width, u32 win_height);
	void draw_system_status(void);
	// Options: auto, manual, 
	void draw_flight_mode(void); 
};
