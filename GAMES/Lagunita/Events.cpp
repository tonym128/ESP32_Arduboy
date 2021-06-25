#include "Events.hpp"

const char textHouse[]  =
    "\n"              /**/
    "WELCOME TO\n"    /**/
    "LAGUNITA,\n"     /**/
    "A SMALL LAKE\n"  /**/
    "IN THE WILD\n"   /**/
    "WILD WEST.\n"    /**/
    "\n"              /**/
    "PRESS A TO\n"    /**/
    "BUILD YOUR\n"    /**/
    "FIRST HOUSE.\n"; /**/
const char textFarm[]  =
    "\n"             /**/
    "BUILD A FARM\n" /**/
    "TO GIVE A\n"    /**/
    "JOB TO THE\n"   /**/
    "CITIZENS.\n"    /**/
    "\n"             /**/
    "ARROW UP\n"     /**/
    "TO SWITCH\n"    /**/
    "BUILDING.\n";   /**/
const char textWater[]  =
    "\n"             /**/
    "MORE PEOPLE\n"  /**/
    "WILL MOVE\n"    /**/
    "IN, IF THE\n"   /**/
    "HOUSES ARE\n"   /**/
    "SERVICED\n"     /**/
    "WITH WATER.\n"  /**/
    "\n"             /**/
    "TRY BUILDING\n" /**/
    "SOME WATER\n"   /**/
    "TOWERS.\n";     /**/
const char textSaloon[]  =
    "\n"            /**/
    "THE COWBOYS\n" /**/
    "ARE GETTING\n" /**/
    "RESTLESS.\n"   /**/
    "\n"            /**/
    "BUILD A NEW\n" /**/
    "SALOON TO\n"   /**/
    "ENTERTAIN\n"   /**/
    "THEM AND\n"    /**/
    "RAISE THEIR\n" /**/
    "HAPPINESS.\n"; /**/
const char textMine[]  =
    "\n"             /**/
    "THE GOLD\n"     /**/
    "FEVER\n"        /**/
    "REACHED\n"      /**/
    "LAGUNITA!\n"    /**/
    "\nBUILD A\n"    /**/
    "MINE TO\n"      /**/
    "INCRESE\n"      /**/
    "THE INCOME.\n"; /**/
const char textBank[]  =
    "\n"             /**/
    "BUILD A BANK\n" /**/
    "TO INCREASE \n" /**/
    "GOLD STORAGE\n" /**/
    "BY 5000$.\n";   /**/
const char textSheriff[]  =
    "\n"             /**/
    "THE BANDITS\n"  /**/
    "COULD SMELL\n"  /**/
    "YOUR MONEY\n"   /**/
    "AND THEY ARE\n" /**/
    "COMING TO\n"    /**/
    "ASSAULT THE\n"  /**/
    "BANK!\n"        /**/
    "BUILD A POST\n" /**/
    "FOR THE\n"      /**/
    "SHERIFF.\n";    /**/
const char textMill[]  =
    "\n"              /**/
    "AS THE TOWN\n"   /**/
    "GROWS, THE\n"    /**/
    "COST FOR\n"      /**/
    "IMPORTING\n"     /**/
    "FOOD GETS\n"     /**/
    "PROBLEMATIC.\n"  /**/
    "\n"              /**/
    "BUILD A MILL\n"  /**/
    "TO FEED THE\n"   /**/
    "LAGUNITIANS.\n"; /**/
const char textChurch[]  =
    "\n"             /**/
    "A PRIEST\n"     /**/
    "PASSING BY\n"   /**/
    "PROPOSES YOU\n" /**/
    "TO EDUCATE\n"   /**/
    "THE PEOPLE\n"   /**/
    "IF YOU BUILD\n" /**/
    "A SMALL\n"      /**/
    "CHURCH FOR\n"   /**/
    "HIM.\n";        /**/
const char textEmpty[]  =
    "\n"            /**/
    "EVEN THE\n"    /**/
    "BEST OF US\n"  /**/
    "MAKE SOME\n"   /**/
    "MISTAKES.\n"   /**/
    "\n"            /**/
    "NOW YOU\n"     /**/
    "CAN REMOVE\n"  /**/
    "TREES AND\n"   /**/
    "BUILDINGS.\n"; /**/
const char textTree[]  =
    "\n"              /**/
    "PLANT A FEW\n"   /**/
    "TREES TO\n"      /**/
    "PROVIDE\n"       /**/
    "SOME SHADE\n"    /**/
    "FOR THE\n"       /**/
    "LAGUNITIANS!\n"; /**/
const char textCactus[]  =
    "\n"             /**/
    "NOW YOU CAN\n"  /**/
    "PLANT YOUR\n"   /**/
    "OWN CACTAI!\n"; /**/
