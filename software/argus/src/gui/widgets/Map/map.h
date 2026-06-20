#pragma once
#include "src/util/defines.h"
#include <imgui/imgui.h>
#include "map_avatar.h"

/*
ImGui encapsulated widget for rendering to .pbf file maps
- https://download.geofabrik.de/
	- example maps downloaded from here

*/

class Map {
public:
	/**
	 * @brief initializes custom gpu rendering for vector 
	 	based mapping(uses ImGui like syntax)
	 *
	 * @param min upper left hand window pixel coordinate
	 * @param max lower right hand window pixel coordinates
	 */
	Map(ImVec2 min, ImVec2 max);
	~Map(void);

	/**
	 * @brief libosmium pbf file to vector map
	 *
	 * @param file_path '.pbf' required, fetch 
		from 'https://download.geofabrik.de/' or alternative
	 */
	void load_pbf(const char* file_path);
	/**
	 * @brief remove gpu and stored information to pbf, 
	 	load does that for you automatically 
	 */
	void unload_pbf(void);

	
	/**
	 * @brief maps latitude longitude coordinates to map spaces
	 *
	 * @details to capture San Francisco within world '.pbf'
		Upper-left: (37.8324, -122.5149)
		Lower-right: (37.7049, -122.3570)
	 *
	 * @param min upper left lat/long coord
	 * @param max lower right lat/long coord
	 */
	void draw(GeoCoord min, GeoCoord max);
	/**
	 * @brief alternative to just draw without specified coordinates
	 */
	void draw(void);

	/**
	 * @brief moves map coordinate
	 *
	 * @param longitude y-coordinate move
	 * @param latitude x-coordinate move
	 */
	void move(f32 longitude, f32 latitude);
	/**
	 * @brief idk how this going to work
	 *
	 * @param factor yabba dabba doo
	 */
	void zoom(f32 factor);

	// ---- Setting up Avatar ----
	/**
	 * @brief generates avatar to render ontop of map
	 *
	 * @param location starting positon
	 */
	void create_avatar(GeoCoord location);
	/**
	 * @brief removes avatar and track log
	 */
	void destroy_avatar(void); 

	MapAvatar& get_avatar(void);


private:
	MapAvatar avatar_;

	ImVec2 pmin_, pmax_; // window pixel coords
	GeoCoord geomin_, geomax_; // coordinates fo where map is zoomed into
};
