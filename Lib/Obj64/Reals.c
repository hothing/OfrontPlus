/*  Ofront 1.2 -xtspkae */
#include "SYSTEM.h"
#include "Platform.h"




export void Reals_Convert (REAL x, SHORTINT n, CHAR *d, LONGINT d__len);
export void Reals_ConvertH (REAL y, CHAR *d, LONGINT d__len);
export void Reals_ConvertHL (LONGREAL x, CHAR *d, LONGINT d__len);
export void Reals_ConvertL (LONGREAL x, SHORTINT n, CHAR *d, LONGINT d__len);
export SHORTINT Reals_Expo (REAL x);
export SHORTINT Reals_ExpoL (LONGREAL x);
export void Reals_SetExpo (SHORTINT e, REAL *x);
export void Reals_SetExpoL (SHORTINT e, LONGREAL *x);
export REAL Reals_Ten (SHORTINT e);
export LONGREAL Reals_TenL (SHORTINT e);
static void Reals_Unpack (BYTE *b, LONGINT b__len, BYTE *d, LONGINT d__len);

#include <stdlib.h>
#define Reals_Offset(adr, offset)	((Platform_MemAdr)((SYSTEM_ADR)adr + offset))
#define Reals_ecvt(x, ndigit, decpt, sign)	((Platform_MemAdr)ecvt (x, ndigit, (int*)(decpt), (int*)(sign)))

/*============================================================================*/

REAL Reals_Ten (SHORTINT e)
{
	LONGREAL r, power;
	r = (LONGREAL)1;
	power = (LONGREAL)10;
	while (e > 0) {
		if (__ODD(e)) {
			r = r * power;
		}
		power = power * power;
		e = __ASHR(e, 1, SHORTINT);
	}
	return r;
}

/*----------------------------------------------------------------------------*/
LONGREAL Reals_TenL (SHORTINT e)
{
	LONGREAL r, power;
	r = (LONGREAL)1;
	power = (LONGREAL)10;
	for (;;) {
		if (__ODD(e)) {
			r = r * power;
		}
		e = __ASHR(e, 1, SHORTINT);
		if (e <= 0) {
			return r;
		}
		power = power * power;
	}
	__RETCHK;
}

/*----------------------------------------------------------------------------*/
SHORTINT Reals_Expo (REAL x)
{
	return (INTEGER)__MASK(__ASHR(__VAL(INTEGER, x), 23, INTEGER), -256);
}

/*----------------------------------------------------------------------------*/
SHORTINT Reals_ExpoL (LONGREAL x)
{
	INTEGER h;
	__GET((LONGINT)&x + 4, h, INTEGER);
	return (INTEGER)__MASK(__ASHR(h, 20, INTEGER), -2048);
}

/*----------------------------------------------------------------------------*/
void Reals_SetExpo (SHORTINT e, REAL *x)
{
	*x = (REAL)(__VAL(SET, *x) & ~0x01fe | (SET)__ASHL((INTEGER)e, 23, INTEGER));
}

/*----------------------------------------------------------------------------*/
void Reals_SetExpoL (SHORTINT e, LONGREAL *x)
{
	SET h;
	__GET((LONGINT)x + 4, h, SET);
	h = h & ~0x0ffe | (SET)__ASHL((INTEGER)e, 20, INTEGER);
	__PUT((LONGINT)x + 4, h, SET);
}

/*----------------------------------------------------------------------------*/
void Reals_Convert (REAL x, SHORTINT n, CHAR *d, LONGINT d__len)
{
	INTEGER i, k;
	i = (INTEGER)__ENTIER(x);
	k = 0;
	while (k < (INTEGER)n) {
		d[__X(k, d__len)] = (CHAR)((INTEGER)__MOD(i, 10) + 48);
		i = __DIV(i, 10);
		k += 1;
	}
}

/*----------------------------------------------------------------------------*/
void Reals_ConvertL (LONGREAL x, SHORTINT n, CHAR *d, LONGINT d__len)
{
	INTEGER decpt, sign, i;
	Platform_MemAdr buf = NIL;
	buf = Reals_ecvt(x, n + 2, &decpt, &sign);
	i = 0;
	while (i < decpt) {
		__GET(Reals_Offset(buf, i), d[__X(((INTEGER)n - i) - 1, d__len)], CHAR);
		i += 1;
	}
	i = ((INTEGER)n - i) - 1;
	while (i >= 0) {
		d[__X(i, d__len)] = '0';
		i -= 1;
	}
}

/*----------------------------------------------------------------------------*/
static void Reals_Unpack (BYTE *b, LONGINT b__len, BYTE *d, LONGINT d__len)
{
	BYTE i, k;
	INTEGER len;
	i = 0;
	len = (INTEGER)b__len;
	while ((INTEGER)i < len) {
		k = __ASHR((INTEGER)(__VAL(CHAR, b[__X(i, b__len)])), 4, SHORTINT);
		if ((INTEGER)k > 9) {
			d[__X(__ASHL((INTEGER)i, 1, SHORTINT), d__len)] = (INTEGER)k + 55;
		} else {
			d[__X(__ASHL((INTEGER)i, 1, SHORTINT), d__len)] = (INTEGER)k + 48;
		}
		k = __MASK((INTEGER)(__VAL(CHAR, b[__X(i, b__len)])), -16);
		if ((INTEGER)k > 9) {
			d[__X(__ASHL((INTEGER)i, 1, SHORTINT) + 1, d__len)] = (INTEGER)k + 55;
		} else {
			d[__X(__ASHL((INTEGER)i, 1, SHORTINT) + 1, d__len)] = (INTEGER)k + 48;
		}
		i += 1;
	}
}

void Reals_ConvertH (REAL y, CHAR *d, LONGINT d__len)
{
	Reals_Unpack((void*)&y, 4, (void*)d, d__len * 1);
}

/*----------------------------------------------------------------------------*/
void Reals_ConvertHL (LONGREAL x, CHAR *d, LONGINT d__len)
{
	Reals_Unpack((void*)&x, 8, (void*)d, d__len * 1);
}

/*----------------------------------------------------------------------------*/

export void *Reals__init(void)
{
	__DEFMOD;
	__IMPORT(Platform__init);
	__REGMOD("Reals", 0);
/* BEGIN */
	__ENDMOD;
}