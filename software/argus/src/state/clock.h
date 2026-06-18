#include "src/util/defines.h"
#include <string>

namespace Clock {

	struct log{
		u8 hour;
		u8 minute;
		u8 second;
	};

	/**
	 * @brief current time of call
	 *
	 * @return HH::MM::SS
	 */
	log now(void);

	/**
	 * @brief gets current timezone
	 *
	 * @return timezone acronyms
	 */
	const char* get_timezone(void);
	/**
	 * @brief relative to european time
	 *
	 * @return 'UTC+N' where n is hour offset from western europe
	 */
	const char* get_utc(void);

}
