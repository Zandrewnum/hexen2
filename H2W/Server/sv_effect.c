
//**************************************************************************
//**
//** sv_effect.c
//**
//** Client side effects.
//**
//** $Header: /HexenWorld/Server/sv_effect.c 39    5/25/98 1:30p Mgummelt $
//**
//**************************************************************************

// HEADER FILES ------------------------------------------------------------

#include "qwsvdef.h"

// MACROS ------------------------------------------------------------------

// TYPES -------------------------------------------------------------------

// EXTERNAL FUNCTION PROTOTYPES --------------------------------------------

// PUBLIC FUNCTION PROTOTYPES ----------------------------------------------

// PRIVATE FUNCTION PROTOTYPES ---------------------------------------------

static int NewEffectEntity(void);
static void FreeEffectEntity(int index);

// EXTERNAL DATA DECLARATIONS ----------------------------------------------

extern cvar_t sv_ce_scale;
extern cvar_t sv_ce_max_size;

// PUBLIC DATA DEFINITIONS -------------------------------------------------

// PRIVATE DATA DEFINITIONS ------------------------------------------------

// CODE --------------------------------------------------------------------


void SV_ClearEffects(void)
{
	memset(sv.Effects,0,sizeof(sv.Effects));
}

// All changes need to be in SV_SendEffect(), SV_ParseEffect(),
// SV_SaveEffects(), SV_LoadEffects(), CL_ParseEffect()
void SV_SendEffect(sizebuf_t *sb, int index)
{
	qboolean	DoTest;
	vec3_t		TestO;
	int			TestDistance;
	int			i;

	if (sv_ce_scale.value > 0)
		DoTest = true;
	else
		DoTest = false;

	VectorCopy(vec3_origin, TestO);

	switch(sv.Effects[index].type)
	{
		case CE_HWSHEEPINATOR:
		case CE_HWXBOWSHOOT:
			VectorCopy(sv.Effects[index].Xbow.origin[5], TestO);
			TestDistance = 900;
			break;
		case CE_SCARABCHAIN:
			VectorCopy(sv.Effects[index].Chain.origin, TestO);
			TestDistance = 900;
			break;

		case CE_TRIPMINE:
			VectorCopy(sv.Effects[index].Chain.origin, TestO);
//			DoTest = false;
			break;

			//ACHTUNG!!!!!!! setting DoTest to false here does not insure that effect will be sent to everyone!
		case CE_TRIPMINESTILL:
			TestDistance = 10000;
			DoTest = false;
			break;

		case CE_RAIN:
			TestDistance = 10000;
			DoTest = false;
			break;

		case CE_FOUNTAIN:
			TestDistance = 10000;
			DoTest = false;
			break;

		case CE_QUAKE:
			VectorCopy(sv.Effects[index].Quake.origin, TestO);
			TestDistance = 700;
			break;

		case CE_WHITE_SMOKE:
		case CE_GREEN_SMOKE:
		case CE_GREY_SMOKE:
		case CE_RED_SMOKE:
		case CE_SLOW_WHITE_SMOKE:
		case CE_TELESMK1:
		case CE_TELESMK2:
		case CE_GHOST:
		case CE_REDCLOUD:
		case CE_FLAMESTREAM:
		case CE_ACID_MUZZFL:
		case CE_FLAMEWALL:
		case CE_FLAMEWALL2:
		case CE_ONFIRE:
		case CE_RIPPLE:
			VectorCopy(sv.Effects[index].Smoke.origin, TestO);
			TestDistance = 250;
			break;

		case CE_SM_WHITE_FLASH:
		case CE_YELLOWRED_FLASH:
		case CE_BLUESPARK:
		case CE_YELLOWSPARK:
		case CE_SM_CIRCLE_EXP:
		case CE_BG_CIRCLE_EXP:
		case CE_SM_EXPLOSION:
		case CE_SM_EXPLOSION2:
		case CE_BG_EXPLOSION:
		case CE_FLOOR_EXPLOSION:
		case CE_BLUE_EXPLOSION:
		case CE_REDSPARK:
		case CE_GREENSPARK:
		case CE_ICEHIT:
		case CE_MEDUSA_HIT:
		case CE_MEZZO_REFLECT:
		case CE_FLOOR_EXPLOSION2:
		case CE_XBOW_EXPLOSION:
		case CE_NEW_EXPLOSION:
		case CE_MAGIC_MISSILE_EXPLOSION:
		case CE_BONE_EXPLOSION:
		case CE_BLDRN_EXPL:
		case CE_ACID_HIT:
		case CE_LBALL_EXPL:
		case CE_FIREWALL_SMALL:
		case CE_FIREWALL_MEDIUM:
		case CE_FIREWALL_LARGE:
		case CE_ACID_SPLAT:
		case CE_ACID_EXPL:
		case CE_FBOOM:
		case CE_BRN_BOUNCE:
		case CE_LSHOCK:
		case CE_BOMB:
		case CE_FLOOR_EXPLOSION3:
			VectorCopy(sv.Effects[index].Smoke.origin, TestO);
			TestDistance = 250;
			break;

		case CE_WHITE_FLASH:
		case CE_BLUE_FLASH:
		case CE_SM_BLUE_FLASH:
		case CE_HWSPLITFLASH:
		case CE_RED_FLASH:
			VectorCopy(sv.Effects[index].Smoke.origin, TestO);
			TestDistance = 250;
			break;


		case CE_RIDER_DEATH:
			DoTest = false;
			break;

		case CE_TELEPORTERPUFFS:
			VectorCopy(sv.Effects[index].Teleporter.origin, TestO);
			TestDistance = 350;
			break;

		case CE_TELEPORTERBODY:
			VectorCopy(sv.Effects[index].Teleporter.origin, TestO);
			TestDistance = 350;
			break;

		case CE_DEATHBUBBLES:
			if (sv.Effects[index].Bubble.owner < 0 || sv.Effects[index].Bubble.owner >= sv.num_edicts)
			{
				return;
			}
			VectorCopy(PROG_TO_EDICT(sv.Effects[index].Bubble.owner)->v.origin, TestO);
			TestDistance = 400;
			break;

		case CE_HWDRILLA:
		case CE_BONESHARD:
		case CE_BONESHRAPNEL:
		case CE_HWBONEBALL:
		case CE_HWRAVENSTAFF:
		case CE_HWRAVENPOWER:

			VectorCopy(sv.Effects[index].Missile.origin, TestO);
			TestDistance = 900;
			break;

		case CE_HWMISSILESTAR:
		case CE_HWEIDOLONSTAR:
			VectorCopy(sv.Effects[index].Missile.origin, TestO);
			TestDistance = 600;
			break;
		default:
//			Sys_Error ("SV_SendEffect: bad type");
			PR_RunError ("SV_SendEffect: bad type");
			break;
	}

	MSG_WriteByte (&sv.multicast, svc_start_effect);
	MSG_WriteByte (&sv.multicast, index);
	MSG_WriteByte (&sv.multicast, sv.Effects[index].type);

	switch(sv.Effects[index].type)
	{
		case CE_RAIN:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Rain.min_org[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Rain.min_org[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Rain.min_org[2]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Rain.max_org[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Rain.max_org[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Rain.max_org[2]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Rain.e_size[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Rain.e_size[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Rain.e_size[2]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Rain.dir[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Rain.dir[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Rain.dir[2]);
			MSG_WriteShort(&sv.multicast, sv.Effects[index].Rain.color);
			MSG_WriteShort(&sv.multicast, sv.Effects[index].Rain.count);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Rain.wait);
			break;

		case CE_FOUNTAIN:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Fountain.pos[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Fountain.pos[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Fountain.pos[2]);
			MSG_WriteAngle(&sv.multicast, sv.Effects[index].Fountain.angle[0]);
			MSG_WriteAngle(&sv.multicast, sv.Effects[index].Fountain.angle[1]);
			MSG_WriteAngle(&sv.multicast, sv.Effects[index].Fountain.angle[2]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Fountain.movedir[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Fountain.movedir[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Fountain.movedir[2]);
			MSG_WriteShort(&sv.multicast, sv.Effects[index].Fountain.color);
			MSG_WriteByte(&sv.multicast, sv.Effects[index].Fountain.cnt);
			break;

		case CE_QUAKE:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Quake.origin[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Quake.origin[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Quake.origin[2]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Quake.radius);
			break;

		case CE_WHITE_SMOKE:
		case CE_GREEN_SMOKE:
		case CE_GREY_SMOKE:
		case CE_RED_SMOKE:
		case CE_SLOW_WHITE_SMOKE:
		case CE_TELESMK1:
		case CE_TELESMK2:
		case CE_GHOST:
		case CE_REDCLOUD:
		case CE_FLAMESTREAM:
		case CE_ACID_MUZZFL:
		case CE_FLAMEWALL:
		case CE_FLAMEWALL2:
		case CE_ONFIRE:
		case CE_RIPPLE:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Smoke.origin[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Smoke.origin[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Smoke.origin[2]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Smoke.velocity[0]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Smoke.velocity[1]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Smoke.velocity[2]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Smoke.framelength);
			break;

		case CE_SM_WHITE_FLASH:
		case CE_YELLOWRED_FLASH:
		case CE_BLUESPARK:
		case CE_YELLOWSPARK:
		case CE_SM_CIRCLE_EXP:
		case CE_BG_CIRCLE_EXP:
		case CE_SM_EXPLOSION:
		case CE_SM_EXPLOSION2:
		case CE_BG_EXPLOSION:
		case CE_FLOOR_EXPLOSION:
		case CE_BLUE_EXPLOSION:
		case CE_REDSPARK:
		case CE_GREENSPARK:
		case CE_ICEHIT:
		case CE_MEDUSA_HIT:
		case CE_MEZZO_REFLECT:
		case CE_FLOOR_EXPLOSION2:
		case CE_XBOW_EXPLOSION:
		case CE_NEW_EXPLOSION:
		case CE_MAGIC_MISSILE_EXPLOSION:
		case CE_BONE_EXPLOSION:
		case CE_BLDRN_EXPL:
		case CE_ACID_HIT:
		case CE_ACID_SPLAT:
		case CE_ACID_EXPL:
		case CE_LBALL_EXPL:	
		case CE_FIREWALL_SMALL:
		case CE_FIREWALL_MEDIUM:
		case CE_FIREWALL_LARGE:
		case CE_FBOOM:
		case CE_BOMB:
		case CE_BRN_BOUNCE:
		case CE_LSHOCK:

			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Smoke.origin[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Smoke.origin[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Smoke.origin[2]);
			break;

		case CE_WHITE_FLASH:
		case CE_BLUE_FLASH:
		case CE_SM_BLUE_FLASH:			
		case CE_HWSPLITFLASH:
		case CE_RED_FLASH:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Smoke.origin[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Smoke.origin[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Smoke.origin[2]);
			break;


		case CE_RIDER_DEATH:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].RD.origin[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].RD.origin[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].RD.origin[2]);
			break;

		case CE_TELEPORTERPUFFS:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Teleporter.origin[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Teleporter.origin[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Teleporter.origin[2]);
			break;

		case CE_TELEPORTERBODY:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Teleporter.origin[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Teleporter.origin[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Teleporter.origin[2]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Teleporter.velocity[0][0]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Teleporter.velocity[0][1]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Teleporter.velocity[0][2]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Teleporter.skinnum);
			break;
		case CE_BONESHRAPNEL:
		case CE_HWBONEBALL:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Missile.origin[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Missile.origin[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Missile.origin[2]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Missile.velocity[0]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Missile.velocity[1]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Missile.velocity[2]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Missile.angle[0]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Missile.angle[1]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Missile.angle[2]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Missile.avelocity[0]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Missile.avelocity[1]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Missile.avelocity[2]);

			break;
		case CE_BONESHARD:
		case CE_HWRAVENSTAFF:
		case CE_HWMISSILESTAR:
		case CE_HWEIDOLONSTAR:
		case CE_HWRAVENPOWER:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Missile.origin[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Missile.origin[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Missile.origin[2]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Missile.velocity[0]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Missile.velocity[1]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Missile.velocity[2]);
			break;
		case CE_HWDRILLA:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Missile.origin[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Missile.origin[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Missile.origin[2]);
			MSG_WriteAngle(&sv.multicast, sv.Effects[index].Missile.angle[0]);
			MSG_WriteAngle(&sv.multicast, sv.Effects[index].Missile.angle[1]);
			MSG_WriteShort(&sv.multicast, sv.Effects[index].Missile.speed);
			break;
		case CE_DEATHBUBBLES:
			MSG_WriteShort(&sv.multicast, sv.Effects[index].Bubble.owner);
			MSG_WriteByte(&sv.multicast, sv.Effects[index].Bubble.offset[0]);
			MSG_WriteByte(&sv.multicast, sv.Effects[index].Bubble.offset[1]);
			MSG_WriteByte(&sv.multicast, sv.Effects[index].Bubble.offset[2]);
			MSG_WriteByte(&sv.multicast, sv.Effects[index].Bubble.count);
			break;
		case CE_SCARABCHAIN:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Chain.origin[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Chain.origin[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Chain.origin[2]);
			MSG_WriteShort(&sv.multicast, sv.Effects[index].Chain.owner+sv.Effects[index].Chain.material);
			MSG_WriteByte(&sv.multicast, sv.Effects[index].Chain.tag);
			break;
		case CE_TRIPMINESTILL:
		case CE_TRIPMINE:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Chain.origin[0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Chain.origin[1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Chain.origin[2]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Chain.velocity[0]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Chain.velocity[1]);
			MSG_WriteFloat(&sv.multicast, sv.Effects[index].Chain.velocity[2]);
			break;
		case CE_HWSHEEPINATOR:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Xbow.origin[5][0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Xbow.origin[5][1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Xbow.origin[5][2]);
			MSG_WriteAngle(&sv.multicast, sv.Effects[index].Xbow.angle[0]);
			MSG_WriteAngle(&sv.multicast, sv.Effects[index].Xbow.angle[1]);

			//now send the guys that have turned
			MSG_WriteByte(&sv.multicast, sv.Effects[index].Xbow.turnedbolts);
			MSG_WriteByte(&sv.multicast, sv.Effects[index].Xbow.activebolts);
			for (i=0;i<5;i++)
			{
				if ((1<<i)&sv.Effects[index].Xbow.turnedbolts)
				{
					MSG_WriteCoord(&sv.multicast, sv.Effects[index].Xbow.origin[i][0]);
					MSG_WriteCoord(&sv.multicast, sv.Effects[index].Xbow.origin[i][1]);
					MSG_WriteCoord(&sv.multicast, sv.Effects[index].Xbow.origin[i][2]);
					MSG_WriteAngle(&sv.multicast, sv.Effects[index].Xbow.vel[i][0]);
					MSG_WriteAngle(&sv.multicast, sv.Effects[index].Xbow.vel[i][1]);
				}
			}
			break;
		case CE_HWXBOWSHOOT:
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Xbow.origin[5][0]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Xbow.origin[5][1]);
			MSG_WriteCoord(&sv.multicast, sv.Effects[index].Xbow.origin[5][2]);
			MSG_WriteAngle(&sv.multicast, sv.Effects[index].Xbow.angle[0]);
			MSG_WriteAngle(&sv.multicast, sv.Effects[index].Xbow.angle[1]);
//				MSG_WriteFloat(&sv.multicast, sv.Effects[index].Xbow.angle[2]);
			MSG_WriteByte(&sv.multicast, sv.Effects[index].Xbow.bolts);
			MSG_WriteByte(&sv.multicast, sv.Effects[index].Xbow.randseed);

			//now send the guys that have turned
			MSG_WriteByte(&sv.multicast, sv.Effects[index].Xbow.turnedbolts);
			MSG_WriteByte(&sv.multicast, sv.Effects[index].Xbow.activebolts);
			for (i=0;i<5;i++)
			{
				if ((1<<i)&sv.Effects[index].Xbow.turnedbolts)
				{
					MSG_WriteCoord(&sv.multicast, sv.Effects[index].Xbow.origin[i][0]);
					MSG_WriteCoord(&sv.multicast, sv.Effects[index].Xbow.origin[i][1]);
					MSG_WriteCoord(&sv.multicast, sv.Effects[index].Xbow.origin[i][2]);
					MSG_WriteAngle(&sv.multicast, sv.Effects[index].Xbow.vel[i][0]);
					MSG_WriteAngle(&sv.multicast, sv.Effects[index].Xbow.vel[i][1]);
				}
			}
			break;
		default:
//			Sys_Error ("SV_SendEffect: bad type");
			PR_RunError ("SV_SendEffect: bad type");
			break;
	}

	if (sb)
	{
		SZ_Write (sb, sv.multicast.data, sv.multicast.cursize);
		SZ_Clear (&sv.multicast);
	}
	else
	{
		if (DoTest)
		{
			SV_Multicast (TestO, MULTICAST_PVS_R);
		}
		else
		{
			SV_Multicast (TestO, MULTICAST_ALL_R);
		}
		sv.Effects[index].client_list = clients_multicast;
	}
}

void SV_UpdateEffects(sizebuf_t *sb)
{
	int index;

	for(index=0;index<MAX_EFFECTS;index++)
		if (sv.Effects[index].type)
			SV_SendEffect(sb,index);
}

// All changes need to be in SV_SendEffect(), SV_ParseEffect(),
// SV_SaveEffects(), SV_LoadEffects(), CL_ParseEffect()
void SV_ParseEffect(sizebuf_t *sb)
{
	int index;
	byte effect;

	effect = G_FLOAT(OFS_PARM0);

	for(index=0;index<MAX_EFFECTS;index++)
		if (!sv.Effects[index].type || 
			(sv.Effects[index].expire_time && sv.Effects[index].expire_time <= sv.time)) 
			break;
		
	if (index >= MAX_EFFECTS)
	{
		PR_RunError ("MAX_EFFECTS reached");
		return;
	}

//	Con_Printf("Effect #%d\n",index);

	memset(&sv.Effects[index],0,sizeof(struct EffectT));

	sv.Effects[index].type = effect;
	G_FLOAT(OFS_RETURN) = index;

	switch(effect)
	{
		case CE_RAIN:
			VectorCopy(G_VECTOR(OFS_PARM1),sv.Effects[index].Rain.min_org);
			VectorCopy(G_VECTOR(OFS_PARM2),sv.Effects[index].Rain.max_org);
			VectorCopy(G_VECTOR(OFS_PARM3),sv.Effects[index].Rain.e_size);
			VectorCopy(G_VECTOR(OFS_PARM4),sv.Effects[index].Rain.dir);
			sv.Effects[index].Rain.color = G_FLOAT(OFS_PARM5);
			sv.Effects[index].Rain.count = G_FLOAT(OFS_PARM6);
			sv.Effects[index].Rain.wait = G_FLOAT(OFS_PARM7);

			sv.Effects[index].Rain.next_time = 0;
			break;

		case CE_FOUNTAIN:
			VectorCopy(G_VECTOR(OFS_PARM1),sv.Effects[index].Fountain.pos);
			VectorCopy(G_VECTOR(OFS_PARM2),sv.Effects[index].Fountain.angle);
			VectorCopy(G_VECTOR(OFS_PARM3),sv.Effects[index].Fountain.movedir);
			sv.Effects[index].Fountain.color = G_FLOAT(OFS_PARM4);
			sv.Effects[index].Fountain.cnt = G_FLOAT(OFS_PARM5);
			break;

		case CE_QUAKE:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Quake.origin);
			sv.Effects[index].Quake.radius = G_FLOAT(OFS_PARM2);
			break;

		case CE_WHITE_SMOKE:
		case CE_GREEN_SMOKE:
		case CE_GREY_SMOKE:
		case CE_RED_SMOKE:
		case CE_SLOW_WHITE_SMOKE:
		case CE_TELESMK1:
		case CE_TELESMK2:
		case CE_GHOST:
		case CE_REDCLOUD:
		case CE_RIPPLE:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Smoke.origin);
			VectorCopy(G_VECTOR(OFS_PARM2), sv.Effects[index].Smoke.velocity);
			sv.Effects[index].Smoke.framelength = G_FLOAT(OFS_PARM3);

			sv.Effects[index].expire_time = sv.time + 1;
			break;

		case CE_ACID_MUZZFL:
		case CE_FLAMESTREAM:
		case CE_FLAMEWALL:
		case CE_FLAMEWALL2:
		case CE_ONFIRE:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Smoke.origin);
			VectorCopy(G_VECTOR(OFS_PARM2), sv.Effects[index].Smoke.velocity);
			sv.Effects[index].Smoke.framelength = 0.05;
			sv.Effects[index].Smoke.frame = G_FLOAT(OFS_PARM3);
			sv.Effects[index].expire_time = sv.time + 1;
			break;

		case CE_SM_WHITE_FLASH:
		case CE_YELLOWRED_FLASH:
		case CE_BLUESPARK:
		case CE_YELLOWSPARK:
		case CE_SM_CIRCLE_EXP:
		case CE_BG_CIRCLE_EXP:
		case CE_SM_EXPLOSION:
		case CE_SM_EXPLOSION2:
		case CE_BG_EXPLOSION:
		case CE_FLOOR_EXPLOSION:
		case CE_BLUE_EXPLOSION:
		case CE_REDSPARK:
		case CE_GREENSPARK:
		case CE_ICEHIT:
		case CE_MEDUSA_HIT:
		case CE_MEZZO_REFLECT:
		case CE_FLOOR_EXPLOSION2:
		case CE_XBOW_EXPLOSION:
		case CE_NEW_EXPLOSION:
		case CE_MAGIC_MISSILE_EXPLOSION:
		case CE_BONE_EXPLOSION:
		case CE_BLDRN_EXPL:
		case CE_ACID_HIT:
		case CE_ACID_SPLAT:
		case CE_ACID_EXPL:
		case CE_LBALL_EXPL:
		case CE_FIREWALL_SMALL:
		case CE_FIREWALL_MEDIUM:
		case CE_FIREWALL_LARGE:
		case CE_FBOOM:
		case CE_BOMB:
		case CE_BRN_BOUNCE:
		case CE_LSHOCK:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Smoke.origin);
			sv.Effects[index].expire_time = sv.time + 1;
			break;

		case CE_WHITE_FLASH:
		case CE_BLUE_FLASH:
		case CE_SM_BLUE_FLASH:
		case CE_HWSPLITFLASH:
		case CE_RED_FLASH:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Flash.origin);
			sv.Effects[index].expire_time = sv.time + 1;
			break;

		case CE_RIDER_DEATH:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].RD.origin);
			break;

		case CE_TELEPORTERPUFFS:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Teleporter.origin);
			sv.Effects[index].expire_time = sv.time + 1;
			break;

		case CE_TELEPORTERBODY:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Teleporter.origin);
			VectorCopy(G_VECTOR(OFS_PARM2),sv.Effects[index].Teleporter.velocity[0]);
			sv.Effects[index].Teleporter.skinnum = G_FLOAT(OFS_PARM3);
			sv.Effects[index].expire_time = sv.time + 1;
			break;

		case CE_BONESHRAPNEL:
		case CE_HWBONEBALL:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Missile.origin);
			VectorCopy(G_VECTOR(OFS_PARM2),sv.Effects[index].Missile.velocity);
			VectorCopy(G_VECTOR(OFS_PARM3),sv.Effects[index].Missile.angle);
			VectorCopy(G_VECTOR(OFS_PARM2),sv.Effects[index].Missile.avelocity);

			sv.Effects[index].expire_time = sv.time + 10;
			break;
		case CE_BONESHARD:
		case CE_HWRAVENSTAFF:
		case CE_HWMISSILESTAR:
		case CE_HWEIDOLONSTAR:
		case CE_HWRAVENPOWER:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Missile.origin);
			VectorCopy(G_VECTOR(OFS_PARM2),sv.Effects[index].Missile.velocity);
			sv.Effects[index].expire_time = sv.time + 10;
			break;
		case CE_DEATHBUBBLES:
			VectorCopy(G_VECTOR(OFS_PARM2), sv.Effects[index].Bubble.offset);
			sv.Effects[index].Bubble.owner = G_EDICTNUM(OFS_PARM1);
			sv.Effects[index].Bubble.count = G_FLOAT(OFS_PARM3);
			sv.Effects[index].expire_time = sv.time + 30;
			break;
		case CE_HWDRILLA:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Missile.origin);
			VectorCopy(G_VECTOR(OFS_PARM2),sv.Effects[index].Missile.angle);
			sv.Effects[index].Missile.speed = G_FLOAT(OFS_PARM3);
			sv.Effects[index].expire_time = sv.time + 10;
			break;
		case CE_TRIPMINESTILL:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Chain.origin);
			VectorCopy(G_VECTOR(OFS_PARM2),sv.Effects[index].Chain.velocity);
			sv.Effects[index].expire_time = sv.time + 70;
			break;
		case CE_TRIPMINE:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Chain.origin);
			VectorCopy(G_VECTOR(OFS_PARM2),sv.Effects[index].Chain.velocity);
			sv.Effects[index].expire_time = sv.time + 10;
			break;
		case CE_SCARABCHAIN:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Chain.origin);
			sv.Effects[index].Chain.owner = G_EDICTNUM(OFS_PARM2);
			sv.Effects[index].Chain.material = G_INT(OFS_PARM3);
			sv.Effects[index].Chain.tag = G_INT(OFS_PARM4);
			sv.Effects[index].Chain.state = 0;
			sv.Effects[index].expire_time = sv.time + 15;
			break;
		case CE_HWSHEEPINATOR:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Xbow.origin[0]);
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Xbow.origin[1]);
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Xbow.origin[2]);
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Xbow.origin[3]);
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Xbow.origin[4]);
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Xbow.origin[5]);
			VectorCopy(G_VECTOR(OFS_PARM2), sv.Effects[index].Xbow.angle);
			sv.Effects[index].Xbow.bolts = 5;
			sv.Effects[index].Xbow.activebolts = 31;
			sv.Effects[index].Xbow.randseed = 0;
			sv.Effects[index].Xbow.turnedbolts = 0;
			sv.Effects[index].expire_time = sv.time + 7;
			break;
		case CE_HWXBOWSHOOT:
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Xbow.origin[0]);
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Xbow.origin[1]);
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Xbow.origin[2]);
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Xbow.origin[3]);
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Xbow.origin[4]);
			VectorCopy(G_VECTOR(OFS_PARM1), sv.Effects[index].Xbow.origin[5]);
			VectorCopy(G_VECTOR(OFS_PARM2), sv.Effects[index].Xbow.angle);
			sv.Effects[index].Xbow.bolts = G_FLOAT(OFS_PARM3);
			sv.Effects[index].Xbow.randseed = G_FLOAT(OFS_PARM4);
			sv.Effects[index].Xbow.turnedbolts = 0;
			if (sv.Effects[index].Xbow.bolts == 3)
			{
				sv.Effects[index].Xbow.activebolts = 7;
			}
			else
			{
				sv.Effects[index].Xbow.activebolts = 31;
			}
			sv.Effects[index].expire_time = sv.time + 15;
			break;
		default:
