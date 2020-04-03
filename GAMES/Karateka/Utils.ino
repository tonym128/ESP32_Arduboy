#include "src/utils/Arduboy2Ext.h"

#define ACTIONS_NUMBER_OF_STANCES 42

const uint8_t PROGMEM actions_distance[] = {


  /* ACTION_MED_KICK ------------------------------------------------------------------------------------- */

/* 000  ACTION_MED_KICK,  STANCE_DEFAULT,                   */ (4 * 30) + DAMAGE_2_POINT,
/* 001  ACTION_MED_KICK,  STANCE_DEFAULT_LEAN_BACK          */ (4 * 30) + DAMAGE_2_POINT,
/* 002  ACTION_MED_KICK,  STANCE_KICK_READY                 */ (4 * 26) + DAMAGE_2_POINT, 
/* 003  ACTION_MED_KICK,  STANCE_DEFAULT_LEAN_FORWARD       */ (4 * 30) + DAMAGE_2_POINT,
/* 004  ACTION_MED_KICK,  STANCE_KICK_STANDING_TRANSITION   */ (4 * 30) + DAMAGE_2_POINT, 
/* 005  ACTION_MED_KICK,  STANCE_KICK_MED_END               */ (4 * 23) + DAMAGE_2_POINT,
/* 006  ACTION_MED_KICK,  STANCE_KICK_LOW_END               */ (4 * 30) + DAMAGE_2_POINT,
/* 007  ACTION_MED_KICK,  STANCE_KICK_HIGH_END              */ (4 * 44) + DAMAGE_2_POINT,
/* 008  ACTION_MED_KICK,  STANCE_PUNCH_MED_RH_END           */ (4 * 30) + DAMAGE_2_POINT, 
/* 009  ACTION_MED_KICK,  STANCE_PUNCH_MED_LH_END           */ (4 * 30) + DAMAGE_2_POINT,
/* 010  ACTION_MED_KICK,  STANCE_PUNCH_HIGH_RH_END          */ (4 * 30) + DAMAGE_2_POINT,
/* 011  ACTION_MED_KICK,  STANCE_PUNCH_HIGH_LH_END          */ (4 * 30) + DAMAGE_2_POINT, 
/* 012  ACTION_MED_KICK,  STANCE_PUNCH_LOW_RH_END           */ (4 * 30) + DAMAGE_2_POINT,
/* 013  ACTION_MED_KICK,  STANCE_PUNCH_LOW_LH_END           */ (4 * 30) + DAMAGE_2_POINT,
/* 014  ACTION_MED_KICK,  STANCE_PUNCH_READY                */ (4 * 23) + DAMAGE_2_POINT, 
/* 015  ACTION_MED_KICK,  STANCE_STANDING_UPRIGHT           */ (4 * 30) + DAMAGE_2_POINT,
/* 016  ACTION_MED_KICK,  STANCE_SIDLING_1                  */ (4 * 30) + DAMAGE_2_POINT,
/* 017  ACTION_MED_KICK,  STANCE_SIDLING_2                  */ (4 * 30) + DAMAGE_2_POINT, 
/* 018  ACTION_MED_KICK,  STANCE_SIDLING_3                  */ (4 * 30) + DAMAGE_2_POINT,
/* 019  ACTION_MED_KICK,  STANCE_SIDLING_4                  */ (4 * 30) + DAMAGE_2_POINT,
/* 020  ACTION_MED_KICK,  STANCE_SIDLING_5                  */ (4 * 30) + DAMAGE_2_POINT, 
/* 021  ACTION_MED_KICK,  STANCE_SIDLING_6                  */ (4 * 30) + DAMAGE_2_POINT,
/* 022  ACTION_MED_KICK,  STANCE_SIDLING_7                  */ (4 * 30) + DAMAGE_2_POINT,
/* 023  ACTION_MED_KICK,  STANCE_RUNNING_1                  */ (4 * 30) + DAMAGE_2_POINT, 
/* 024  ACTION_MED_KICK,  STANCE_RUNNING_2                  */ (4 * 30) + DAMAGE_2_POINT,
/* 025  ACTION_MED_KICK,  STANCE_RUNNING_3                  */ (4 * 30) + DAMAGE_2_POINT,
/* 026  ACTION_MED_KICK,  STANCE_RUNNING_4                  */ (4 * 30) + DAMAGE_2_POINT, 
/* 027  ACTION_MED_KICK,  STANCE_RUNNING_5                  */ (4 * 30) + DAMAGE_2_POINT,
/* 028  ACTION_MED_KICK,  STANCE_RUNNING_6                  */ (4 * 30) + DAMAGE_2_POINT,
/* 029  ACTION_MED_KICK,  STANCE_RUNNING_7                  */ (4 * 30) + DAMAGE_2_POINT, 
/* 030  ACTION_MED_KICK,  STANCE_RUNNING_8                  */ (4 * 30) + DAMAGE_2_POINT, 
/* 031  ACTION_MED_KICK,  STANCE_RUNNING_STRAIGHTEN_UP      */ (4 * 30) + DAMAGE_2_POINT,
/* 032  ACTION_MED_KICK,  STANCE_STANDING_UPRIGHT_REV       */ (4 * 30) + DAMAGE_2_POINT, 
/* 033  ACTION_MED_KICK,  STANCE_RUNNING_1_REV              */ (4 * 30) + DAMAGE_2_POINT,
/* 034  ACTION_MED_KICK,  STANCE_RUNNING_2_REV              */ (4 * 30) + DAMAGE_2_POINT,
/* 035  ACTION_MED_KICK,  STANCE_RUNNING_3_REV              */ (4 * 30) + DAMAGE_2_POINT, 
/* 036  ACTION_MED_KICK,  STANCE_RUNNING_4_REV              */ (4 * 30) + DAMAGE_2_POINT,
/* 037  ACTION_MED_KICK,  STANCE_RUNNING_5_REV              */ (4 * 30) + DAMAGE_2_POINT,
/* 038  ACTION_MED_KICK,  STANCE_RUNNING_6_REV              */ (4 * 30) + DAMAGE_2_POINT, 
/* 039  ACTION_MED_KICK,  STANCE_RUNNING_7_REV              */ (4 * 30) + DAMAGE_2_POINT,
/* 040  ACTION_MED_KICK,  STANCE_RUNNING_8_REV              */ (4 * 30) + DAMAGE_2_POINT,
/* 041  ACTION_MED_KICK,  STANCE_RUNNING_STRAIGHTEN_UP_REV  */ (4 * 30) + DAMAGE_2_POINT, 


/* ACTION_LOW_KICK ----------------------------------------------------------- */

/* 042  ACTION_LOW_KICK,  STANCE_DEFAULT,                   */ (4 * 35) + DAMAGE_1_POINT,
/* 043  ACTION_LOW_KICK,  STANCE_DEFAULT_LEAN_BACK          */ (4 * 35) + DAMAGE_1_POINT,
/* 044  ACTION_LOW_KICK,  STANCE_KICK_READY                 */ (4 * 23) + DAMAGE_1_POINT,
/* 045  ACTION_LOW_KICK,  STANCE_DEFAULT_LEAN_FORWARD       */ (4 * 35) + DAMAGE_1_POINT,
/* 046  ACTION_LOW_KICK,  STANCE_KICK_STANDING_TRANSITION   */ (4 * 32) + DAMAGE_1_POINT,
/* 047  ACTION_LOW_KICK,  STANCE_KICK_MED_END               */ (4 * 42) + DAMAGE_1_POINT,
/* 048  ACTION_LOW_KICK,  STANCE_KICK_LOW_END               */ (4 * 35) + DAMAGE_1_POINT,
/* 049  ACTION_LOW_KICK,  STANCE_KICK_HIGH_END              */ (4 * 42) + DAMAGE_1_POINT,
/* 050  ACTION_LOW_KICK,  STANCE_PUNCH_MED_RH_END           */ (4 * 35) + DAMAGE_1_POINT,
/* 051  ACTION_LOW_KICK,  STANCE_PUNCH_MED_LH_END           */ (4 * 35) + DAMAGE_1_POINT,
/* 052  ACTION_LOW_KICK,  STANCE_PUNCH_HIGH_RH_END          */ (4 * 35) + DAMAGE_1_POINT,
/* 053  ACTION_LOW_KICK,  STANCE_PUNCH_HIGH_LH_END          */ (4 * 35) + DAMAGE_1_POINT,
/* 054  ACTION_LOW_KICK,  STANCE_PUNCH_LOW_RH_END           */ (4 * 35) + DAMAGE_1_POINT,
/* 055  ACTION_LOW_KICK,  STANCE_PUNCH_LOW_LH_END           */ (4 * 35) + DAMAGE_1_POINT,
/* 056  ACTION_LOW_KICK,  STANCE_PUNCH_READY                */ (4 * 35) + DAMAGE_1_POINT,
/* 057  ACTION_LOW_KICK,  STANCE_STANDING_UPRIGHT           */ (4 * 35) + DAMAGE_1_POINT,
/* 058  ACTION_LOW_KICK,  STANCE_SIDLING_1                  */ (4 * 35) + DAMAGE_1_POINT,
/* 059  ACTION_LOW_KICK,  STANCE_SIDLING_2                  */ (4 * 35) + DAMAGE_1_POINT,
/* 060  ACTION_LOW_KICK,  STANCE_SIDLING_3                  */ (4 * 35) + DAMAGE_1_POINT,
/* 061  ACTION_LOW_KICK,  STANCE_SIDLING_4                  */ (4 * 35) + DAMAGE_1_POINT,
/* 062  ACTION_LOW_KICK,  STANCE_SIDLING_5                  */ (4 * 35) + DAMAGE_1_POINT,
/* 063  ACTION_LOW_KICK,  STANCE_SIDLING_6                  */ (4 * 35) + DAMAGE_1_POINT,
/* 064  ACTION_LOW_KICK,  STANCE_SIDLING_7                  */ (4 * 35) + DAMAGE_1_POINT,
/* 065  ACTION_LOW_KICK,  STANCE_RUNNING_1                  */ (4 * 35) + DAMAGE_1_POINT,
/* 066  ACTION_LOW_KICK,  STANCE_RUNNING_2                  */ (4 * 35) + DAMAGE_1_POINT,
/* 067  ACTION_LOW_KICK,  STANCE_RUNNING_3                  */ (4 * 35) + DAMAGE_1_POINT,
/* 068  ACTION_LOW_KICK,  STANCE_RUNNING_4                  */ (4 * 35) + DAMAGE_1_POINT,
/* 069  ACTION_LOW_KICK,  STANCE_RUNNING_5                  */ (4 * 35) + DAMAGE_1_POINT,
/* 070  ACTION_LOW_KICK,  STANCE_RUNNING_6                  */ (4 * 35) + DAMAGE_1_POINT,
/* 071  ACTION_LOW_KICK,  STANCE_RUNNING_7                  */ (4 * 35) + DAMAGE_1_POINT,
/* 072  ACTION_LOW_KICK,  STANCE_RUNNING_8                  */ (4 * 35) + DAMAGE_1_POINT,
/* 073  ACTION_LOW_KICK,  STANCE_RUNNING_STRAIGHTEN_UP      */ (4 * 35) + DAMAGE_1_POINT,
/* 074  ACTION_LOW_KICK,  STANCE_STANDING_UPRIGHT_REV       */ (4 * 35) + DAMAGE_1_POINT,
/* 075  ACTION_LOW_KICK,  STANCE_RUNNING_1_REV              */ (4 * 35) + DAMAGE_1_POINT,
/* 076  ACTION_LOW_KICK,  STANCE_RUNNING_2_REV              */ (4 * 35) + DAMAGE_1_POINT,
/* 077  ACTION_LOW_KICK,  STANCE_RUNNING_3_REV              */ (4 * 35) + DAMAGE_1_POINT,
/* 078  ACTION_LOW_KICK,  STANCE_RUNNING_4_REV              */ (4 * 35) + DAMAGE_1_POINT,
/* 079  ACTION_LOW_KICK,  STANCE_RUNNING_5_REV              */ (4 * 35) + DAMAGE_1_POINT,
/* 080  ACTION_LOW_KICK,  STANCE_RUNNING_6_REV              */ (4 * 35) + DAMAGE_1_POINT,
/* 081  ACTION_LOW_KICK,  STANCE_RUNNING_7_REV              */ (4 * 35) + DAMAGE_1_POINT,
/* 082  ACTION_LOW_KICK,  STANCE_RUNNING_8_REV              */ (4 * 35) + DAMAGE_1_POINT,
/* 083  ACTION_LOW_KICK,  STANCE_RUNNING_STRAIGHTEN_UP_REV  */ (4 * 37) + DAMAGE_1_POINT,


/* ACTION_HIGH_KICK ------------------------------------------------------------------------------------- */

/* 084  ACTION_HIGH_KICK, STANCE_DEFAULT,                   */ (4 * 26) + DAMAGE_3_POINT,
/* 085  ACTION_HIGH_KICK, STANCE_DEFAULT_LEAN_BACK          */ (4 * 27) + DAMAGE_3_POINT,
/* 086  ACTION_HIGH_KICK, STANCE_KICK_READY                 */ (4 * 25) + DAMAGE_3_POINT,
/* 087  ACTION_HIGH_KICK, STANCE_DEFAULT_LEAN_FORWARD       */ (4 * 27) + DAMAGE_3_POINT,
/* 088  ACTION_HIGH_KICK, STANCE_KICK_STANDING_TRANSITION   */ (4 * 27) + DAMAGE_3_POINT,
/* 089  ACTION_HIGH_KICK, STANCE_KICK_MED_END               */ (4 * 44) + DAMAGE_3_POINT,
/* 090  ACTION_HIGH_KICK, STANCE_KICK_LOW_END               */ (4 * 27) + DAMAGE_3_POINT,
/* 091  ACTION_HIGH_KICK, STANCE_KICK_HIGH_END              */ (4 * 44) + DAMAGE_3_POINT,
/* 092  ACTION_HIGH_KICK, STANCE_PUNCH_MED_RH_END           */ (4 * 27) + DAMAGE_3_POINT,
/* 093  ACTION_HIGH_KICK, STANCE_PUNCH_MED_LH_END           */ (4 * 27) + DAMAGE_3_POINT,
/* 094  ACTION_HIGH_KICK, STANCE_PUNCH_HIGH_RH_END          */ (4 * 27) + DAMAGE_3_POINT,
/* 095  ACTION_HIGH_KICK, STANCE_PUNCH_HIGH_LH_END          */ (4 * 27) + DAMAGE_3_POINT,
/* 096  ACTION_HIGH_KICK, STANCE_PUNCH_LOW_RH_END           */ (4 * 27) + DAMAGE_3_POINT,
/* 097  ACTION_HIGH_KICK, STANCE_PUNCH_LOW_LH_END           */ (4 * 27) + DAMAGE_3_POINT,
/* 098  ACTION_HIGH_KICK, STANCE_PUNCH_READY                */ (4 * 27) + DAMAGE_3_POINT,
/* 090  ACTION_HIGH_KICK, STANCE_STANDING_UPRIGHT           */ (4 * 14) + DAMAGE_MAX_POINTS,
/* 100  ACTION_HIGH_KICK, STANCE_SIDLING_1                  */ (4 * 26) + DAMAGE_3_POINT,
/* 101  ACTION_HIGH_KICK, STANCE_SIDLING_2                  */ (4 * 26) + DAMAGE_3_POINT,
/* 102  ACTION_HIGH_KICK, STANCE_SIDLING_3                  */ (4 * 26) + DAMAGE_3_POINT,
/* 103  ACTION_HIGH_KICK, STANCE_SIDLING_4                  */ (4 * 26) + DAMAGE_3_POINT,
/* 104  ACTION_HIGH_KICK, STANCE_SIDLING_5                  */ (4 * 26) + DAMAGE_3_POINT,
/* 105  ACTION_HIGH_KICK, STANCE_SIDLING_6                  */ (4 * 26) + DAMAGE_3_POINT,
/* 106  ACTION_HIGH_KICK, STANCE_SIDLING_7                  */ (4 * 26) + DAMAGE_3_POINT,
/* 107  ACTION_HIGH_KICK, STANCE_RUNNING_1                  */ (4 * 49) + DAMAGE_3_POINT,
/* 108  ACTION_HIGH_KICK, STANCE_RUNNING_2                  */ (4 * 49) + DAMAGE_3_POINT,
/* 109  ACTION_HIGH_KICK, STANCE_RUNNING_3                  */ (4 * 49) + DAMAGE_3_POINT,
/* 110  ACTION_HIGH_KICK, STANCE_RUNNING_4                  */ (4 * 49) + DAMAGE_3_POINT,
/* 111  ACTION_HIGH_KICK, STANCE_RUNNING_5                  */ (4 * 49) + DAMAGE_3_POINT,
/* 112  ACTION_HIGH_KICK, STANCE_RUNNING_6                  */ (4 * 49) + DAMAGE_3_POINT,
/* 113  ACTION_HIGH_KICK, STANCE_RUNNING_7                  */ (4 * 49) + DAMAGE_3_POINT,
/* 114  ACTION_HIGH_KICK, STANCE_RUNNING_8                  */ (4 * 49) + DAMAGE_3_POINT,
/* 115  ACTION_HIGH_KICK, STANCE_RUNNING_STRAIGHTEN_UP      */ (4 * 49) + DAMAGE_3_POINT,
/* 116  ACTION_HIGH_KICK, STANCE_STANDING_UPRIGHT_REV       */ (4 * 27) + DAMAGE_3_POINT,
/* 117  ACTION_HIGH_KICK, STANCE_RUNNING_1_REV              */ (4 * 27) + DAMAGE_3_POINT,
/* 118  ACTION_HIGH_KICK, STANCE_RUNNING_2_REV              */ (4 * 27) + DAMAGE_3_POINT,
/* 119  ACTION_HIGH_KICK, STANCE_RUNNING_3_REV              */ (4 * 27) + DAMAGE_3_POINT,
/* 120  ACTION_HIGH_KICK, STANCE_RUNNING_4_REV              */ (4 * 27) + DAMAGE_3_POINT,
/* 121  ACTION_HIGH_KICK, STANCE_RUNNING_5_REV              */ (4 * 27) + DAMAGE_3_POINT,
/* 122  ACTION_HIGH_KICK, STANCE_RUNNING_6_REV              */ (4 * 27) + DAMAGE_3_POINT,
/* 123  ACTION_HIGH_KICK, STANCE_RUNNING_7_REV              */ (4 * 27) + DAMAGE_3_POINT,
/* 124  ACTION_HIGH_KICK, STANCE_RUNNING_8_REV              */ (4 * 27) + DAMAGE_3_POINT,
/* 125  ACTION_HIGH_KICK, STANCE_RUNNING_STRAIGHTEN_UP_REV  */ (4 * 27) + DAMAGE_3_POINT,

/* ACTION_MED_PUNCH ------------------------------------------------------------------------------------- */

/* 126  ACTION_MED_PUNCH,   STANCE_DEFAULT,                   */ (4 * 31) + DAMAGE_2_POINT,
/* 127  ACTION_MED_PUNCH,   STANCE_DEFAULT_LEAN_BACK          */ (4 * 31) + DAMAGE_2_POINT,
/* 128  ACTION_MED_PUNCH,   STANCE_KICK_READY                 */ (4 * 25) + DAMAGE_2_POINT,
/* 129  ACTION_MED_PUNCH,   STANCE_DEFAULT_LEAN_FORWARD       */ (4 * 31) + DAMAGE_2_POINT,
/* 130  ACTION_MED_PUNCH,   STANCE_KICK_STANDING_TRANSITION   */ (4 * 31) + DAMAGE_2_POINT,
/* 131  ACTION_MED_PUNCH,   STANCE_KICK_MED_END               */ (4 * 44) + DAMAGE_2_POINT,
/* 132  ACTION_MED_PUNCH,   STANCE_KICK_LOW_END               */ (4 * 31) + DAMAGE_2_POINT,
/* 133  ACTION_MED_PUNCH,   STANCE_KICK_HIGH_END              */ (4 * 44) + DAMAGE_2_POINT,
/* 134  ACTION_MED_PUNCH,   STANCE_PUNCH_MED_RH_END           */ (4 * 31) + DAMAGE_2_POINT,
/* 135  ACTION_MED_PUNCH,   STANCE_PUNCH_MED_LH_END           */ (4 * 31) + DAMAGE_2_POINT,
/* 136  ACTION_MED_PUNCH,   STANCE_PUNCH_HIGH_RH_END          */ (4 * 31) + DAMAGE_2_POINT,
/* 137  ACTION_MED_PUNCH,   STANCE_PUNCH_HIGH_LH_END          */ (4 * 31) + DAMAGE_2_POINT,
/* 138  ACTION_MED_PUNCH,   STANCE_PUNCH_LOW_RH_END           */ (4 * 31) + DAMAGE_2_POINT,
/* 139  ACTION_MED_PUNCH,   STANCE_PUNCH_LOW_LH_END           */ (4 * 31) + DAMAGE_2_POINT,
/* 140  ACTION_MED_PUNCH,   STANCE_PUNCH_READY                */ (4 * 26) + DAMAGE_2_POINT,
/* 141  ACTION_MED_PUNCH,   STANCE_STANDING_UPRIGHT           */ (4 * 31) + DAMAGE_2_POINT,
/* 142  ACTION_MED_PUNCH,   STANCE_SIDLING_1                  */ (4 * 31) + DAMAGE_2_POINT,
/* 143  ACTION_MED_PUNCH,   STANCE_SIDLING_2                  */ (4 * 31) + DAMAGE_2_POINT,
/* 144  ACTION_MED_PUNCH,   STANCE_SIDLING_3                  */ (4 * 31) + DAMAGE_2_POINT,
/* 145  ACTION_MED_PUNCH,   STANCE_SIDLING_4                  */ (4 * 31) + DAMAGE_2_POINT,
/* 146  ACTION_MED_PUNCH,   STANCE_SIDLING_5                  */ (4 * 31) + DAMAGE_2_POINT,
/* 147  ACTION_MED_PUNCH,   STANCE_SIDLING_6                  */ (4 * 31) + DAMAGE_2_POINT,
/* 148  ACTION_MED_PUNCH,   STANCE_SIDLING_7                  */ (4 * 31) + DAMAGE_2_POINT,
/* 149  ACTION_MED_PUNCH,   STANCE_RUNNING_1                  */ (4 * 31) + DAMAGE_2_POINT,
/* 150  ACTION_MED_PUNCH,   STANCE_RUNNING_2                  */ (4 * 31) + DAMAGE_2_POINT,
/* 151  ACTION_MED_PUNCH,   STANCE_RUNNING_3                  */ (4 * 31) + DAMAGE_2_POINT,
/* 152  ACTION_MED_PUNCH,   STANCE_RUNNING_4                  */ (4 * 31) + DAMAGE_2_POINT,
/* 153  ACTION_MED_PUNCH,   STANCE_RUNNING_5                  */ (4 * 31) + DAMAGE_2_POINT,
/* 154  ACTION_MED_PUNCH,   STANCE_RUNNING_6                  */ (4 * 31) + DAMAGE_2_POINT,
/* 155  ACTION_MED_PUNCH,   STANCE_RUNNING_7                  */ (4 * 31) + DAMAGE_2_POINT,
/* 156  ACTION_MED_PUNCH,   STANCE_RUNNING_8                  */ (4 * 31) + DAMAGE_2_POINT,
/* 157  ACTION_MED_PUNCH,   STANCE_RUNNING_STRAIGHTEN_UP      */ (4 * 31) + DAMAGE_2_POINT,
/* 158  ACTION_MED_PUNCH,   STANCE_STANDING_UPRIGHT_REV       */ (4 * 31) + DAMAGE_2_POINT,
/* 159  ACTION_MED_PUNCH,   STANCE_RUNNING_1_REV              */ (4 * 31) + DAMAGE_2_POINT,
/* 160  ACTION_MED_PUNCH,   STANCE_RUNNING_2_REV              */ (4 * 31) + DAMAGE_2_POINT,
/* 161  ACTION_MED_PUNCH,   STANCE_RUNNING_3_REV              */ (4 * 31) + DAMAGE_2_POINT,
/* 162  ACTION_MED_PUNCH,   STANCE_RUNNING_4_REV              */ (4 * 31) + DAMAGE_2_POINT,
/* 163  ACTION_MED_PUNCH,   STANCE_RUNNING_5_REV              */ (4 * 31) + DAMAGE_2_POINT,
/* 164  ACTION_MED_PUNCH,   STANCE_RUNNING_6_REV              */ (4 * 31) + DAMAGE_2_POINT,
/* 165  ACTION_MED_PUNCH,   STANCE_RUNNING_7_REV              */ (4 * 31) + DAMAGE_2_POINT,
/* 166  ACTION_MED_PUNCH,   STANCE_RUNNING_8_REV              */ (4 * 31) + DAMAGE_2_POINT,
/* 167  ACTION_MED_PUNCH,   STANCE_RUNNING_STRAIGHTEN_UP_REV  */ (4 * 31) + DAMAGE_2_POINT,


/* ACTION_HIGH_PUNCH ------------------------------------------------------------------------------------- */

/* 168  ACTION_HIGH_PUNCH,  STANCE_DEFAULT,                   */ (4 * 30) + DAMAGE_3_POINT,
/* 169  ACTION_HIGH_PUNCH,  STANCE_DEFAULT_LEAN_BACK          */ (4 * 31) + DAMAGE_3_POINT,
/* 170  ACTION_HIGH_PUNCH,  STANCE_KICK_READY                 */ (4 * 17) + DAMAGE_3_POINT,
/* 171  ACTION_HIGH_PUNCH,  STANCE_DEFAULT_LEAN_FORWARD       */ (4 * 31) + DAMAGE_3_POINT,
/* 172  ACTION_HIGH_PUNCH,  STANCE_KICK_STANDING_TRANSITION   */ (4 * 31) + DAMAGE_3_POINT,
/* 173  ACTION_HIGH_PUNCH,  STANCE_KICK_MED_END               */ (4 * 44) + DAMAGE_3_POINT,
/* 174  ACTION_HIGH_PUNCH,  STANCE_KICK_LOW_END               */ (4 * 31) + DAMAGE_3_POINT,
/* 175  ACTION_HIGH_PUNCH,  STANCE_KICK_HIGH_END              */ (4 * 44) + DAMAGE_3_POINT,
/* 176  ACTION_HIGH_PUNCH,  STANCE_PUNCH_MED_RH_END           */ (4 * 31) + DAMAGE_3_POINT,
/* 177  ACTION_HIGH_PUNCH,  STANCE_PUNCH_MED_LH_END           */ (4 * 31) + DAMAGE_3_POINT,
/* 178  ACTION_HIGH_PUNCH,  STANCE_PUNCH_HIGH_RH_END          */ (4 * 31) + DAMAGE_3_POINT,
/* 179  ACTION_HIGH_PUNCH,  STANCE_PUNCH_HIGH_LH_END          */ (4 * 31) + DAMAGE_3_POINT,
/* 180  ACTION_HIGH_PUNCH,  STANCE_PUNCH_LOW_RH_END           */ (4 * 31) + DAMAGE_3_POINT,
/* 181  ACTION_HIGH_PUNCH,  STANCE_PUNCH_LOW_LH_END           */ (4 * 31) + DAMAGE_3_POINT,
/* 182  ACTION_HIGH_PUNCH,  STANCE_PUNCH_READY                */ (4 * 28) + DAMAGE_3_POINT,
/* 183  ACTION_HIGH_PUNCH,  STANCE_STANDING_UPRIGHT           */ (4 * 31) + DAMAGE_3_POINT,
/* 184  ACTION_HIGH_PUNCH,  STANCE_SIDLING_1                  */ (4 * 31) + DAMAGE_3_POINT,
/* 185  ACTION_HIGH_PUNCH,  STANCE_SIDLING_2                  */ (4 * 31) + DAMAGE_3_POINT,
/* 186  ACTION_HIGH_PUNCH,  STANCE_SIDLING_3                  */ (4 * 31) + DAMAGE_3_POINT,
/* 187  ACTION_HIGH_PUNCH,  STANCE_SIDLING_4                  */ (4 * 31) + DAMAGE_3_POINT,
/* 188  ACTION_HIGH_PUNCH,  STANCE_SIDLING_5                  */ (4 * 31) + DAMAGE_3_POINT,
/* 189  ACTION_HIGH_PUNCH,  STANCE_SIDLING_6                  */ (4 * 31) + DAMAGE_3_POINT,
/* 190  ACTION_HIGH_PUNCH,  STANCE_SIDLING_7                  */ (4 * 31) + DAMAGE_3_POINT,
/* 191  ACTION_HIGH_PUNCH,  STANCE_RUNNING_1                  */ (4 * 31) + DAMAGE_3_POINT,
/* 192  ACTION_HIGH_PUNCH,  STANCE_RUNNING_2                  */ (4 * 31) + DAMAGE_3_POINT,
/* 193  ACTION_HIGH_PUNCH,  STANCE_RUNNING_3                  */ (4 * 31) + DAMAGE_3_POINT,
/* 194  ACTION_HIGH_PUNCH,  STANCE_RUNNING_4                  */ (4 * 31) + DAMAGE_3_POINT,
/* 195  ACTION_HIGH_PUNCH,  STANCE_RUNNING_5                  */ (4 * 31) + DAMAGE_3_POINT,
/* 196  ACTION_HIGH_PUNCH,  STANCE_RUNNING_6                  */ (4 * 31) + DAMAGE_3_POINT,
/* 197  ACTION_HIGH_PUNCH,  STANCE_RUNNING_7                  */ (4 * 31) + DAMAGE_3_POINT,
/* 198  ACTION_HIGH_PUNCH,  STANCE_RUNNING_8                  */ (4 * 31) + DAMAGE_3_POINT,
/* 199  ACTION_HIGH_PUNCH,  STANCE_RUNNING_STRAIGHTEN_UP      */ (4 * 31) + DAMAGE_3_POINT,
/* 200  ACTION_HIGH_PUNCH,  STANCE_STANDING_UPRIGHT_REV       */ (4 * 31) + DAMAGE_3_POINT,
/* 201  ACTION_HIGH_PUNCH,  STANCE_RUNNING_1_REV              */ (4 * 31) + DAMAGE_3_POINT,
/* 202  ACTION_HIGH_PUNCH,  STANCE_RUNNING_2_REV              */ (4 * 31) + DAMAGE_3_POINT,
/* 203  ACTION_HIGH_PUNCH,  STANCE_RUNNING_3_REV              */ (4 * 31) + DAMAGE_3_POINT,
/* 204  ACTION_HIGH_PUNCH,  STANCE_RUNNING_4_REV              */ (4 * 31) + DAMAGE_3_POINT,
/* 205  ACTION_HIGH_PUNCH,  STANCE_RUNNING_5_REV              */ (4 * 31) + DAMAGE_3_POINT,
/* 206  ACTION_HIGH_PUNCH,  STANCE_RUNNING_6_REV              */ (4 * 31) + DAMAGE_3_POINT,
/* 207  ACTION_HIGH_PUNCH,  STANCE_RUNNING_7_REV              */ (4 * 31) + DAMAGE_3_POINT,
/* 208  ACTION_HIGH_PUNCH,  STANCE_RUNNING_8_REV              */ (4 * 31) + DAMAGE_3_POINT,
/* 209  ACTION_HIGH_PUNCH,  STANCE_RUNNING_STRAIGHTEN_UP_REV  */ (4 * 31) + DAMAGE_3_POINT,


/* ACTION_LOW_PUNCH ------------------------------------------------------------------------------------- */

/* 210  ACTION_LOW_PUNCH,   STANCE_DEFAULT,                   */ (4 * 29) + DAMAGE_1_POINT,
/* 211  ACTION_LOW_PUNCH,   STANCE_DEFAULT_LEAN_BACK          */ (4 * 29) + DAMAGE_1_POINT,
/* 212  ACTION_LOW_PUNCH,   STANCE_KICK_READY                 */ (4 * 27) + DAMAGE_1_POINT,
/* 213  ACTION_LOW_PUNCH,   STANCE_DEFAULT_LEAN_FORWARD       */ (4 * 29) + DAMAGE_1_POINT,
/* 214  ACTION_LOW_PUNCH,   STANCE_KICK_STANDING_TRANSITION   */ (4 * 29) + DAMAGE_1_POINT,
/* 215  ACTION_LOW_PUNCH,   STANCE_KICK_MED_END               */ (4 * 44) + DAMAGE_1_POINT,
/* 216  ACTION_LOW_PUNCH,   STANCE_KICK_LOW_END               */ (4 * 29) + DAMAGE_1_POINT,
/* 217  ACTION_LOW_PUNCH,   STANCE_KICK_HIGH_END              */ (4 * 44) + DAMAGE_1_POINT,
/* 218  ACTION_LOW_PUNCH,   STANCE_PUNCH_MED_RH_END           */ (4 * 29) + DAMAGE_1_POINT,
/* 219  ACTION_LOW_PUNCH,   STANCE_PUNCH_MED_LH_END           */ (4 * 29) + DAMAGE_1_POINT,
/* 220  ACTION_LOW_PUNCH,   STANCE_PUNCH_HIGH_RH_END          */ (4 * 29) + DAMAGE_1_POINT,
/* 221  ACTION_LOW_PUNCH,   STANCE_PUNCH_HIGH_LH_END          */ (4 * 29) + DAMAGE_1_POINT,
/* 222  ACTION_LOW_PUNCH,   STANCE_PUNCH_LOW_RH_END           */ (4 * 29) + DAMAGE_1_POINT,
/* 223  ACTION_LOW_PUNCH,   STANCE_PUNCH_LOW_LH_END           */ (4 * 29) + DAMAGE_1_POINT,
/* 224  ACTION_LOW_PUNCH,   STANCE_PUNCH_READY                */ (4 * 25) + DAMAGE_1_POINT,
/* 225  ACTION_LOW_PUNCH,   STANCE_STANDING_UPRIGHT           */ (4 * 29) + DAMAGE_1_POINT,
/* 226  ACTION_LOW_PUNCH,   STANCE_SIDLING_1                  */ (4 * 29) + DAMAGE_1_POINT,
/* 227  ACTION_LOW_PUNCH,   STANCE_SIDLING_2                  */ (4 * 29) + DAMAGE_1_POINT,
/* 228  ACTION_LOW_PUNCH,   STANCE_SIDLING_3                  */ (4 * 29) + DAMAGE_1_POINT,
/* 229  ACTION_LOW_PUNCH,   STANCE_SIDLING_4                  */ (4 * 29) + DAMAGE_1_POINT,
/* 230  ACTION_LOW_PUNCH,   STANCE_SIDLING_5                  */ (4 * 29) + DAMAGE_1_POINT,
/* 231  ACTION_LOW_PUNCH,   STANCE_SIDLING_6                  */ (4 * 29) + DAMAGE_1_POINT,
/* 232  ACTION_LOW_PUNCH,   STANCE_SIDLING_7                  */ (4 * 29) + DAMAGE_1_POINT,
/* 233  ACTION_LOW_PUNCH,   STANCE_RUNNING_1                  */ (4 * 29) + DAMAGE_1_POINT,
/* 234  ACTION_LOW_PUNCH,   STANCE_RUNNING_2                  */ (4 * 29) + DAMAGE_1_POINT,
/* 235  ACTION_LOW_PUNCH,   STANCE_RUNNING_3                  */ (4 * 29) + DAMAGE_1_POINT,
/* 236  ACTION_LOW_PUNCH,   STANCE_RUNNING_4                  */ (4 * 29) + DAMAGE_1_POINT,
/* 237  ACTION_LOW_PUNCH,   STANCE_RUNNING_5                  */ (4 * 29) + DAMAGE_1_POINT,
/* 238  ACTION_LOW_PUNCH,   STANCE_RUNNING_6                  */ (4 * 29) + DAMAGE_1_POINT,
/* 239  ACTION_LOW_PUNCH,   STANCE_RUNNING_7                  */ (4 * 29) + DAMAGE_1_POINT,
/* 240  ACTION_LOW_PUNCH,   STANCE_RUNNING_8                  */ (4 * 29) + DAMAGE_1_POINT,
/* 241  ACTION_LOW_PUNCH,   STANCE_RUNNING_STRAIGHTEN_UP      */ (4 * 29) + DAMAGE_1_POINT,
/* 242  ACTION_LOW_PUNCH,   STANCE_STANDING_UPRIGHT_REV       */ (4 * 29) + DAMAGE_1_POINT,
/* 243  ACTION_LOW_PUNCH,   STANCE_RUNNING_1_REV              */ (4 * 29) + DAMAGE_1_POINT,
/* 244  ACTION_LOW_PUNCH,   STANCE_RUNNING_2_REV              */ (4 * 29) + DAMAGE_1_POINT,
/* 245  ACTION_LOW_PUNCH,   STANCE_RUNNING_3_REV              */ (4 * 29) + DAMAGE_1_POINT,
/* 246  ACTION_LOW_PUNCH,   STANCE_RUNNING_4_REV              */ (4 * 29) + DAMAGE_1_POINT,
/* 247  ACTION_LOW_PUNCH,   STANCE_RUNNING_5_REV              */ (4 * 29) + DAMAGE_1_POINT,
/* 248  ACTION_LOW_PUNCH,   STANCE_RUNNING_6_REV              */ (4 * 29) + DAMAGE_1_POINT,
/* 249  ACTION_LOW_PUNCH,   STANCE_RUNNING_7_REV              */ (4 * 29) + DAMAGE_1_POINT,
/* 250  ACTION_LOW_PUNCH,   STANCE_RUNNING_8_REV              */ (4 * 29) + DAMAGE_1_POINT,
/* 251  ACTION_LOW_PUNCH,   STANCE_RUNNING_STRAIGHTEN_UP_REV  */ (4 * 29) + DAMAGE_1_POINT,


};


