#include <stdio.h>
#include "rate.h"

#define min(x, y) { typeof(x) t1 = x, t2 = y; t1 < t2 ? t1 : t2}

static const struct rate NKREKU_200_rates[] = {
{
	 /* March 1, 2015 */
	.applied_since = {
		.tm_sec = 0,
		.tm_min = 0,
		.tm_hour = 0,
		.tm_mday = 1,
		.tm_mon = 2,
		.tm_year = 2015-1900,
		.tm_wday = 0,
		.tm_yday = 0,
		.tm_isdst = -1,
	},
	.limits = {
		{100, 36.6},
		{600, 63.0},
		{9999999, 140.7},
	}
},
{
	 /* September 1, 2015 */
	.applied_since = {
		.tm_sec = 0,
		.tm_min = 0,
		.tm_hour = 0,
		.tm_mday = 1,
		.tm_mon = 8,
		.tm_year = 2015-1900,
		.tm_wday = 0,
		.tm_yday = 0,
		.tm_isdst = -1,
	},
	.limits = {
		{100, 45.6},
		{600, 78.9},
		{9999999, 147.9},
	}
},
{
	 /* March 1, 2016 */
	.applied_since = {
		.tm_sec = 0,
		.tm_min = 0,
		.tm_hour = 0,
		.tm_mday = 1,
		.tm_mon = 2,
		.tm_year = 2016-1900,
		.tm_wday = 0,
		.tm_yday = 0,
		.tm_isdst = -1,
	},
	.limits = {
		{100, 57.0},
		{600, 99.0},
		{9999999, 156.0},
	}
},
{
	 /* September 1, 2016 */
	.applied_since = {
		.tm_sec = 0,
		.tm_min = 0,
		.tm_hour = 0,
		.tm_mday = 1,
		.tm_mon = 8,
		.tm_year = 2016-1900,
		.tm_wday = 0,
		.tm_yday = 0,
		.tm_isdst = -1,
	},
	.limits = {
		{100, 71.4},
		{600, 129.0},
		{9999999, 163.8},
	}
},
{
	 /* March 1, 2017 */
	.applied_since = {
		.tm_sec = 0,
		.tm_min = 0,
		.tm_hour = 0,
		.tm_mday = 1,
		.tm_mon = 2,
		.tm_year = 2017-1900,
		.tm_wday = 0,
		.tm_yday = 0,
		.tm_isdst = -1,
	},
	.limits = {
		{100, 90.0},
		{600, 168.0},
		{9999999, 168.0},
	}
}
};

/* Get the latest rate. */
const struct rate *get_recent_rate()
{
	time_t now = time(NULL);
	time_t t;
	double min = 0.0;
	int min_idx;
	size_t i;

	for (i = 0; i < sizeof(NKREKU_200_rates)/sizeof(NKREKU_200_rates[0]); i++) {
		struct tm tm_t = NKREKU_200_rates[i].applied_since;
		t = mktime(&tm_t);
		double m = difftime(now, t);
		if (m < 0)
			/* t points to future */
			continue;
		if (min == 0.0 || m < min) {
			min = m;
			min_idx = i;
		}
	}

	return &NKREKU_200_rates[min_idx];
}

/* Represent 601 as 100+500+1. Return a pointer to them. */
static unsigned *calc_electricity_waste(const struct rate *r, unsigned kwh_consumed)
{
	static unsigned waste[3];
	int i;

	for (i = 0; i < 3; i++)
		waste[i] = nr_kwh_within_rate(r->limits, i, kwh_consumed);
#if D
	printf("%d %d %d\n", waste[0], waste[1], waste[2]);
#endif
	return &waste[0];
}

/* */
float calc_electricity_cost(const struct rate *r, unsigned kwh_consumed)
{
	int i;
	float cost = 0.0;
	unsigned *ewaste = calc_electricity_waste(r, kwh_consumed);

	for (i = 0; i < 3; i++)
		cost += ewaste[i] * r->limits[i].kwh_cost;

	return cost / 100;
}

const char *str_cost(const struct rate *r, unsigned kwh_consumed,
				const char *currency, int detailed)
{
	static char buf[512];
	int i;
	size_t n = 0;

	if (detailed) {
		n += sprintf(buf + n, "%d kWh = ", kwh_consumed);
	}

	if (detailed) {
		unsigned *ewaste = calc_electricity_waste(r, kwh_consumed);

		for (i = 0; i < 3; i++) {
			if (!ewaste[i])
				break;
			n += sprintf(buf + n, "(%d kWh * %.03f %s) + ", ewaste[i],
						r->limits[i].kwh_cost / 100, currency);
		}
		if (n)
			n -= 3;
	}

	if (detailed)
		n += sprintf(buf + n, " = ");

	sprintf(buf + n, "%.02f %s", calc_electricity_cost(r, kwh_consumed),
					currency);
	return buf;
}

const char *str_rate_increase(const struct rate *r)
{
	static char buf[100];

	strftime(buf, 100, "%F", &r->applied_since);
	return buf;
}
