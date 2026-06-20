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