//			Sys_Error ("SV_ParseEffect: bad type");
			PR_RunError ("SV_SendEffect: bad type");
	}

	SV_SendEffect(sb,index);
}

// All changes need to be in SV_SendEffect(), SV_ParseEffect(),
// SV_SaveEffects(), SV_LoadEffects(), CL_ParseEffect()
void SV_SaveEffects(FILE *FH)
{
	int index,count;

	for(index=count=0;index<MAX_EFFECTS;index++)
		if (sv.Effects[index].type)
			count++;

	fprintf(FH,"Effects: %d\n",count);

	for(index=count=0;index<MAX_EFFECTS;index++)
		if (sv.Effects[index].type)
		{
			fprintf(FH,"Effect: %d %d %f: ",index,sv.Effects[index].type,sv.Effects[index].expire_time);

			switch(sv.Effects[index].type)
			{
				case CE_RAIN:
					fprintf(FH, "%f ", sv.Effects[index].Rain.min_org[0]);
					fprintf(FH, "%f ", sv.Effects[index].Rain.min_org[1]);
					fprintf(FH, "%f ", sv.Effects[index].Rain.min_org[2]);
					fprintf(FH, "%f ", sv.Effects[index].Rain.max_org[0]);
					fprintf(FH, "%f ", sv.Effects[index].Rain.max_org[1]);
					fprintf(FH, "%f ", sv.Effects[index].Rain.max_org[2]);
					fprintf(FH, "%f ", sv.Effects[index].Rain.e_size[0]);
					fprintf(FH, "%f ", sv.Effects[index].Rain.e_size[1]);
					fprintf(FH, "%f ", sv.Effects[index].Rain.e_size[2]);
					fprintf(FH, "%f ", sv.Effects[index].Rain.dir[0]);
					fprintf(FH, "%f ", sv.Effects[index].Rain.dir[1]);
					fprintf(FH, "%f ", sv.Effects[index].Rain.dir[2]);
					fprintf(FH, "%d ", sv.Effects[index].Rain.color);
					fprintf(FH, "%d ", sv.Effects[index].Rain.count);
					fprintf(FH, "%f\n", sv.Effects[index].Rain.wait);
					break;

				case CE_FOUNTAIN:
					fprintf(FH, "%f ", sv.Effects[index].Fountain.pos[0]);
					fprintf(FH, "%f ", sv.Effects[index].Fountain.pos[1]);
					fprintf(FH, "%f ", sv.Effects[index].Fountain.pos[2]);
					fprintf(FH, "%f ", sv.Effects[index].Fountain.angle[0]);
					fprintf(FH, "%f ", sv.Effects[index].Fountain.angle[1]);
					fprintf(FH, "%f ", sv.Effects[index].Fountain.angle[2]);
					fprintf(FH, "%f ", sv.Effects[index].Fountain.movedir[0]);
					fprintf(FH, "%f ", sv.Effects[index].Fountain.movedir[1]);
					fprintf(FH, "%f ", sv.Effects[index].Fountain.movedir[2]);
					fprintf(FH, "%d ", sv.Effects[index].Fountain.color);
					fprintf(FH, "%d\n", sv.Effects[index].Fountain.cnt);
					break;

				case CE_QUAKE:
					fprintf(FH, "%f ", sv.Effects[index].Quake.origin[0]);
					fprintf(FH, "%f ", sv.Effects[index].Quake.origin[1]);
					fprintf(FH, "%f ", sv.Effects[index].Quake.origin[2]);
					fprintf(FH, "%f\n", sv.Effects[index].Quake.radius);
					break;

				case CE_WHITE_SMOKE:
				case CE_GREEN_SMOKE:
				case CE_GREY_SMOKE:
				case CE_RED_SMOKE:
				case CE_SLOW_WHITE_SMOKE:
				case CE_TELESMK1:
				case CE_TELESMK2:
				case CE_GHOST:
				case CE_REDCLOUD:
				case CE_ACID_MUZZFL:
				case CE_FLAMESTREAM:
				case CE_FLAMEWALL:
				case CE_FLAMEWALL2:
				case CE_ONFIRE:
				case CE_RIPPLE:
					fprintf(FH, "%f ", sv.Effects[index].Smoke.origin[0]);
					fprintf(FH, "%f ", sv.Effects[index].Smoke.origin[1]);
					fprintf(FH, "%f ", sv.Effects[index].Smoke.origin[2]);
					fprintf(FH, "%f ", sv.Effects[index].Smoke.velocity[0]);
					fprintf(FH, "%f ", sv.Effects[index].Smoke.velocity[1]);
					fprintf(FH, "%f ", sv.Effects[index].Smoke.velocity[2]);
					fprintf(FH, "%f\n", sv.Effects[index].Smoke.framelength);
					break;

				case CE_SM_WHITE_FLASH:
				case CE_YELLOWRED_FLASH:
				case CE_BLUESPARK:
				case CE_YELLOWSPARK:
				case CE_SM_CIRCLE_EXP:
				case CE_BG_CIRCLE_EXP:
				case CE_SM_EXPLOSION:
				case CE_SM_EXPLOSION2:
				case CE_BG_EXPLOSION:
				case CE_FLOOR_EXPLOSION:
				case CE_BLUE_EXPLOSION:
				case CE_REDSPARK:
				case CE_GREENSPARK:
				case CE_ICEHIT:
				case CE_MEDUSA_HIT:
				case CE_MEZZO_REFLECT:
				case CE_FLOOR_EXPLOSION2:
				case CE_XBOW_EXPLOSION:
				case CE_NEW_EXPLOSION:
				case CE_MAGIC_MISSILE_EXPLOSION:
				case CE_BONE_EXPLOSION:
				case CE_BLDRN_EXPL:
				case CE_BRN_BOUNCE:
				case CE_LSHOCK:
				case CE_ACID_HIT:
				case CE_ACID_SPLAT:
				case CE_ACID_EXPL:
				case CE_LBALL_EXPL:
				case CE_FIREWALL_SMALL:
				case CE_FIREWALL_MEDIUM:
				case CE_FIREWALL_LARGE:
				case CE_FBOOM:
				case CE_BOMB:
					fprintf(FH, "%f ", sv.Effects[index].Smoke.origin[0]);
					fprintf(FH, "%f ", sv.Effects[index].Smoke.origin[1]);
					fprintf(FH, "%f\n", sv.Effects[index].Smoke.origin[2]);
					break;

				case CE_WHITE_FLASH:
				case CE_BLUE_FLASH:
				case CE_SM_BLUE_FLASH:
				case CE_HWSPLITFLASH:
				case CE_RED_FLASH:
					fprintf(FH, "%f ", sv.Effects[index].Flash.origin[0]);
					fprintf(FH, "%f ", sv.Effects[index].Flash.origin[1]);
					fprintf(FH, "%f\n", sv.Effects[index].Flash.origin[2]);
					break;

				case CE_RIDER_DEATH:
					fprintf(FH, "%f ", sv.Effects[index].RD.origin[0]);
					fprintf(FH, "%f ", sv.Effects[index].RD.origin[1]);
					fprintf(FH, "%f\n", sv.Effects[index].RD.origin[2]);
					break;
				case CE_TELEPORTERPUFFS:
					fprintf(FH, "%f ", sv.Effects[index].Teleporter.origin[0]);
					fprintf(FH, "%f ", sv.Effects[index].Teleporter.origin[1]);
					fprintf(FH, "%f\n", sv.Effects[index].Teleporter.origin[2]);
					break;

				case CE_TELEPORTERBODY:
					fprintf(FH, "%f ", sv.Effects[index].Teleporter.origin[0]);
					fprintf(FH, "%f ", sv.Effects[index].Teleporter.origin[1]);
					fprintf(FH, "%f\n", sv.Effects[index].Teleporter.origin[2]);
					break;

				case CE_BONESHRAPNEL:
				case CE_HWBONEBALL:
					fprintf(FH, "%f ", sv.Effects[index].Missile.origin[0]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.origin[1]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.origin[2]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.velocity[0]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.velocity[1]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.velocity[2]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.angle[0]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.angle[1]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.angle[2]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.avelocity[0]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.avelocity[1]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.avelocity[2]);
					break;
				case CE_BONESHARD:
				case CE_HWRAVENSTAFF:
				case CE_HWRAVENPOWER:
				case CE_HWMISSILESTAR:
				case CE_HWEIDOLONSTAR:
					fprintf(FH, "%f ", sv.Effects[index].Missile.origin[0]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.origin[1]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.origin[2]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.velocity[0]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.velocity[1]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.velocity[2]);
					break;
				case CE_DEATHBUBBLES:
					fprintf(FH, "%f ", sv.Effects[index].Bubble.offset[0]);
					fprintf(FH, "%f ", sv.Effects[index].Bubble.offset[1]);
					fprintf(FH, "%f ", sv.Effects[index].Bubble.offset[2]);
					fprintf(FH, "%f ", sv.Effects[index].Bubble.owner);
					fprintf(FH, "%f ", sv.Effects[index].Bubble.count);
					break;
				case CE_HWDRILLA:
					fprintf(FH, "%f ", sv.Effects[index].Missile.origin[0]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.origin[1]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.origin[2]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.angle[0]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.angle[1]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.angle[2]);
					fprintf(FH, "%f ", sv.Effects[index].Missile.speed);
					break;
				case CE_SCARABCHAIN:
					fprintf(FH, "%f ", sv.Effects[index].Chain.origin[0]);
					fprintf(FH, "%f ", sv.Effects[index].Chain.origin[1]);
					fprintf(FH, "%f ", sv.Effects[index].Chain.origin[2]);
					fprintf(FH, "%f ", sv.Effects[index].Chain.owner);
					fprintf(FH, "%f ", sv.Effects[index].Chain.material);
					fprintf(FH, "%f ", sv.Effects[index].Chain.tag);
					break;
				case CE_HWSHEEPINATOR:
				case CE_HWXBOWSHOOT:
					fprintf(FH, "%f ", sv.Effects[index].Xbow.origin[5][0]);
					fprintf(FH, "%f ", sv.Effects[index].Xbow.origin[5][1]);
					fprintf(FH, "%f ", sv.Effects[index].Xbow.origin[5][2]);
					fprintf(FH, "%f ", sv.Effects[index].Xbow.angle[0]);
					fprintf(FH, "%f ", sv.Effects[index].Xbow.angle[1]);
					fprintf(FH, "%f ", sv.Effects[index].Xbow.angle[2]);
					fprintf(FH, "%f ", sv.Effects[index].Xbow.bolts);
					fprintf(FH, "%f ", sv.Effects[index].Xbow.activebolts);
					fprintf(FH, "%f ", sv.Effects[index].Xbow.turnedbolts);
					break;
				case CE_TRIPMINESTILL:
				case CE_TRIPMINE:
					fprintf(FH, "%f ", sv.Effects[index].Chain.origin[0]);
					fprintf(FH, "%f ", sv.Effects[index].Chain.origin[1]);
					fprintf(FH, "%f ", sv.Effects[index].Chain.origin[2]);
					fprintf(FH, "%f ", sv.Effects[index].Chain.velocity[0]);
					fprintf(FH, "%f ", sv.Effects[index].Chain.velocity[1]);
					fprintf(FH, "%f ", sv.Effects[index].Chain.velocity[2]);
					break;
				default:
					PR_RunError ("SV_SaveEffect: bad type");
					break;
			}

		}
}

