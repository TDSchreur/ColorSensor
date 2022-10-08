#define PST_TIME_ZONE -8
#define PST_TIME_ZONE_DAYLIGHT_SAVINGS_DIFF   1
#define GMT_OFFSET_SECS (PST_TIME_ZONE * 3600)
#define GMT_OFFSET_SECS_DST ((PST_TIME_ZONE + PST_TIME_ZONE_DAYLIGHT_SAVINGS_DIFF) * 3600)
#define UNIX_TIME_NOV_13_2017 1510592825
#define NTP_SERVERS "pool.ntp.org", "time.nist.gov"

class TimeConfig {
public:
    TimeConfig();

    void InitializeTime();
};

extern TimeConfig TimeHelper;