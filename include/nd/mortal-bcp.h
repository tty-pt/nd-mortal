#ifndef ND_MORTAL_BCP_H
#define ND_MORTAL_BCP_H

/*
 * mortal-bcp.h — client-side wire constants owned by nd-mortal.
 * Channel ND_BCP_HP is a (val,max) bar frame decoded into the generic gauge
 * named ND_MORTAL_HP_GAUGE.
 */

#include <nd/bcp-types.h>

#define ND_BCP_HP            10
#define ND_MORTAL_HP_GAUGE   "hp"

/* Channel 18: u32 target_ref, u16 val, u16 max -- the player's current combat
 * target (ref 0 clears it). Decoded into nd_client_state_t.target_ref and the
 * generic gauge ND_MORTAL_TARGET_GAUGE. */
#define ND_BCP_TARGET_HP         18
#define ND_MORTAL_TARGET_GAUGE   ND_CLIENT_TARGET_GAUGE

#include <stdint.h>
#include <string.h>
#include <nd/bcp-types.h>

/* Returns 1 on success, -1 on a short/invalid frame. `p`/`len` is the payload
 * after the 3-byte header. */
static inline int
nd_mortal_target_unpack(nd_client_state_t *st, const uint8_t *p, size_t len)
{
	uint32_t ref;
	uint16_t val, max;
	if (!st || !p || len < 8)
		return -1;
	memcpy(&ref, p, 4);
	memcpy(&val, p + 4, 2);
	memcpy(&max, p + 6, 2);
	st->target_ref = ref;
	nd_client_gauge_set(st, ND_MORTAL_TARGET_GAUGE, ref ? val : 0, ref ? max : 0);
	return 1;
}

#endif /* ND_MORTAL_BCP_H */
