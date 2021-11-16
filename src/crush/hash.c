#ifdef __KERNEL__
# include <linux/crush/hash.h>
#else
# include "hash.h"
#endif

/*
 * Robert Jenkins' function for mixing 32-bit values
 * http://burtleburtle.net/bob/hash/evahash.html
 * a, b = random bits, c = input and output
 */
#define crush_hashmix(a, b, c) do {			\
		a = a-b;  a = a-c;  a = a^(c>>13);	\
		b = b-c;  b = b-a;  b = b^(a<<8);	\
		c = c-a;  c = c-b;  c = c^(b>>13);	\
		a = a-b;  a = a-c;  a = a^(c>>12);	\
		b = b-c;  b = b-a;  b = b^(a<<16);	\
		c = c-a;  c = c-b;  c = c^(b>>5);	\
		a = a-b;  a = a-c;  a = a^(c>>3);	\
		b = b-c;  b = b-a;  b = b^(a<<10);	\
		c = c-a;  c = c-b;  c = c^(b>>15);	\
	} while (0)

#define crush_hashmix_simdx2(a, b, c, a1, b1, c1) {\
		a = vsubq_u32(a, b); a1 = vsubq_u32(a1, b1); a = vsubq_u32(a, c); a1 = vsubq_u32(a1, c1);\
			a = veorq_u32(a, vshrq_n_u32(c, 13)); a1 = veorq_u32(a1, vshrq_n_u32(c1, 13));\
		b = vsubq_u32(b, c); b1 = vsubq_u32(b1, c1); b = vsubq_u32(b, a); b1 = vsubq_u32(b1, a1);\
			b = veorq_u32(b, vshlq_n_u32(a, 8)); b1 = veorq_u32(b1, vshlq_n_u32(a1, 8));\
		c = vsubq_u32(c, a); c1 = vsubq_u32(c1, a1); c = vsubq_u32(c, b); c1 = vsubq_u32(c1, b1);\
			c = veorq_u32(c, vshrq_n_u32(b, 13)); c1 = veorq_u32(c1, vshrq_n_u32(b1, 13));\
		a = vsubq_u32(a, b); a1 = vsubq_u32(a1, b1); a = vsubq_u32(a, c); a1 = vsubq_u32(a1, c1);\
			a = veorq_u32(a, vshrq_n_u32(c, 12)); a1 = veorq_u32(a1, vshrq_n_u32(c1, 12));\
		b = vsubq_u32(b, c); b1 = vsubq_u32(b1, c1); b = vsubq_u32(b, a); b1 = vsubq_u32(b1, a1);\
			b = veorq_u32(b, vshlq_n_u32(a, 16)); b1 = veorq_u32(b1, vshlq_n_u32(a1, 16));\
		c = vsubq_u32(c, a); c1 = vsubq_u32(c1, a1); c = vsubq_u32(c, b); c1 = vsubq_u32(c1, b1);\
			c = veorq_u32(c, vshrq_n_u32(b, 5)); c1 = veorq_u32(c1, vshrq_n_u32(b1, 5));\
		a = vsubq_u32(a, b); a1 = vsubq_u32(a1, b1); a = vsubq_u32(a, c); a1 = vsubq_u32(a1, c1);\
			a = veorq_u32(a, vshrq_n_u32(c, 3)); a1 = veorq_u32(a1, vshrq_n_u32(c1, 3));\
		b = vsubq_u32(b, c); b1 = vsubq_u32(b1, c1); b = vsubq_u32(b, a); b1 = vsubq_u32(b1, a1);\
			b = veorq_u32(b, vshlq_n_u32(a, 10)); b1 = veorq_u32(b1, vshlq_n_u32(a1, 10));\
		c = vsubq_u32(c, a); c1 = vsubq_u32(c1, a1); c = vsubq_u32(c, b); c1 = vsubq_u32(c1, b1);\
			c = veorq_u32(c, vshrq_n_u32(b, 15)); c1 = veorq_u32(c1, vshrq_n_u32(b1, 15));\
	}while (0);

