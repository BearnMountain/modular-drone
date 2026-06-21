#include "map.h"

// class MercatorCoordinate {
//   static mercatorXfromLng(lng) {
//     return (180 + lng) / 360;
//   }
//
//   static mercatorYfromLat(lat) {
//     return (180 - (180 / Math.PI * Math.log(Math.tan(Math.PI / 4 + lat * Math.PI / 360)))) / 360;
//   }
//
//   static fromLngLat(lngLat) {
//     const x = MercatorCoordinate.mercatorXfromLng(lngLat[0]);
//     const y = MercatorCoordinate.mercatorYfromLat(lngLat[1]);
//     return [x, y];
//   }
// }

Map::Map(Renderer::BackendInitDesc* desc, ImVec2 min, ImVec2 max) {
	avatar_ = std::make_unique<MapAvatar>(GeoCoord(0.0f,0.0f));
	desc_ = desc;
	pmin_ = min;
	pmax_ = max;
}

Map::~Map(void) {

}

void Map::draw(void) {

	ImGui::Image(
		create_map_texture(), 
		ImVec2(100.0f, 100.0f)
	);
}