// All changes need to be in SV_SendEffect(), SV_ParseEffect(),
// SV_SaveEffects(), SV_LoadEffects(), CL_ParseEffect()
void SV_LoadEffects(FILE *FH)
{
	int index,Total,count;

	// Since the map is freshly loaded, clear out any effects as a result of
	// the loading
	SV_ClearEffects();

	fscanf(FH,"Effects: %d\n",&Total);

	for(count=0;count<Total;count++)
	{
		fscanf(FH,"Effect: %d ",&index);
		fscanf(FH,"%d %f: ",&sv.Effects[index].type,&sv.Effects[index].expire_time);

		switch(sv.Effects[index].type)
		{
			case CE_RAIN:
				fscanf(FH, "%f ", &sv.Effects[index].Rain.min_org[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Rain.min_org[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Rain.min_org[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Rain.max_org[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Rain.max_org[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Rain.max_org[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Rain.e_size[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Rain.e_size[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Rain.e_size[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Rain.dir[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Rain.dir[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Rain.dir[2]);
				fscanf(FH, "%d ", &sv.Effects[index].Rain.color);
				fscanf(FH, "%d ", &sv.Effects[index].Rain.count);
				fscanf(FH, "%f\n", &sv.Effects[index].Rain.wait);
				break;

			case CE_FOUNTAIN:
				fscanf(FH, "%f ", &sv.Effects[index].Fountain.pos[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Fountain.pos[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Fountain.pos[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Fountain.angle[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Fountain.angle[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Fountain.angle[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Fountain.movedir[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Fountain.movedir[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Fountain.movedir[2]);
				fscanf(FH, "%d ", &sv.Effects[index].Fountain.color);
				fscanf(FH, "%d\n", &sv.Effects[index].Fountain.cnt);
				break;

			case CE_QUAKE:
				fscanf(FH, "%f ", &sv.Effects[index].Quake.origin[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Quake.origin[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Quake.origin[2]);
				fscanf(FH, "%f\n", &sv.Effects[index].Quake.radius);
				break;

			case CE_WHITE_SMOKE:
			case CE_GREEN_SMOKE:
			case CE_GREY_SMOKE:
			case CE_RED_SMOKE:
			case CE_SLOW_WHITE_SMOKE:
			case CE_TELESMK1:
			case CE_TELESMK2:
			case CE_GHOST:
			case CE_REDCLOUD:
			case CE_ACID_MUZZFL:
			case CE_FLAMESTREAM:
			case CE_FLAMEWALL:
			case CE_FLAMEWALL2:
			case CE_ONFIRE:
			case CE_RIPPLE:
				fscanf(FH, "%f ", &sv.Effects[index].Smoke.origin[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Smoke.origin[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Smoke.origin[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Smoke.velocity[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Smoke.velocity[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Smoke.velocity[2]);
				fscanf(FH, "%f\n", &sv.Effects[index].Smoke.framelength);
				break;

			case CE_SM_WHITE_FLASH:
			case CE_YELLOWRED_FLASH:
			case CE_BLUESPARK:
			case CE_YELLOWSPARK:
			case CE_SM_CIRCLE_EXP:
			case CE_BG_CIRCLE_EXP:
			case CE_SM_EXPLOSION:
			case CE_SM_EXPLOSION2:
			case CE_BG_EXPLOSION:
			case CE_FLOOR_EXPLOSION:
			case CE_BLUE_EXPLOSION:
			case CE_REDSPARK:
			case CE_GREENSPARK:
			case CE_ICEHIT:
			case CE_MEDUSA_HIT:
			case CE_MEZZO_REFLECT:
			case CE_FLOOR_EXPLOSION2:
			case CE_XBOW_EXPLOSION:
			case CE_NEW_EXPLOSION:
			case CE_MAGIC_MISSILE_EXPLOSION:
			case CE_BONE_EXPLOSION:
			case CE_BLDRN_EXPL:
			case CE_BRN_BOUNCE:
			case CE_LSHOCK:
			case CE_ACID_HIT:
			case CE_ACID_SPLAT:
			case CE_ACID_EXPL:
			case CE_LBALL_EXPL:
			case CE_FBOOM:
			case CE_FIREWALL_SMALL:
			case CE_FIREWALL_MEDIUM:
			case CE_FIREWALL_LARGE:
			case CE_BOMB:

				fscanf(FH, "%f ", &sv.Effects[index].Smoke.origin[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Smoke.origin[1]);
				fscanf(FH, "%f\n", &sv.Effects[index].Smoke.origin[2]);
				break;

			case CE_WHITE_FLASH:
			case CE_BLUE_FLASH:
			case CE_SM_BLUE_FLASH:
			case CE_HWSPLITFLASH:
			case CE_RED_FLASH:
				fscanf(FH, "%f ", &sv.Effects[index].Flash.origin[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Flash.origin[1]);
				fscanf(FH, "%f\n", &sv.Effects[index].Flash.origin[2]);
				break;

			case CE_RIDER_DEATH:
				fscanf(FH, "%f ", &sv.Effects[index].RD.origin[0]);
				fscanf(FH, "%f ", &sv.Effects[index].RD.origin[1]);
				fscanf(FH, "%f\n", &sv.Effects[index].RD.origin[2]);
				break;

			case CE_TELEPORTERPUFFS:
				fscanf(FH, "%f ", &sv.Effects[index].Teleporter.origin[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Teleporter.origin[1]);
				fscanf(FH, "%f\n", &sv.Effects[index].Teleporter.origin[2]);
				break;

			case CE_TELEPORTERBODY:
				fscanf(FH, "%f ", &sv.Effects[index].Teleporter.origin[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Teleporter.origin[1]);
				fscanf(FH, "%f\n", &sv.Effects[index].Teleporter.origin[2]);
				break;

			case CE_BONESHRAPNEL:
			case CE_HWBONEBALL:
				fscanf(FH, "%f ", &sv.Effects[index].Missile.origin[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.origin[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.origin[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.velocity[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.velocity[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.velocity[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.angle[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.angle[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.angle[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.avelocity[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.avelocity[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.avelocity[2]);
				break;
			case CE_BONESHARD:
			case CE_HWRAVENSTAFF:
			case CE_HWRAVENPOWER:
			case CE_HWMISSILESTAR:
			case CE_HWEIDOLONSTAR:
				fscanf(FH, "%f ", &sv.Effects[index].Missile.origin[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.origin[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.origin[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.velocity[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.velocity[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.velocity[2]);
				break;
			case CE_DEATHBUBBLES:
				fscanf(FH, "%f ", &sv.Effects[index].Bubble.offset[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Bubble.offset[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Bubble.offset[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Bubble.owner);
				fscanf(FH, "%f ", &sv.Effects[index].Bubble.count);
				break;
			case CE_HWDRILLA:
				fscanf(FH, "%f ", &sv.Effects[index].Missile.origin[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.origin[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.origin[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.angle[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.angle[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.angle[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Missile.speed);
				break;
			case CE_SCARABCHAIN:
				fscanf(FH, "%f ", &sv.Effects[index].Chain.origin[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Chain.origin[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Chain.origin[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Chain.owner);
				fscanf(FH, "%f ", &sv.Effects[index].Chain.material);
				fscanf(FH, "%f ", &sv.Effects[index].Chain.tag);
				break;
			case CE_HWSHEEPINATOR:
			case CE_HWXBOWSHOOT:
				fscanf(FH, "%f ", &sv.Effects[index].Xbow.origin[5][0]);
				fscanf(FH, "%f ", &sv.Effects[index].Xbow.origin[5][1]);
				fscanf(FH, "%f ", &sv.Effects[index].Xbow.origin[5][2]);
				fscanf(FH, "%f ", &sv.Effects[index].Xbow.angle[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Xbow.angle[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Xbow.angle[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Xbow.bolts);
				fscanf(FH, "%f ", &sv.Effects[index].Xbow.activebolts);
				fscanf(FH, "%f ", &sv.Effects[index].Xbow.turnedbolts);
				break;
			case CE_TRIPMINESTILL:
			case CE_TRIPMINE:
				fscanf(FH, "%f ", &sv.Effects[index].Chain.origin[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Chain.origin[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Chain.origin[2]);
				fscanf(FH, "%f ", &sv.Effects[index].Chain.velocity[0]);
				fscanf(FH, "%f ", &sv.Effects[index].Chain.velocity[1]);
				fscanf(FH, "%f ", &sv.Effects[index].Chain.velocity[2]);
				break;
			default:
				PR_RunError ("SV_SaveEffect: bad type");
				break;
		}
	}
}


// this random generator can have its effects duplicated on the client
// side by passing the randomseed over the network, as opposed to sending
// all the generated values
static unsigned int randomseed;
void SV_setseed(int seed)
{
	randomseed = seed;
}

float SV_seedrand(void)
{
	int max;
	randomseed = (randomseed * 877 + 573) % 9968;
	return (float)randomseed / 9968;
}


// this will create several effects and store the ids in the array
static float MultiEffectIds[10];
static int MultiEffectIdCount;

void SV_ParseMultiEffect(sizebuf_t *sb)
{
	int index, count;
	byte effect;
	vec3_t	orig, vel, right;

	MultiEffectIdCount = 0;
	effect = G_FLOAT(OFS_PARM0);
	switch(effect)
	{
	case CE_HWRAVENPOWER:
		// need to set aside 3 effect ids
		
		MSG_WriteByte (sb, svc_multieffect);
		MSG_WriteByte (sb, effect);

		VectorCopy(G_VECTOR(OFS_PARM1), orig);
		MSG_WriteCoord(sb, orig[0]);
		MSG_WriteCoord(sb, orig[1]);
		MSG_WriteCoord(sb, orig[2]);
		VectorCopy(G_VECTOR(OFS_PARM2), vel);
		MSG_WriteCoord(sb, vel[0]);
		MSG_WriteCoord(sb, vel[1]);
		MSG_WriteCoord(sb, vel[2]);
		for(count=0;count<3;count++)
		{
			for(index=0;index<MAX_EFFECTS;index++)
				if (!sv.Effects[index].type || 
					(sv.Effects[index].expire_time && sv.Effects[index].expire_time <= sv.time)) 
					break;
					if (index >= MAX_EFFECTS)
			{
				PR_RunError ("MAX_EFFECTS reached");
				return;
			}
			MSG_WriteByte(sb, index);
			sv.Effects[index].type = CE_HWRAVENPOWER;
			VectorCopy(orig, sv.Effects[index].Missile.origin);
			VectorCopy(vel, sv.Effects[index].Missile.velocity);
			sv.Effects[index].expire_time = sv.time + 10;
			MultiEffectIds[count] = index;
		}
		break;
	default:
		PR_RunError ("SV_ParseMultiEffect: bad type");
	}
}

float SV_GetMultiEffectId(void)
{
	MultiEffectIdCount++;
	return MultiEffectIds[MultiEffectIdCount-1];
}

/*
 * $Log: /HexenWorld/Server/sv_effect.c $
 * 
 * 39    5/25/98 1:30p Mgummelt
 * 
 * 38    4/23/98 5:17p Mgummelt
 * Fixes for Siege
 * 
 * 37    4/20/98 2:03p Rjohnson
 * Big update
 * 
 * 36    4/08/98 6:37p Rjohnson
 * Updated to send effect
 * 
 * 35    3/26/98 5:01p Mgummelt
 * Added Mission Pack client effects
 * 
 * 34    3/25/98 12:45p Rjohnson
 * Check for death bubbles without an owner
 * 
 * 33    3/24/98 3:21p Rjohnson
 * Enable testing for tripmine initial wire
 * 
 * 32    3/22/98 4:48p Rjohnson
 * Effects use PVS
 * 
 * 31    3/21/98 8:10p Rjohnson
 * Effects through reliable datagram per client now
 * 
 * 30    3/11/98 9:46p Ssengele
 * xbolts (& theoretically, setstaff & lambinator) teleport cleanly.
 * currently cause a spike, which can and will be fixed soon.
 * server by default only sends the effects close enough to client to
 * matter to that client.
 * 
 * 29    3/11/98 3:04p Ssengele
 * fixed test distance for xbow&lambinator
 * 
 * 28    3/11/98 3:03p Ssengele
 * 
 * 27    3/10/98 3:55p Ssengele
 * expire times more reasonable for effects.
 * 
 * 26    3/09/98 4:47p Ssengele
 * started solidifying server tracking of updateable effects: not used
 * yet; made recoil from daggerdive less extreme than jab; made sure
 * update message doesn't write over effect info if the type updated is
 * different from the type already there.
 * 
 * 25    3/05/98 10:25p Ssengele
 * fixed a bug with deathbubbles (was crashing when non-client called it)
 * 
 * 24    3/05/98 9:58p Ssengele
 * attempt to limit flickering of scarab chains; minimum of 3 b&g mana for
 * untomed scarab
 * 
 * 23    3/03/98 8:09p Ssengele
 * reflection code for xbow bolts and sheepinator; also, a little variance
 * for where bolts start out, so autoaim doesn't make them clump up quite
 * as much (variance is consistent between sv & cl)
 * 
 * 22    3/03/98 4:43p Ssengele
 * sheepinator uses xbow code now.
 * 
 * 21    3/03/98 1:58p Ssengele
 * er, took a break from weapon stuff to move the bubble code over to a
 * client effect
 * 
 * 20    3/02/98 6:02p Ssengele
 * made scarab chains disappear w/o being burden on nettraffic
 * 
 * 19    3/02/98 4:12p Ssengele
 * minor improvement for xbow, punchdagger sheep gone away
 * 
 * 18    3/02/98 2:35p Ssengele
 * set staff worst-case not bad at all anymore--the drilla is an effect
 * 
 * 17    3/02/98 12:36p Ssengele
 * shaved a few bytes from powered up setstaff (moving puffs to client),
 * have begun working on worst-case unpowered setstaff (explosions are now
 * tempent
 * vs effect & sound they were before). unpowered ss still clogs up net
 * when firing fast--i believe i'll make the whole thing an effect.
 * 
 * 16    2/28/98 3:26p Ssengele
 * some work on set staff
 * 
 * 15    2/27/98 7:12p Ssengele
 * grrrrrrrr...xbow more optimized than ever (plus chunks, or did i add
 * those yesterday?), still spiking not-so-pretty (nudges first red line)
 * occasionally in the worst case scenario.
 * 
 * 14    2/26/98 3:55p Rmidthun
 * powered ravenstaff uses multieffect, also moved tempent stuff to be
 * called from client effects, there is STILL a little red and I'm not
 * sure if the client effects are always in the correct place, but it
 * should be close anyway.
 * 
 * 13    2/26/98 1:34p Rmidthun
 * working on multiple effects with one net message
 * 
 * 12    2/25/98 4:01p Rmidthun
 * ravens are now client effect too, still need to reduce the large
 * creation spike
 * 
 * 11    2/24/98 1:41p Ssengele
 * 
 * 10    2/24/98 11:44a Rmidthun
 * magic missiles are client effects, the homing function sends updates
 * 
 * 9     2/23/98 8:04p Ssengele
 * erg--checking in so's not to hog these files TOO much.  cleffect that
 * handles multiple xbow bolts is IN, but impact is neither consistant nor
 * in the right spot.
 * 
 * 8     2/23/98 9:47a Rmidthun
 * combined small blue flash and split sound for raven staff
 * 
 * 7     2/20/98 4:45p Rmidthun
 * moved rest of sounds to client side
 * 
 * 6     2/20/98 1:15p Ssengele
 * basic server side of scarab chain in, the effect updating is only half
 * in, pending the availability of a handful of precious files.
 * 
 * 5     2/20/98 10:08a Rmidthun
 * bone shards don't need to send angle or angular vel
 * 
 * 4     2/19/98 8:07p Ssengele
 * tripmine client effect in--added access to streams in cl_effect
 * 
 * 3     2/19/98 7:00p Rmidthun
 * ravenstaff is almost all client effect, added a "turneffect" builtin to
 * reduce net traffic further
 * 
 * 2     2/19/98 4:30p Rmidthun
 * due to an error in sourcesafe, the previous versions were destroyed.
 * sorry.
 * 
 * 1     2/19/98 4:29p Rmidthun
 * 
 * 8     2/19/98 3:56p Rmidthun
 * added bone ball
 * 
 * 7     2/18/98 8:25p Ssengele
 * xbow client effect (for normal mode) lookin good--but it trades off
 * significantly less traffic when arrows are in air for a slightly bigger
 * spike when they're fired.  will figure out how to minimize that spike
 * yet.
 * 
 * 6     2/18/98 5:37p Ssengele
 * start at xbow bolt client effect
 * 
 * 5     2/18/98 3:14p Rmidthun
 * added load/save for client effect stars
 * 
 * 4     2/18/98 11:53a Rmidthun
 * eidolon star (SECOND ATTEMPT)
 * 
 * 3     2/17/98 6:54p Rmidthun
 * moving missile stars to client, half done
 * 
 * 2     2/06/98 4:46p Rjohnson
 * Server Effects
 * 
 * 1     2/05/98 5:07p Rjohnson
 * 
 */
