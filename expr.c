#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "expr.h"

struct stack {
	int data[100];
	int top;
};

static void stack_init(struct stack *s)
{
	s->top = -1;
}

static void stack_push(struct stack *s, int v)
{
	if (s->top > 99)
		return;

	s->top += 1;
	s->data[s->top] = v;
}

static int stack_pop(struct stack *s)
{
	if (s->top < 0)
		return 0;

	int v = s->data[s->top];
	s->top -= 1;
	return v;
}

static int stack_empty(struct stack *s)
{
	return s->top == -1;
}

static int stack_count(const struct stack *s)
{
	return s->top >= 0 ? s->top + 1 : 0;
}

static void infix_to_rpn(const char *expr, char rpn[])
{
	int n = 0, i = 0;
	struct stack operators;

	stack_init(&operators);
	while (expr[i]) {
		if (isspace(expr[i])) {
			i++;
			continue;
		} else if (isdigit(expr[i])) {
			int x = atoi(&expr[i]);
			n += sprintf(rpn + n, "%d ", x);
			while (isdigit(expr[i]))
				i++;
			if (!stack_empty(&operators))
				n += sprintf(rpn + n, "%c ", stack_pop(&operators));
		} else switch(expr[i]) {
			case '+':
			case '-':
			case '*':
			case '/':
				stack_push(&operators, expr[i]);
				i++;
				break;
			default:
				i++;
				break;
		}
	}
}

static int rpn_eval(char *rpn, int *result)
{
	int i = 0;
	struct stack operators, operands;

	stack_init(&operands);
	stack_init(&operators);

	while (rpn[i]) {
		if (isspace(rpn[i])) {
			i++;
		} else if (isdigit(rpn[i])) {
			int x;
			sscanf(&rpn[i], "%d ", &x);
			stack_push(&operands, x);
			while (isdigit(rpn[i]))
				i++;
		} else {
			int x, y;
			if (stack_count(&operands) < 2) {
				fprintf(stderr, "Not enough arguments\n");
				return -1;
			}
			y = stack_pop(&operands);
			x = stack_pop(&operands);
			switch (rpn[i]) {
			case '+':
				stack_push(&operands, x + y);
				break;
			case '*':
				stack_push(&operands, x * y);
				break;
			case '-':
				stack_push(&operands, x - y);
				break;
			case '/':
				if (!y) {
					fprintf(stderr, "Division by zero\n");
					return -2;
				}
				stack_push(&operands, x / y);
				break;
			}
			i += 2;
		}
	}
	*result = stack_pop(&operands);
	return 0;
}

int expr_eval(const char *expr, int *result)
{
	char rpn[BUFSIZ];

	if (!expr[0])
		return 0;
	infix_to_rpn(expr, rpn);
	return rpn_eval(rpn, result);
}