const char textWeed[]  =
    "\n"             /**/
    "TAKE SOME\n"    /**/
    "WEED SEEDS!\n"; /**/
const char textStable[]  =
    "\n"             /**/
    "THE QUALITY\n"  /**/
    "OF YOUR FOOD\n" /**/
    "IS RENOWNED!\n" /**/
    "\n"             /**/
    "BUILD A\n"      /**/
    "STABLE TO\n"    /**/
    "EXPORT THE\n"   /**/
    "EXTRA FOOD.\n"; /**/
const char textUpgrade[]  =
    "\n"             /**/
    "AS THE TOWN \n" /**/
    "GROWS, SPACE\n" /**/
    "BECOMES VERY\n" /**/
    "VALUABLE.\n"    /**/
    "NOW YOU CAN\n"  /**/
    "UPGRADE SOME\n" /**/
    "OF THE\n"       /**/
    "BUILDINGS.\n";  /**/
#if 0
const char templ[]  = 
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

const char* const texts[Events::count]  = {
    textHouse,   /* HOUSE   */
    textFarm,    /* FARM    */
    textWater,   /* WATER   */
    textSaloon,  /* SALOON  */
    textStable,  /* STABLE  */
    textMine,    /* MINE    */
    textSheriff, /* SHERIFF */
    textChurch,  /* CHURCH  */
    textBank,    /* BANK    */
    textMill,    /* MILL    */
    textEmpty,   /* CLEAR   */
    textTree,    /* TREE    */
    textCactus,  /* CACTUS  */
    textWeed,    /* WEED    */
    textUpgrade, /* UPGRADE */
};

static const uint16_t onPopulation[Events::count]  = {
    0,   /* HOUSE   */
    1,   /* FARM    */
    10,  /* WATER   */
    15,  /* SALOON  */
    20,  /* STABLE  */
    30,  /* MINE    */
    50,  /* SHERIFF */
    65,  /* CHURCH  */
    75,  /* BANK    */
    100, /* MILL    */
    150, /* CLEAR   */
    200, /* TREE    */
    250, /* CACTUS  */
    300, /* WEED    */
    175, /* UPGRADE */
};

static const Building::IDs unlocks[Events::count]  = {
    Building::IDs::house,   /* HOUSE   */
    Building::IDs::farm,    /* FARM    */
    Building::IDs::water,   /* WATER   */
    Building::IDs::saloon,  /* SALOON  */
    Building::IDs::stable,  /* STABLE  */
    Building::IDs::mine,    /* MINE    */
    Building::IDs::sheriff, /* SHERIFF */
    Building::IDs::church,  /* CHURCH  */
    Building::IDs::bank,    /* BANK    */
    Building::IDs::mill,    /* MILL    */
    Building::IDs::empty,   /* CLEAR   */
    Building::IDs::tree,    /* TREE    */
    Building::IDs::cactus,  /* CACTUS  */
    Building::IDs::weed,    /* WEED    */
    Building::IDs::upgrade, /* UPGRADE */
};

static const Building::IDs dependsOn[Events::count]  = {
    Building::IDs::back,    /* HOUSE   */
    Building::IDs::house,   /* FARM    */
    Building::IDs::farm,    /* WATER   */
    Building::IDs::water,   /* SALOON  */
    Building::IDs::saloon,  /* STABLE  */
    Building::IDs::stable,  /* MINE    */
    Building::IDs::mine,    /* SHERIFF */
    Building::IDs::sheriff, /* CHURCH  */
    Building::IDs::church,  /* BANK    */
    Building::IDs::bank,    /* MILL    */
    Building::IDs::back,    /* CLEAR   */
    Building::IDs::back,    /* TREE    */
    Building::IDs::back,    /* CACTUS  */
    Building::IDs::back,    /* WEED    */
    Building::IDs::back,    /* UPGRADE */
};

const char* Events::getText(uint8_t id) {
  return (const char*)pgm_read_ptr(&(texts[id]));
}

uint8_t Events::buildingUnlocked(uint8_t id) {
  return pgm_read_byte(&unlocks[id]);
}

EventState Events::update(
    EventState ev,
    uint8_t id,
    uint16_t population,
    Building::Status buildings[static_cast<uint8_t>(Building::IDs::count)]) {
  switch (ev) {
    case EventState::untriggered:
      if (population >= pgm_read_word(&onPopulation[id]) &&
          (buildings[static_cast<uint8_t>(pgm_read_byte(&dependsOn[id]))].built > 0)) {
        return EventState::justTriggered;
      } else {
        return EventState::untriggered;
      }
      break;
    default:
      break;
  }

  return EventState::triggered;
}
