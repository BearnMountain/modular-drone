#include "clock.h"
#include <ctime>

Clock::log Clock::now(void) {
	std::time_t timestamp = std::time(nullptr);
    std::tm* local_time = std::localtime(&timestamp);

	return {
        static_cast<u8>(local_time->tm_hour),
        static_cast<u8>(local_time->tm_min),
        static_cast<u8>(local_time->tm_sec)
    };
}

const char* Clock::get_timezone(void) {
	static char buf[8];

    time_t now = time(nullptr);
    tm local{};
    localtime_r(&now, &local);

    strftime(buf, sizeof(buf), "%Z", &local);
    return buf;
}

const char* Clock::get_utc() {
    static char buf[16];

    time_t now = time(nullptr);
    tm local{};
    localtime_r(&now, &local);

    char offset[8];
    strftime(offset, sizeof(offset), "%z", &local); // +HHMM

    int hours = (offset[1] - '0') * 10 + (offset[2] - '0');
    if (offset[0] == '-') hours = -hours;

    snprintf(buf, sizeof(buf), "UTC%+d", hours);
    return buf;
}
