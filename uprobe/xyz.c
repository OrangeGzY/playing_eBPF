/*
 * Compile with:
 * 	gcc -o xyz -Wall -g -O0 xyz.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

struct string {
	size_t len;
	char  *buf;
};

struct load {
	unsigned long val;
	struct string str;
};

void do_work(struct load *l)
{
	if (!l || !l->str.len || !l->str.buf)
		return;
	printf("address of l = %016lx\n", (unsigned long) l);
	printf("address of l->str = %016lx\n", (unsigned long) &(l->str));
	printf("address of l->str.buf = %016lx\n", (unsigned long) (l->str).buf);
	printf("val = %lu, len = %lu, buf = %s\n",
	       l->val, l->str.len, l->str.buf);
}

int main(int argc, char **argv)
{
	struct load l = {
		1000, { 12, "Hello world!" }
	};

	do_work(&l);

	return EXIT_SUCCESS;
}
