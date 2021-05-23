#include "Buildings.hpp"
#include "Bitmaps.hpp"

const Building buildings[static_cast<uint8_t>(Building::IDs::count)] PROGMEM = {
    /* clang-format off */
    /* name,    bitmap,    jobs,  cost, w, h, maintenance, profit, upgrade */
    {"CLEAR",   bmp_empty,    0,    50, 1, 1, 0,           0,      Building::IDs::empty},
    {"PAUSE",   bmp_empty,    0,     0, 1, 1, 0,           0,      Building::IDs::empty},
    {"UPGRADE", bmp_empty,    0,     0, 1, 1, 0,           0,      Building::IDs::empty},
    {"HOUSE",   bmp_house,    0,   100, 2, 2, 1,           0,      Building::IDs::house2},
    {"WATER",   bmp_water,    0,   250, 1, 3, 0,           1,      Building::IDs::empty},
    {"SALOON",  bmp_saloon,   8,   500, 3, 2, 0,           1,      Building::IDs::saloon2},
    {"FARM",    bmp_farm,     8,   500, 4, 2, 0,          10,      Building::IDs::farm2},
    {"MILL",    bmp_mill,     1,   750, 1, 3, 0,           1,      Building::IDs::empty},
    {"MINE",    bmp_mine,    30,  2000, 3, 3, 0,          30,      Building::IDs::empty},
    {"CHURCH",  bmp_church,   1,  2000, 3, 2, 2,           0,      Building::IDs::church2},
    {"SHERIFF", bmp_sheriff,  2,  1500, 2, 2, 2,           0,      Building::IDs::sheriff2},
    {"BANK",    bmp_bank,     4,  2500, 2, 2, 5,           0,      Building::IDs::bank2},
    {"STABLE",  bmp_stable,   2,  1500, 4, 2, 4,           0,      Building::IDs::stable2},
    {"WEED",    bmp_weed,     0,    10, 1, 1, 0,           0,      Building::IDs::empty},
    {"CACTUS",  bmp_cactus,   0,    15, 1, 1, 0,           0,      Building::IDs::empty},
    {"TREE",    bmp_tree,     0,    30, 2, 2, 0,           0,      Building::IDs::empty},
    {"TTM",     bmp_totem,    0,     0, 1, 3, 0,           0,      Building::IDs::empty},
    {"HS2",     bmp_house2,   1,   250, 2, 2, 1,           0,      Building::IDs::empty},
    {"BNK2",    bmp_bank2,    7,  5000, 2, 3, 5,           0,      Building::IDs::empty},
    {"SRFF2",   bmp_sheriff2, 2,  3000, 2, 2, 2,           0,      Building::IDs::empty},
    {"CRC2",    bmp_church2,  2,  1500, 3, 3, 2,           0,      Building::IDs::empty},
    {"SLN2",    bmp_saloon2, 18,  1000, 3, 3, 0,           5,      Building::IDs::empty},
    {"STBL2",   bmp_stable2,  5,  1000, 4, 3, 4,           0,      Building::IDs::empty},
    {"FRM2",    bmp_farm2,   18,  1000, 4, 3, 0,          20,      Building::IDs::empty},
    /* clang-format on */
};

const __FlashStringHelper* Building::name() const {
  return reinterpret_cast<const __FlashStringHelper*>(_name);
}

const uint8_t Building::jobs() const {
  return pgm_read_byte(&_jobs);
}

const uint16_t Building::cost() const {
  return pgm_read_word(&_cost);
}

const uint8_t Building::width() const {
  return pgm_read_byte(&_width);
}

const uint8_t Building::height() const {
  return pgm_read_byte(&_height);
}

const uint8_t Building::maintenance() const {
  return pgm_read_byte(&_maintenance);
}

const uint8_t Building::profit() const {
  return pgm_read_byte(&_profit);
}

const uint8_t* Building::bitmap() const {
  return (const uint8_t*)pgm_read_ptr(&_bitmap);
}