template<typename T> T absT(const T & v) {
  
    return (v < 0) ? -v : v;
  
  }

/* ----------------------------------------------------------------------------
 *  Draw a horizontal dotted line. 
 *  
 *  So much nicer than a solid line!
 */
void drawHorizontalDottedLine(int x1, int x2, int y) {
    
  for (int z = x1; z <= x2; z+=2) {
    arduboy.drawPixel(z, y, WHITE);
  }
  
}


uint8_t inStrikingRange(uint8_t action, int16_t attackerXPos, uint8_t targetType, uint8_t targetStance, int16_t targetXPos) {

  int16_t distBetween = absT(targetXPos - attackerXPos);

  if (targetType == ENEMY_TYPE_PERSON) {

    uint16_t pos = (action * ACTIONS_NUMBER_OF_STANCES) + targetStance;
    uint8_t distance = pgm_read_byte(&actions_distance[ pos ]) >> 2;
    uint8_t damage = pgm_read_byte(&actions_distance[ pos ]) & 0x03;

    #ifdef DEBUG_STRIKING_RANGE
      _distBetween = distBetween;
      _distTest = distance;
      _pos = pos;
      _targetStance = targetStance;
      _action = action;
    #endif

    #ifdef DEBUG_HITS
      _distBetween = distBetween;
      _distTest = distance;
      _pos = pos;
      _targetStance = targetStance;
    #endif

    #ifdef DEBUG_SERIAL
      Serial.print("inStrikingRange: "); 
      Serial.print(pos); 
      Serial.print(" "); 
      Serial.print(distance); 
      Serial.print(" "); 
      Serial.print(distBetween); 
      Serial.print(" "); 
      Serial.println(damage); 
    #endif

    return ((distBetween <= distance) ? (damage == 0 ? 255 : damage) : 0);
    
  }
  else {
    

    // Attacking eagle ..

    if (action != ACTION_LOW_PUNCH) {
      return (distBetween > 35 && distBetween < 43 ? DAMAGE_3_POINT : 0); 
    }
    else {
      return (distBetween > 30 && distBetween < 36 ? DAMAGE_3_POINT : 0); 
    }


  }

  return 0;

}


