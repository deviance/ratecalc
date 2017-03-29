

/* Represents rate intervals, for example 'below 100 kWh',
 * 'from 100 kWh to 600 kWh' etc. */
struct rate_limit {
	unsigned rated_kwh_limit;
	float kwh_cost;
};

unsigned nr_kwh_within_rate(const struct rate_limit r[],
	unsigned rate_nr, unsigned kwh_consumed);
