#include <nd/nd.h>
#include <stdio.h>
#include "./include/uapi/mortal.h"

unsigned mortal_hd, bcp_hp;

unsigned huth_y[2] = {
	DAYTICK_Y + 2,
	DAYTICK_Y + 3
};

/* SIC */

SIC_DEF(int, on_mortal_life, unsigned, player_ref, double, dt)
SIC_DEF(int, on_mortal_survival, unsigned, player_ref, double, dt)

SIC_DEF(int, on_birth, unsigned, ref, uint64_t, v)
SIC_DEF(int, on_death, unsigned, ref)

SIC_DEF(int, on_murder, unsigned, killer_ref, unsigned, ref)

/* API */

SIC_DEF(int, mortal_damage, unsigned, killer_ref, unsigned, victim_ref, short, amt)
SIC_DEF(int, mcp_hp, unsigned, player_ref)

int mcp_hp(unsigned ref) {
	mortal_t mortal;
	nd_get(mortal_hd, &mortal, &ref);
	mcp_bar(bcp_hp, ref, mortal.hp, HP_MAX(&mortal));
	return 0;
}

static inline int
huth_notify(unsigned player_ref, mortal_t *mortal, enum huth type)
{
	static char const *msg[] = {
		"You are thirsty.\n",
		"You are very thirsty.\n",
		"you are dehydrated.\n",
		"You are dying of thirst.\n",
		"You are hungry.\n",
		"You are very hungry.\n",
		"You are starving.\n",
		"You are starving to death.\n"
	};

	unsigned thirst_y = huth_y[HUTH_THIRST],
		hunger_y = huth_y[HUTH_HUNGER];

	unsigned long long const on[] = {
		1ULL << (thirst_y - 1),
		1ULL << (thirst_y - 2),
		1ULL << (thirst_y - 3),
		1ULL << (hunger_y - 1),
		1ULL << (hunger_y - 2),
		1ULL << (hunger_y - 3)
	};

	unsigned long long const *n = on + 3 * type;
	unsigned long long v;
	unsigned char rn = mortal->huth_n[type];

	v = mortal->huth[type] += 1ULL << (DAYTICK_Y - 10);

	char const **m = msg + 4 * type;

	if (rn >= 4) {
		return HP_MAX(mortal) >> 3;
	} else if (rn >= 3) {
		if (v >= 2 * n[1] + n[2]) {
			nd_writef(player_ref, m[3]);
			rn += 1;
		}
	} else if (rn >= 2) {
		if (v >= 2 * n[1]) {
			nd_writef(player_ref, m[2]);
			rn += 1;
		}
	} else if (rn >= 1) {
		if (v >= n[1]) {
			nd_writef(player_ref, m[1]);
			rn += 1;
		}
	} else if (v >= n[2]) {
		nd_writef(player_ref, m[0]);
		rn += 1;
	}

	mortal->huth_n[type] = rn;
	return 0;
}

static inline unsigned
mortal_body(unsigned ref)
{
	unsigned tmp_ref;
	char buf[32];
	OBJ mob, dead_mob;
	nd_get(HD_OBJ, &mob, &ref);
	snprintf(buf, sizeof(buf), "%s's body.", mob.name);
	unsigned body_ref = object_add(&dead_mob, mob.skid, mob.location, 0);
	unsigned n = 0;

	nd_cur_t c = nd_iter(HD_CONTENTS, &ref);
	while (nd_next(&ref, &tmp_ref, &c)) {
		object_move(tmp_ref, body_ref);
		n++;
	}

	if (n > 0) {
		strlcpy(dead_mob.name, buf, sizeof(dead_mob.name));
		nd_put(HD_OBJ, &body_ref, &dead_mob);
		nd_owritef(ref, "%s's body drops to the ground.\n", mob.name);
		return body_ref;
	} else {
		object_move(body_ref, NOTHING);
		return NOTHING;
	}
}

