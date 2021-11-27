#ifndef CEPH_CRUSH_HASH_H
#define CEPH_CRUSH_HASH_H

#ifdef __KERNEL__
# include <linux/types.h>
#else
# include "crush_compat.h"
#endif

#define CRUSH_HASH_RJENKINS1   0

#define CRUSH_HASH_DEFAULT CRUSH_HASH_RJENKINS1

#ifdef __aarch64__
#define CRUSH_NEON_NUM 4
extern void crush_hash32_3_neonx2(int type, __u32 a[CRUSH_NEON_NUM * 2],
		__u32 b[CRUSH_NEON_NUM * 2], __u32 c[CRUSH_NEON_NUM * 2], __u32 hash[CRUSH_NEON_NUM * 2]);
#endif

extern const char *crush_hash_name(int type);

extern __u32 crush_hash32(int type, __u32 a);
extern __u32 crush_hash32_2(int type, __u32 a, __u32 b);
extern __u32 crush_hash32_3(int type, __u32 a, __u32 b, __u32 c);
extern __u32 crush_hash32_4(int type, __u32 a, __u32 b, __u32 c, __u32 d);
extern __u32 crush_hash32_5(int type, __u32 a, __u32 b, __u32 c, __u32 d,
			    __u32 e);

#endif
