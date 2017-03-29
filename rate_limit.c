#include "rate_limit.h"

/* I need to know how much electricity I consumed within given rate. Say
 * 153 kWh is consumed and the interval of interest is 100-600, so
 * 53 will be returned. */
unsigned nr_kwh_within_rate(const struct rate_limit r[],
	unsigned rate_nr, unsigned kwh_consumed)
{
	unsigned curr_limit = r[rate_nr].rated_kwh_limit;
	unsigned prev_limit = rate_nr > 0 ? r[rate_nr - 1].rated_kwh_limit : 0;

#if D
	printf("rate_nr %d\n", rate_nr);
	printf("curr_limit %d, prev_limit %d\n", curr_limit, prev_limit);
	fflush(stdout);
#endif
	if (kwh_consumed > curr_limit) {
#if D
		printf("curr_limit - prev_limit %d\n", curr_limit - prev_limit);
		fflush(stdout);
#endif
		return curr_limit - prev_limit;
	}

	int remainder = kwh_consumed - prev_limit;

#if D
	printf("remainder %d\n", remainder > 0 ? remainder : 0);
	fflush(stdout);
#endif
	return remainder > 0 ? remainder : 0;
}