void returnFromAction(uint8_t action, uint8_t returnAction) {

  switch (returnAction) {  // Return to the default or kick ready position?
      
    case ACTION_RETURN_TO_DEFAULT:
    
      switch (action) {
        
        case ACTION_HIGH_KICK:
        case ACTION_MED_KICK:
        case ACTION_LOW_KICK:
          enemyStack.push(STANCE_DEFAULT, STANCE_DEFAULT_LEAN_FORWARD);
          break;

        case ACTION_HIGH_PUNCH:
        case ACTION_MED_PUNCH:
        case ACTION_LOW_PUNCH:
          enemyStack.push(STANCE_DEFAULT, STANCE_DEFAULT_LEAN_FORWARD, STANCE_PUNCH_READY);
          break;

      }

      break;

    case ACTION_RETURN_TO_ACTION_READY:

      switch (action) {
      
        case ACTION_HIGH_KICK:
        case ACTION_MED_KICK:
        case ACTION_LOW_KICK:
          enemyStack.push(STANCE_KICK_READY);
          break;

        case ACTION_HIGH_PUNCH:
        case ACTION_MED_PUNCH:
        case ACTION_LOW_PUNCH:
          enemyStack.push(STANCE_PUNCH_READY);
          break;

      }

      break;

  }

}


uint8_t getActionFromStance(uint8_t stance) {
  
  switch (stance) {

    case STANCE_KICK_MED_END:
    case STANCE_KICK_HIGH_END:
    case STANCE_KICK_LOW_END:
      return stance - STANCE_KICK_MED_END;

    case STANCE_PUNCH_MED_RH_END:
    case STANCE_PUNCH_MED_LH_END:
      return ACTION_MED_PUNCH;

    case STANCE_PUNCH_HIGH_RH_END:
    case STANCE_PUNCH_HIGH_LH_END:
      return ACTION_HIGH_PUNCH;

    case STANCE_PUNCH_LOW_RH_END:
    case STANCE_PUNCH_LOW_LH_END:
      return ACTION_LOW_PUNCH;

    default:
      return 9;

  }

}