static inline unsigned
mortal_murder(unsigned killer_ref, unsigned ref)
{
	OBJ victim;
	ENT ent;
	OBJ loc;
	mortal_t mortal;
	unsigned loc_ref;

	notify_wts(ref, "die", "dies", "");

	mortal_body(ref);

	SIC_CALL(NULL, on_murder, killer_ref, ref);

	nd_get(HD_OBJ, &victim, &ref);
	loc_ref = victim.location;

	nd_get(HD_OBJ, &loc, &loc_ref);

	ent = ent_get(ref);
	mortal.hp = 1;
	nd_put(mortal_hd, &ref, &mortal);
	SIC_CALL(NULL, on_death, ref);
	look_around(ref);
	mcp_hp(ref);
	/* nd_flush(ref); */

	return ref;
}

int
mortal_damage(unsigned attacker_ref, unsigned ref, short amt)
{
	mortal_t mortal;
	long hp;

	nd_get(mortal_hd, &mortal, &ref);
	hp = mortal.hp;
	hp += amt;

	if (!amt)
		return 0;

	if (hp <= 0) {
		mortal_murder(attacker_ref, ref);
		return 1;
	}

	register unsigned short max = HP_MAX(&mortal);
	if (hp > max)
		hp = max;

	mortal.hp = hp;
	nd_put(mortal_hd, &ref, &mortal);
	mcp_hp(ref);

	return 0;
}

void
mortal_update(unsigned ref, double dt)
{
	OBJ player;
	nd_get(HD_OBJ, &player, &ref);
	ENT ent = ent_get(ref);
	mortal_t mortal;
	unsigned short ohp = mortal.hp;

	nd_get(mortal_hd, &mortal, &ref);

	int damage = 0;

	damage -= huth_notify(ref, &mortal, HUTH_THIRST);
	damage -= huth_notify(ref, &mortal, HUTH_HUNGER);

	mortal.damage = damage;
	nd_put(mortal_hd, &ref, &mortal);
	SIC_CALL(NULL, on_mortal_life, ref, dt);
	nd_get(mortal_hd, &mortal, &ref);

	if (mortal_damage(NOTHING, ref, mortal.damage))
		return;

	SIC_CALL(NULL, on_mortal_survival, ref, dt);

	nd_get(mortal_hd, &mortal, &ref);

	if (mortal.hp == ohp)
		return;

	if (mortal.hp)
		mcp_hp(ref);
}

int on_update(unsigned ref, unsigned type, double dt) {
	if (type != TYPE_ENTITY)
		return 1;
	mortal_update(ref, dt);
	return 0;
}

int on_add(unsigned ref, unsigned type, uint64_t v) {
	mortal_t mortal;

	if (type != TYPE_ENTITY)
		return 0;

	memset(&mortal, 0, sizeof(mortal));
	nd_put(mortal_hd, &ref, &mortal);
	SIC_CALL(NULL, on_birth, ref, v);
	return 0;
}

int
on_status(unsigned player_ref)
{
	mortal_t mortal;
	nd_get(mortal_hd, &mortal, &player_ref);
	nd_writef(player_ref, "Mortal:\t"
			"hp %u/%u\tstuck 0x%x\t"
			"hunger %u\tthirst %u\n",
			mortal.hp, HP_MAX(&mortal),
			mortal.huth[HUTH_HUNGER],
			mortal.huth[HUTH_THIRST]);
	return 0;
}

int on_examine(unsigned player_ref, unsigned ref, unsigned type) {
	mortal_t target;

	if (type != TYPE_ENTITY)
		return 1;

	nd_get(mortal_hd, &target, &ref);
	nd_writef(player_ref, "Hp: %d/%d\n", target.hp, HP_MAX(&target));
	return 0;
}

void
mod_open(void *arg __attribute__((unused))) {
	nd_len_reg("mortal", sizeof(mortal_t));
	mortal_hd = nd_open("mortal", "u", "mortal", 0);
	bcp_hp = nd_put(HD_BCP, NULL, "hp");

	SIC_AREG(on_mortal_life);
	SIC_AREG(on_mortal_survival);

	SIC_AREG(on_birth);
	SIC_AREG(on_death);

	SIC_AREG(on_murder);

	/* API */
	
	SIC_AREG(mortal_damage);
	SIC_AREG(mcp_hp);
}

void
mod_install(void *arg) {
	mod_open(arg);
}