const Building::IDs Building::upgrade() const {
  return static_cast<Building::IDs>(pgm_read_byte(&_upgrade));
}

const __FlashStringHelper* Building::name(const uint8_t id) {
  return buildings[id].name();
}

const uint8_t Building::jobs(const uint8_t id) {
  return buildings[id].jobs();
}

const uint16_t Building::cost(const uint8_t id) {
  return buildings[id].cost();
}

const uint8_t Building::width(const uint8_t id) {
  return buildings[id].width();
}

const uint8_t Building::height(const uint8_t id) {
  return buildings[id].height();
}

const uint8_t Building::maintenance(const uint8_t id) {
  return buildings[id].maintenance();
}

const uint8_t Building::profit(const uint8_t id) {
  return buildings[id].profit();
}

const uint8_t* Building::bitmap(const uint8_t id) {
  return buildings[id].bitmap();
}

const Building::IDs Building::upgrade(const uint8_t id) {
  return buildings[id].upgrade();
}

const char textHouse[] PROGMEM =
    "HOUSES ARE\n"    /**/
    "NECESSARY TO\n"  /**/
    "INCREASE THE\n"  /**/
    "POPULATION.\n\n" /**/
    "HOUSES NEAR\n"   /**/
    "VEGETATION\n"    /**/
    "ATTRACT MORE\n"  /**/
    "PEOPLE.\n";      /**/
const char textFarm[] PROGMEM =
    "WORKERS ARE\n" /**/
    "ALLOCATED\n"   /**/
    "TO FARMS\n"    /**/
    "FIRST.\n";     /**/
const char textWater[] PROGMEM =
    "WATER TOWERS\n" /**/
    "CAN HELP\n"     /**/
    "IRRIGATING\n"   /**/
    "THE FIELDS\n"   /**/
    "IN NEARBY\n"    /**/
    "FARMS AS\n"     /**/
    "WELL.\n";       /**/
const char textSaloon[] PROGMEM =
    "SALOONS\n"       /**/
    "INCREASE THE\n"  /**/
    "HAPPINES OF\n"   /**/
    "LAGUNITIANS.\n"  /**/
    "\nPRESS THE B\n" /**/
    "BUTTON TO\n"     /**/
    "CHECK THE\n"     /**/
    "STATISTIC\n"     /**/
    "REPORT!\n";      /**/
const char textMine[] PROGMEM =
    "MINES ARE\n"     /**/
    "GREAT TO\n"      /**/
    "INCREASE THE\n"  /**/
    "AVAILABLE\n"     /**/
    "JOBS AND THE\n"  /**/
    "INCOME, BUT\n"   /**/
    "THEY MAY\n"      /**/
    "HURT THE\n"      /**/
    "ENVIRONMENT.\n"; /**/
const char textBank[] PROGMEM =
    "EACH BANK\n"    /**/
    "INCREASES\n"    /**/
    "THE MAXIMUM\n"  /**/
    "MONEY YOU\n"    /**/
    "CAN STORE,\n"   /**/
    "BUT THEY\n"     /**/
    "ATTRACT MORE\n" /**/
    "THIEVES.\n";    /**/
const char textSheriff[] PROGMEM =
    "REMEMBER TO\n"  /**/
    "CHECK THE\n"    /**/
    "STATISTICS\n"   /**/
    "TO KNOW WHEN\n" /**/
    "SAFETY DROPS\n" /**/
    "LOW!\n";        /**/
const char textMill[] PROGMEM =
    "MILLS REFINE\n" /**/
    "SEEDS FROM\n"   /**/
    "NEARBY FARMS\n" /**/
    "INTO FLOWER,\n" /**/
    "SO THEY ARE\n"  /**/
    "QUITE MORE\n"   /**/
    "EFFECTIVE\n"    /**/
    "WHEN PLACED\n"  /**/
    "NEAR FARMS.\n"; /**/