#define crush_hash_seed 1315423911

static __u32 crush_hash32_rjenkins1(__u32 a)
{
	__u32 hash = crush_hash_seed ^ a;
	__u32 b = a;
	__u32 x = 231232;
	__u32 y = 1232;
	crush_hashmix(b, x, hash);
	crush_hashmix(y, a, hash);
	return hash;
}

static __u32 crush_hash32_rjenkins1_2(__u32 a, __u32 b)
{
	__u32 hash = crush_hash_seed ^ a ^ b;
	__u32 x = 231232;
	__u32 y = 1232;
	crush_hashmix(a, b, hash);
	crush_hashmix(x, a, hash);
	crush_hashmix(b, y, hash);
	return hash;
}

static __u32 crush_hash32_rjenkins1_3(__u32 a, __u32 b, __u32 c)
{
	__u32 hash = crush_hash_seed ^ a ^ b ^ c;
	__u32 x = 231232;
	__u32 y = 1232;
	crush_hashmix(a, b, hash);
	crush_hashmix(c, x, hash);
	crush_hashmix(y, a, hash);
	crush_hashmix(b, x, hash);
	crush_hashmix(y, c, hash);
	return hash;
}

static __u32 crush_hash32_rjenkins1_4(__u32 a, __u32 b, __u32 c, __u32 d)
{
	__u32 hash = crush_hash_seed ^ a ^ b ^ c ^ d;
	__u32 x = 231232;
	__u32 y = 1232;
	crush_hashmix(a, b, hash);
	crush_hashmix(c, d, hash);
	crush_hashmix(a, x, hash);
	crush_hashmix(y, b, hash);
	crush_hashmix(c, x, hash);
	crush_hashmix(y, d, hash);
	return hash;
}

static __u32 crush_hash32_rjenkins1_5(__u32 a, __u32 b, __u32 c, __u32 d,
				      __u32 e)
{
	__u32 hash = crush_hash_seed ^ a ^ b ^ c ^ d ^ e;
	__u32 x = 231232;
	__u32 y = 1232;
	crush_hashmix(a, b, hash);
	crush_hashmix(c, d, hash);
	crush_hashmix(e, x, hash);
	crush_hashmix(y, a, hash);
	crush_hashmix(b, x, hash);
	crush_hashmix(y, c, hash);
	crush_hashmix(d, x, hash);
	crush_hashmix(y, e, hash);
	return hash;
}


__u32 crush_hash32(int type, __u32 a)
{
	switch (type) {
	case CRUSH_HASH_RJENKINS1:
		return crush_hash32_rjenkins1(a);
	default:
		return 0;
	}
}

__u32 crush_hash32_2(int type, __u32 a, __u32 b)
{
	switch (type) {
	case CRUSH_HASH_RJENKINS1:
		return crush_hash32_rjenkins1_2(a, b);
	default:
		return 0;
	}
}

__u32 crush_hash32_3(int type, __u32 a, __u32 b, __u32 c)
{
	switch (type) {
	case CRUSH_HASH_RJENKINS1:
		return crush_hash32_rjenkins1_3(a, b, c);
	default:
		return 0;
	}
}

__u32 crush_hash32_4(int type, __u32 a, __u32 b, __u32 c, __u32 d)
{
	switch (type) {
	case CRUSH_HASH_RJENKINS1:
		return crush_hash32_rjenkins1_4(a, b, c, d);
	default:
		return 0;
	}
}

__u32 crush_hash32_5(int type, __u32 a, __u32 b, __u32 c, __u32 d, __u32 e)
{
	switch (type) {
	case CRUSH_HASH_RJENKINS1:
		return crush_hash32_rjenkins1_5(a, b, c, d, e);
	default:
		return 0;
	}
}

const char *crush_hash_name(int type)
{
	switch (type) {
	case CRUSH_HASH_RJENKINS1:
		return "rjenkins1";
	default:
		return "unknown";
	}
}
