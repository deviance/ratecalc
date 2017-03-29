#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "rate.h"
#include "expr.h"

#define D 0

int main(int argc, char *argv[])
{
	int args_on_cmdline = 0;
	int kwh_consumed;
	int err;
	int nread;
	char buf[1024];

	if (argc > 1)
		args_on_cmdline = 1;

	for (;;) {
		if (!args_on_cmdline) {
			nread = scanf("%s\n", &buf[0]);

			if (nread == EOF && ferror(stdin)) {
				perror("scanf");
				return -1;
			} else if (nread == EOF ) {
				break;
			}
		}

		err = expr_eval(args_on_cmdline ? argv[1] : buf, &kwh_consumed);

		if (err) {
			fprintf(stderr, "expr_eval: error: %d\n", err);
			continue;
		}

		struct rate r = *get_recent_rate();

		calc_electricity_cost(&r, kwh_consumed);
		puts(str_cost(&r, kwh_consumed, "UAH", 0));

		if (args_on_cmdline)
			break;

	}
	return 0;
}
