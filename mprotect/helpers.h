#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include <assert.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/mman.h>

#define  u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define SYS_pkey_mprotect 386
#define SYS_pkey_alloc   384
#define SYS_pkey_free    385
#define si_pkey_offset  0x20
#define REG_IP_IDX PT_NIP
#define REG_TRAPNO PT_TRAP
#define REG_AMR         45
#define gregs gp_regs
#define fpregs fp_regs


#define NR_PKEYS		32
#define PKRU_BITS_PER_PKEY	2	
#define PKEY_DISABLE_ACCESS     0x3
#define PKEY_DISABLE_WRITE      0x2
#define PKEY_ACCESS_MASK	PKEY_DISABLE_ACCESS |  PKEY_DISABLE_WRITE

/* FIXME: 0xd is the SPRN for AMR register */
static inline u64 __rdpkru(void)
{
	u64 eax, pkru;

	asm volatile("mfspr %0, 0xd" : "=r" ((u64)(eax)));

	pkru = (u64)eax;
	return pkru;
}

static inline u64 __wrpkru(u64 pkru)
{
	u64 eax = pkru;

	asm volatile("mtspr 0xd, %0" : : "r" ((unsigned long)(eax)) : "memory");
	assert(pkru == __rdpkru());
}

static inline u32 pkey_to_shift(int pkey)
{
	return (NR_PKEYS - pkey - 1) * PKRU_BITS_PER_PKEY;
}

u64 reset_bits(int pkey, u64 bits)
{
	u32 shift = pkey_to_shift(pkey);

	return ~(bits << shift);
}

u64 left_shift_bits(int pkey, u64 bits)
{
	u32 shift = pkey_to_shift(pkey);

	return (bits << shift);
}

u64 right_shift_bits(int pkey, u64 bits)
{
	u32 shift = pkey_to_shift(pkey);

	return (bits >> shift);
}

u64 pkey_get(int pkey, unsigned long flags)
{
	u64 mask = PKEY_ACCESS_MASK;
	u64 pkru = __rdpkru();
	u64 shifted_pkru;
	u64 masked_pkru;

	shifted_pkru = right_shift_bits(pkey, pkru);
	masked_pkru = shifted_pkru & mask;
	return masked_pkru;
}

int pkey_set(int pkey, unsigned long rights, unsigned long flags)
{
	u64 mask = PKEY_ACCESS_MASK;
	u64 old_pkru = __rdpkru();
	u64 new_pkru;

	assert(!(rights & ~mask));

	new_pkru = old_pkru;
	new_pkru &= reset_bits(pkey, mask);
	new_pkru |= left_shift_bits(pkey, rights);
	__wrpkru(new_pkru);
	return 0;
}

void pkey_disable_set(int pkey, int flags)
{
	int ret;
	u64 pkey_rights = pkey_get(pkey, 0);
	u64 orig_pkru = __rdpkru();

	assert(flags & PKEY_ACCESS_MASK);
	assert(pkey_rights >= 0);

	if (flags && !(pkey_rights & flags)) {
		pkey_rights |= flags;
		ret = pkey_set(pkey, pkey_rights, 0);
		assert(ret >= 0);

		if (flags) {
			//printf("%s pkey %d orig %lx new %lx\n", __func__, pkey, orig_pkru, __rdpkru());
			assert(__rdpkru() > orig_pkru);
		}
	}
}

void pkey_disable_clear(int pkey, int flags)
{
	int ret;
	u64 pkey_rights = pkey_get(pkey, 0);
	u64 orig_pkru = __rdpkru();

	assert(flags & PKEY_ACCESS_MASK);
	assert(pkey_rights >= 0);

	if (flags && (pkey_rights & flags)) {
		pkey_rights &= ~flags;
		ret = pkey_set(pkey, pkey_rights, 0);
		assert(ret >= 0);

		if (flags) {
			//printf("%s pkey %d orig %lx new %lx\n", __func__, pkey, orig_pkru, __rdpkru());
			assert(__rdpkru() < orig_pkru);
		}
	}
}

void pkey_write_allow(int pkey)
{
	pkey_disable_clear(pkey, PKEY_DISABLE_WRITE);
}

void pkey_write_deny(int pkey)
{
	pkey_disable_set(pkey, PKEY_DISABLE_WRITE);
}

void pkey_access_allow(int pkey)
{
	pkey_disable_clear(pkey, PKEY_DISABLE_ACCESS);
}

void pkey_access_deny(int pkey)
{
	pkey_disable_set(pkey, PKEY_DISABLE_ACCESS);
}

int sys_pkey_alloc(unsigned long flags, unsigned long init_val)
{
        int ret = syscall(SYS_pkey_alloc, flags, init_val);

        return ret;
}

int sys_pkey_free(unsigned long pkey)
{
        int ret = syscall(SYS_pkey_free, pkey);

        return ret;
}

int sys_pkey_mprotect(void *ptr, size_t size, unsigned long prot, unsigned long pkey)
{
        int ret = syscall(SYS_pkey_mprotect, ptr, size, prot, pkey);

        return ret;
}

double time_ms(struct timeval begin, struct timeval end)
{
	double total_time = 0;

	total_time = (end.tv_sec - begin.tv_sec) * 1000;	/* In ms */
	total_time += (end.tv_usec - begin.tv_usec) / 1000;	/* In ms */
	return total_time;
}

double time_us(struct timeval begin, struct timeval end)
{
	double total_time = 0;

	total_time = (end.tv_sec - begin.tv_sec) * 1000000;	/* In ms */
	total_time += (end.tv_usec - begin.tv_usec);	/* In ms */
	return total_time;
}


