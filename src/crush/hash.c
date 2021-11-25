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

#ifdef __aarch64__
#define crush_hashmix_neonx2(a, b, c, a1, b1, c1) {\
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
#endif

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

#ifdef __aarch64__
void crush_hash32_3_neonx2(int type, __u32 a[CRUSH_NEON_NUM * 2],
					__u32 b[CRUSH_NEON_NUM * 2], __u32 c[CRUSH_NEON_NUM * 2], __u32 hash[CRUSH_NEON_NUM * 2])
{
	uint32x4_t a1;
	uint32x4_t a2;
	uint32x4_t b1;
	uint32x4_t b2;
	uint32x4_t c1;
	uint32x4_t c2;
	uint32x4_t hash1;
	uint32x4_t hash2;
	uint32x4_t x1;
	uint32x4_t x2;
	uint32x4_t y1;
	uint32x4_t y2;
	uint32x4_t tmp;

	switch (type) {
	case CRUSH_HASH_RJENKINS1:
		a1 = vld1q_u32(&a[0]);
		a2 = vld1q_u32(&a[CRUSH_NEON_NUM]);
		b1 = vld1q_u32(&b[0]);
		b2 = vld1q_u32(&b[CRUSH_NEON_NUM]);
		c1 = vld1q_u32(&c[0]);
		c2 = vld1q_u32(&c[CRUSH_NEON_NUM]);

		hash1 = vdupq_n_u32(crush_hash_seed);
		x1 = vdupq_n_u32(231232);
		y1 = vdupq_n_u32(1232);
		hash2 = vdupq_n_u32(crush_hash_seed);
		x2 = vdupq_n_u32(231232);
		y2 = vdupq_n_u32(1232);

		hash1 = veorq_u32(hash1, a1);
		tmp = veorq_u32(b1, c1);
		hash1 = veorq_u32(hash1, tmp);
		hash2 = veorq_u32(hash2, a2);
		tmp = veorq_u32(b2, c2);
		hash2 = veorq_u32(hash2, tmp);

		crush_hashmix_neonx2(a1, b1, hash1, a2, b2, hash2);
		crush_hashmix_neonx2(c1, x1, hash1, c2, x2, hash2);
		crush_hashmix_neonx2(y1, a1, hash1, y2, a2, hash2);
		crush_hashmix_neonx2(b1, x1, hash1, b2, x2, hash2);
		crush_hashmix_neonx2(y1, c1, hash1, y2, c2, hash2);

		vst1q_u32(&hash[0], hash1);
		vst1q_u32(&hash[CRUSH_NEON_NUM], hash2);
	default:
		return;
	}
}
#endif

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