const char textChurch[] PROGMEM =
    "CHURCHS KEEP\n" /**/
    "LAGUNITAS\n"    /**/
    "TOGETHER,\n"    /**/
    "BUT THE\n"      /**/
    "CLERICS DO\n"   /**/
    "NOT LIKE\n"     /**/
    "TO BE AROUND\n" /**/
    "OTHER NOISY\n"  /**/
    "BUILDINGS.\n";  /**/
const char textEmpty[] PROGMEM =
    "NOW YOU CAN\n"   /**/
    "CLEAR ANY\n"     /**/
    "TYPE OF\n"       /**/
    "VEGETATION\n"    /**/
    "AND ANY TYPE\n"  /**/
    "OF BUILDING.\n"; /**/
const char textTree[] PROGMEM =
    "TREES AROUND\n" /**/
    "CHURCHES AND\n" /**/
    "HOUSES MAKE\n"  /**/
    "THEM MORE\n"    /**/
    "ATTRACTIVE.\n"; /**/
const char textCactus[] PROGMEM =
    "IS CACTUSES\n" /**/
    "OR CACTI?\n";  /**/
const char textWeed[] PROGMEM =
    "PLANTING\n"    /**/
    "WEED IS A\n"   /**/
    "GREAT WAY\n"   /**/
    "TO PAY YOUR\n" /**/
    "CO2 QUOTA.\n"; /**/
const char textStable[] PROGMEM =
    "EACH STABLE\n"  /**/
    "ALLOWS YOU\n"   /**/
    "TO EXPORT\n"    /**/
    "ADDITIONAL\n"   /**/
    "EXCEEDING\n"    /**/
    "FOOD.\n"        /**/
    "\nPRESS B TO\n" /**/
    "CHECK YOUR\n"   /**/
    "EXPORTS.\n";    /**/
const char textUpgrade[] PROGMEM =
    "REMEMBER TO\n"   /**/
    "CHECK STATS\n"   /**/
    "TO SEE HOW\n"    /**/
    "UPGRADING\n"     /**/
    "BUILDINGS\n"     /**/
    "ALTERS THEM.\n"; /**/
const char textBack[] PROGMEM = "";
const char textHouse2[] PROGMEM = "";
const char textBank2[] PROGMEM = "";
const char textSheriff2[] PROGMEM = "";
const char textChurch2[] PROGMEM = "";
const char textSaloon2[] PROGMEM = "";
const char textStable2[] PROGMEM = "";
const char textFarm2[] PROGMEM = "";
const char textTotem[] PROGMEM = "";
#if 0
const char templ[] PROGMEM = 
    "            \n"  /**/
    "            \n"  /**/
    "            \n"  /**/
    "            \n"  /**/
    "            \n"  /**/
    "            \n"  /**/
    "            \n"  /**/
    "            \n"  /**/
    "            \n"; /**/
#endif

const char* const descriptions[static_cast<uint8_t>(Building::IDs::count)] PROGMEM = {
    textEmpty,    /* EMPTY    */
    textBack,     /* BACK     */
    textUpgrade,  /* UPGRADE  */
    textHouse,    /* HOUSE    */
    textWater,    /* WATER    */
    textSaloon,   /* SALOON   */
    textFarm,     /* FARM     */
    textMill,     /* MILL     */
    textMine,     /* MINE     */
    textChurch,   /* CHURCH   */
    textSheriff,  /* SHERIFF  */
    textBank,     /* BANK     */
    textStable,   /* STABLE   */
    textWeed,     /* WEED     */
    textCactus,   /* CACTUS   */
    textTree,     /* TREE     */
    textTotem,    /* TOTEM    */
    textHouse2,   /* HOUSE2   */
    textBank2,    /* BANK2    */
    textSheriff2, /* SHERIFF2 */
    textChurch2,  /* CHURCH2  */
    textSaloon2,  /* SALOON2  */
    textStable2,  /* STABLE2  */
    textFarm2,    /* FARM2    */
};

const char* Building::description(const uint8_t id) {
  return (const char*)pgm_read_ptr(&(descriptions[id]));
}
