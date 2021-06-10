#include "Words.hpp"

const Words words[Words::wordsCount] PROGMEM = {
    {"OASIS"},  /* 00 */
    {"SODA"},   /* 01 */
    {"SAUNA"},  /* 02 */
    {"SUMO"},   /* 03 */
    {"SIR"},    /* 04 */
    {"SEAL"},   /* 05 */
    {"SAGE"},   /* 06 */
    {"SOCK"},   /* 07 */
    {"SOFA"},   /* 08 */
    {"SOAP"},   /* 09 */
    {"DICE"},   /* 10 */
    {"DAD"},    /* 11 */
    {"TUNA"},   /* 12 */
    {"DAM"},    /* 13 */
    {"TREE"},   /* 14 */
    {"DOLL"},   /* 15 */
    {"DISH"},   /* 16 */
    {"DOG"},    /* 17 */
    {"TV"},     /* 18 */
    {"TAPE"},   /* 19 */
    {"NOSE"},   /* 20 */
    {"NET"},    /* 21 */
    {"NUN"},    /* 22 */
    {"ANIME"},  /* 23 */
    {"NERO"},   /* 24 */
    {"NAIL"},   /* 25 */
    {"HINGE"},  /* 26 */
    {"NECK"},   /* 27 */
    {"NOVA"},   /* 28 */
    {"NOOB"},   /* 29 */
    {"MOUSE"},  /* 30 */
    {"MUD"},    /* 31 */
    {"MOON"},   /* 32 */
    {"MIME"},   /* 33 */
    {"MARE"},   /* 34 */
    {"MULE"},   /* 35 */
    {"IMAGE"},  /* 36 */
    {"MUG"},    /* 37 */
    {"MAFIA"},  /* 38 */
    {"MAP"},    /* 39 */
    {"ROSE"},   /* 40 */
    {"RAT"},    /* 41 */
    {"ARENA"},  /* 42 */
    {"WORM"},   /* 43 */
    {"AURORA"}, /* 44 */
    {"RAIL"},   /* 45 */
    {"RAGE"},   /* 46 */
    {"RAKE"},   /* 47 */
    {"ROOF"},   /* 48 */
    {"ROPE"},   /* 49 */
    {"LASSO"},  /* 50 */
    {"LED"},    /* 51 */
    {"LION"},   /* 52 */
    {"LIME"},   /* 53 */
    {"LYRA"},   /* 54 */
    {"LILY"},   /* 55 */
    {"ALGAE"},  /* 56 */
    {"LEG"},    /* 57 */
    {"ELF"},    /* 58 */
    {"LOOP"},   /* 59 */
    {"CHEESE"}, /* 60 */
    {"JET"},    /* 61 */
    {"CHAIN"},  /* 62 */
    {"GYM"},    /* 63 */
    {"CHAIR"},  /* 64 */
    {"CELLO"},  /* 65 */
    {"JUDGE"},  /* 66 */
    {"JOGA"},   /* 67 */
    {"CHEF"},   /* 68 */
    {"SHEEP"},  /* 69 */
    {"GOOSE"},  /* 70 */
    {"CAT"},    /* 71 */
    {"GNU"},    /* 72 */
    {"COMB"},   /* 73 */
    {"CURRY"},  /* 74 */
    {"COAL"},   /* 75 */
    {"CAGE"},   /* 76 */
    {"CAKE"},   /* 77 */
    {"COFFEE"}, /* 78 */
    {"CUP"},    /* 79 */
    {"VASE"},   /* 80 */
    {"FOOT"},   /* 81 */
    {"OVEN"},   /* 82 */
    {"FOAM"},   /* 83 */
    {"IVORY"},  /* 84 */
    {"VILLA"},  /* 85 */
    {"FISH"},   /* 86 */
    {"FIG"},    /* 87 */
    {"FIFA"},   /* 88 */
    {"FBI"},    /* 89 */
    {"PIZZA"},  /* 90 */
    {"BAT"},    /* 91 */
    {"PONY"},   /* 92 */
    {"PUMA"},   /* 93 */
    {"BOAR"},   /* 94 */
    {"APPLE"},  /* 95 */
    {"BEACH"},  /* 96 */
    {"BIKE"},   /* 97 */
    {"BEEF"},   /* 98 */
    {"POPE"},   /* 99 */
};

const __FlashStringHelper* Words::word() const {
  return reinterpret_cast<const __FlashStringHelper*>(_word);
}

const __FlashStringHelper* Words::word(const uint8_t id) {
  return words[id].word();
}
