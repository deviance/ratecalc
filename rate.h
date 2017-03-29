#include <time.h>
#include "rate_limit.h"

struct rate {
	struct tm applied_since;
	struct rate_limit limits[3];
};

#ifdef __cplusplus
extern "C" {
#endif
/* Get the latest rate. */
const struct rate *get_recent_rate();
/**/
const char *str_cost(const struct rate *r, unsigned kwh_consumed,
				const char *currency, int detailed);
const char *str_rate_increase(const struct rate *r);
float calc_electricity_cost(const struct rate *r, unsigned kwh_consumed);
#ifdef __cplusplus
}
#endif
