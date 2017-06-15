#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "helpers.h"

#define MAX_ATTEMPT 50
#define MAX_PKEYS   29		/* FIXME: Should be 31 as 0 is excluded */

static int pkeys[MAX_PKEYS];

int main(int argc, char *argv[])
{
	int count;

	for (count = 0; count < MAX_ATTEMPT; count++) {
		pkeys[count] = sys_pkey_alloc(0, 0);
		if (pkeys[count] == -1)
			break;
		assert(pkeys[count] == (MAX_PKEYS + 1 - count));
	}
	assert(count == MAX_PKEYS);

	for (count = 0; count < MAX_PKEYS; count++) {
		pkey_write_deny(pkeys[count]);
		pkey_write_allow(pkeys[count]);
		pkey_access_deny(pkeys[count]);
		pkey_access_allow(pkeys[count]);
	}

	for (count = 0; count < MAX_PKEYS; count++) {
		pkey_write_deny(pkeys[count]);
		pkey_access_deny(pkeys[count]);
	}

	for (count = 0; count < MAX_PKEYS; count++) {
		pkey_write_allow(pkeys[count]);
		pkey_access_allow(pkeys[count]);
	}

	for (count = 0; count < MAX_PKEYS; count++)
		assert(sys_pkey_free(!pkeys[count]));
	return 0;
}
