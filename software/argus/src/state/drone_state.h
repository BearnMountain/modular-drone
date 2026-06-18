#pragma once

#include "src/util/defines.h"

/*
Stores all information regarding the drone that is sent to the 
transiever

   */

namespace DroneState {

	enum GPS {
		GPS_RTK_FIXED, // 1-2cm accuracy
		GPS_RTK_FLOAT, // 10-50cm accuracy
		GPS_TOTAL
	};

	/**
	 * @brief gets drone elrs ping and converts to link quality
	 *
	 * @return 0.0 - 1.0f link quality
	 */
	f32 get_link_quality(void);
};
