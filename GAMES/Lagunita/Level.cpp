#include "Level.hpp"
#include "Lagunita.hpp"

extern Arduboy2 arduboy;
uint8_t mountains_dat[255];

static const uint16_t minute = Lagunita::frame_rate * 64;

void Level::load(uint16_t start) {
  uint8_t* raw = reinterpret_cast<uint8_t*>(this);
  for (uint16_t loc = 0; loc < sizeof(Level); loc++) {
    raw[loc] = EEPROM.read(start + loc);
  }
}

void Level::save(uint16_t start) {
  uint8_t* raw = reinterpret_cast<uint8_t*>(this);
  for (uint16_t loc = 0; loc < sizeof(Level); loc++) {
    EEPROM.write(start + loc, raw[loc]);
  }
  EEPROM.commit();
}

void Level::init() {
  camera = 0;
  camera_off = 0;
  camera_scrolls = 0;
  camera_sign = 0;
  housing = 0;
  jobs = 0;
  earnings = 0;
  maintenance = 0;
  happiness = 100;
  safety = 100;
  spirituality = 100;
  environment = 100;
  exports = 0;
  currBuil = Building::IDs::house;
  money = 999;
  tutor[0] = '\0';
  tutorVisible = false;
  population = 0;
  inProgress = false;
  inStats = false;
  river_in = 5;
  river_out = size / 2;

  for (uint8_t i=0; i<sizeof(mountains_dat); i++)
    mountains_dat[i]=random(255);

  for (uint8_t i = 0; i < static_cast<uint8_t>(Events::count); i++) {
    tutorials[i] = EventState::untriggered;
  }

  for (uint8_t i = 0; i < static_cast<uint8_t>(Building::IDs::count); i++) {
    buildings[i].enabled = false;
    buildings[i].built = 0;
  }

  /* Enable building houses as it's the first building that unlock others. */
  buildings[static_cast<uint8_t>(Building::IDs::house)].enabled = true;

  /* Make exceptions for the action that allows to go back to main menu.*/
  buildings[static_cast<uint8_t>(Building::IDs::back)].enabled = true;
  buildings[static_cast<uint8_t>(Building::IDs::back)].built = 1;

  frameNextEvent = minute + (rand() % minute);

  /* Add some random vegetation. */
  uint8_t mask = 0x0f;
  for (uint16_t i = 0; i < size; i++) {
    uint8_t r = rand() & mask;
    mask = 0x0f;
    /* Leave some empty space around the starting area. */
    if (i < 4 || i > size - 4) {
      r = 0x00;
    }
    switch (r) {
      case 1:
        tiles[i].building = Building::IDs::cactus;
        tiles[i].progress = 0b111;
        break;
      case 2:
        tiles[i].building = Building::IDs::weed;
        tiles[i].progress = 0b111;
        break;
      /* Trees are non removable and add to the strategy: let's double them. */
      case 3:
      case 4:
        tiles[i].building = Building::IDs::tree;
        tiles[i].progress = 0b111;
        /* After a tree there must be a blank space, so reset the mask. */
        mask = 0x00;
        break;
      default:
        setTileEmpty(i);
        break;
    }
  }

  /* Exceptions for rivers. */
  setTileEmpty(river_in);
  setTileEmpty(river_out);

  /* Add 2 special totems. */
  setTileEmpty(1 * (size / 3) - 1);
  tiles[1 * (size / 3) + 0].building = Building::IDs::totem;
  tiles[1 * (size / 3) + 0].progress = 0b111;
  setTileEmpty(2 * (size / 3) - 1);
  tiles[2 * (size / 3) + 0].building = Building::IDs::totem;
  tiles[2 * (size / 3) + 0].progress = 0b111;

  /* Init the random walkers and birds */
  for (uint8_t i = 0; i < npc_count; i++) {
    walking[i] = rand() % (size * 8);
    flying[i] = rand() % (size * 8);
  }

  inProgress = true;
  resume();
}

void Level::onInput(Input dir) {
  /* If there is a tutorial displayed, only allow the b button */
  if (tutorVisible && (dir != Input::b)) {
    return;
  }
  uint8_t sel = static_cast<uint8_t>(currBuil);
  bool doubleClick = (frame - frameLastInput) < 12;
  frameLastInput = frame;
  switch (dir) {
    case Input::up:
      if (lastPressed == Input::up && doubleClick) {
        /* undo last up. */
        sel = (sel + 1) % Building::count();

        /* find next category up. */
        if (sel >= Building::NATURE) {
          sel = Building::SERVICES;
        } else if (sel >= Building::SERVICES) {
          sel = Building::HOUSING;
        } else if (sel >= Building::HOUSING) {
          sel = Building::ACTIONS;
        } else {
          sel = Building::NATURE;
        }

        /* find previously available building */
        while (buildings[sel].enabled == false) {
          sel = (sel == 0) ? (Building::count() - 1) : sel - 1;
        }

        frameLastInput = 0;
      } else {
        do {
          sel = (sel == 0) ? (Building::count() - 1) : sel - 1;
        } while (buildings[sel].enabled == false);
      }
      currBuil = static_cast<Building::IDs>(sel);
      lastPressed = Input::up;
      break;
    case Input::down:
      if (lastPressed == Input::down && doubleClick) {
        /* undo last down. */
        sel = (sel == 0) ? (Building::count() - 1) : sel - 1;

        /* find next category down. */
        if (sel < Building::HOUSING) {
          sel = Building::HOUSING;
        } else if (sel < Building::SERVICES) {
          sel = Building::SERVICES;
        } else if (sel < Building::NATURE) {
          sel = Building::NATURE;
        } else {
          sel = Building::ACTIONS;
        }

        /* find next available building */
        while (buildings[sel].enabled == false) {
          sel = (sel + 1) % Building::count();
        }

        frameLastInput = 0;
      } else {
        do {
          sel = (sel + 1) % Building::count();
        } while (buildings[sel].enabled == false);
      }

      currBuil = static_cast<Building::IDs>(sel);
      lastPressed = Input::down;
      break;
    case Input::left:
      if (lastPressed == Input::left && doubleClick) {
        findFirstAvailableSpot(-1);
        frameLastInput = 0;
      }
      lastPressed = Input::left;
      break;
    case Input::right:
      if (lastPressed == Input::right && doubleClick) {
        findFirstAvailableSpot(+1);
        frameLastInput = 0;
      }
      lastPressed = Input::right;
      break;
    case Input::a: {
      uint8_t idx = static_cast<uint8_t>(currBuil);
      if (money < Building::cost(idx)) {
        snprintf_P(tutor, tutorLen, PSTR("\nYOU HAVE\nNOT ENOUGH\nMONEY TO\nBUILD."));
        tutorVisible = true;
      } else if (Building::IDs::upgrade == currBuil) {
        Building::IDs current = cursorOverlaps(false);
        Building::IDs upgrade = Building::upgrade(current);
        if (upgrade != Building::IDs::empty) {
          uint16_t cost = Building::cost(upgrade);
          if (money < cost) {
            snprintf_P(tutor, tutorLen, PSTR("\nYOU HAVE\nNOT ENOUGH\nMONEY TO\nUPGRADE."));
            tutorVisible = true;
          } else {
            cursorOverlaps(true, upgrade);
            uint8_t upgrade_u8 = static_cast<uint8_t>(upgrade);
            buildings[upgrade_u8].built = min(buildings[upgrade_u8].built + 1, 1024);
            money -= cost;
          }
        }
      } else if (Building::IDs::back == currBuil) {
        pause();
      } else {
        bool replace = true;
        uint16_t cidx = (camera + 7) % size;

        if (Building::IDs::empty == currBuil) {
          if (Building::IDs::totem != tiles[cidx].building) {
            cursorOverlaps(true);
          } else {
            replace = false;
          }
        } else {
          Building::IDs collision = buildCollides();
          if (Building::IDs::tree == collision) {
            snprintf_P(tutor, tutorLen, PSTR("\nYOU CANNOT\nBUILD OVER A\nTREE."));
            tutorVisible = true;
            replace = false;
          } else if (Building::IDs::weed != collision && Building::IDs::cactus != collision &&
                     Building::IDs::empty != collision) {
            snprintf_P(tutor, tutorLen, PSTR("\nYOU CANNOT\nBUILD OVER A\nBUILDING."));
            tutorVisible = true;
            replace = false;
          }
        }
        if (replace) {
          /* Destroy everything on the path of this building */
          for (uint16_t i = 0; i < Building::width(currBuil); i++) {
            uint8_t del_b = static_cast<uint8_t>(tiles[(cidx + i) % size].building);
            if (buildings[del_b].built > 0 &&
                del_b != static_cast<uint8_t>(Building::IDs::empty)) {
              buildings[del_b].built--;
            }
            setTileEmpty((cidx + i) % size);
          }

          tiles[cidx].building = currBuil;
          money -= Building::cost(idx);
        }
      }
      lastPressed = Input::a;
      break;
    }
    case Input::b:
      if (tutorVisible) {
        if (inStats) {
          snprintf_P(tutor, tutorLen,                                        /**/
                     PSTR("\nHAPPINESS\n%4d %%\nSAFETY\n%4d %%\n"            /**/
                          "SPIRITUALITY\n%4d %%\nENVIRONMENT\n%4d %%\n"),    /**/
                     happiness, safety, spirituality, environment, exports); /**/
          inStats = false;
        } else {
          if (tutorChars >= strlen(tutor)) {
            /* Pressing B when the text is all shown, closes the dialog. */
            tutorVisible = false;
          } else {
            /* Pressing B while the text is animating, shows all the text. */
            tutorChars = tutorLen;
          }
        }
      } else {
        inStats = true;
        tutorVisible = true;
        tutorChars = tutorLen; /* No text animation for stats. */

        snprintf_P(tutor, tutorLen,                                            /**/
                   PSTR("\nHOUSING DAYS\n%7d%5d\nJOBS\t  FOOD\n%4d%8d\n"       /**/
                        " MAINTENANCE\n%8d $/s\n\tEARNINGS\n%8d $/s\n"         /**/
                        "\tEXPORTS\n%8d $/s"),                                 /**/
                   housing, days, jobs, food, maintenance, earnings, exports); /**/
      }
      lastPressed = Input::b;
      break;
    default:
      break;
  }
}

static void update_statistic(uint8_t& statistic,
                             uint8_t multiplier,
                             uint16_t count,
                             uint16_t denominator) {
  /* Assume numerator <= size and multiplier < 65536 / size */
  uint16_t numerator = count * multiplier;

  if (denominator < multiplier || numerator >= denominator) {
    if (statistic < 100)
      statistic++;
    return;
  }

  if (static_cast<uint32_t>(100) * numerator < static_cast<uint32_t>(statistic) * denominator) {
    statistic--;
  }
  if (static_cast<uint32_t>(100) * numerator >=
          static_cast<uint32_t>(statistic + 1) * denominator &&
      statistic < 100) {
    statistic++;
  }
}


Building::IDs Level::cursorOverlaps(bool clear, Building::IDs replaceWith) {
  uint16_t cidx = camera + 7;
  uint16_t cidxm = cidx % size;

  /* Check if we are in the middle of another building or tree */
  for (uint16_t i = 0; i < 4; i++) {
    uint16_t lidx = (cidxm + size - i) % size;
    Building::IDs id = tiles[lidx].building;
    uint16_t ends = Building::width(id);
    /* If cidx is bigger than size-4, don't do the modulo. */
    if ((cidxm < (size - 4) && ((lidx + ends) % size) > cidxm) || ((lidx + ends) > cidx)) {
      if (Building::IDs::empty != id) {
        bool freeToClear = Building::IDs::weed == id || Building::IDs::cactus == id;
        if (clear || (!freeToClear)) {
          if (clear) {
            uint8_t del_b = static_cast<uint8_t>(id);
            if (buildings[del_b].built > 0) {
              buildings[del_b].built--;
            }
            tiles[lidx].building = replaceWith;
            tiles[lidx].progress = 0;
          }
          return id;
        }
      }
    }
  }

  return Building::IDs::empty;
}

Building::IDs Level::buildCollides() {
  uint16_t cidxm = (camera + 7) % size;

  /* Check if there is another building or tree on the right */
  for (uint16_t i = 0; i < Building::width(currBuil); i++) {
    Building::IDs id = tiles[(cidxm + i) % size].building;
    if (Building::IDs::weed != id && Building::IDs::cactus != id &&
        Building::IDs::empty != id) {
      return id;
    }
  }

  return cursorOverlaps();
}

bool Level::canBuild() {
  if (Building::IDs::empty == currBuil)
    return true;

  return Building::IDs::empty == buildCollides();
}

void Level::findFirstAvailableSpot(int8_t dir) {
  uint16_t start = camera;

  do {
    if (canBuild()) {
      camera_off = 4;
      break;
    }
    camera += dir;
    camera %= size;
  } while (camera != start);
}

void Level::nextDay() {
  days++;
}

void Level::update() {
  frame++;
  if (frame % dayLength == 0)
    nextDay();

  if (!tutorVisible && (camera_off == 0)) {
    bool left = arduboy.pressed(LEFT_BUTTON);
    bool right = arduboy.pressed(RIGHT_BUTTON);
    if (left && right) {
      /* Scroll back to the center position */
      camera /= 2;
      camera_scrolls = 0;
    } else if (left) {
      if (camera == 0) {
        camera = size - 1;
      } else {
        camera--;
      }
      camera_off = 8;
      camera_scrolls = camera_scrolls < 32 ? camera_scrolls + 1 : 32;
      camera_sign = 0;
    } else if (right) {
      camera = (camera + 1) % size;
      camera_off = 8;
      camera_scrolls = camera_scrolls < 32 ? camera_scrolls + 1 : 32;
      camera_sign = 1;
    } else {
      camera_scrolls = 0;
    }
  }

  static uint8_t updateProgress = 0;

  if ((frame % (fps / 8)) == 0) {
    updateProgress = (updateProgress + 1) % 8;
    uint16_t max_money = 2500;
    housing = 0;
    maintenance = 0;
    earnings = 0;
    uint16_t vegetation = 0;
    uint16_t churches = 0;
    int16_t churches_effectivity = 0;
    uint16_t sheriffs = 0;
    uint16_t saloons = 0;
    jobs = 1;
    food = 1;
    for (uint16_t obj = 0; obj < size; obj++) {
      maintenance += Building::maintenance(tiles[obj].building);
      jobs += Building::jobs(tiles[obj].building);
      uint16_t house_housing = 0;
      if (tiles[obj].building == Building::IDs::house ||
          tiles[obj].building == Building::IDs::house2) {
        house_housing += 4;
        for (uint16_t i = (obj + size - 16); i < (obj + size + 16); i++) {
          if (tiles[i % size].building == Building::IDs::water) {
            house_housing += 4;
            break;
          }
        }
        for (uint16_t i = (obj + size - 8); i < (obj + size + 8); i++) {
          Building::IDs bld = tiles[i % size].building;
          if (bld == Building::IDs::tree || bld == Building::IDs::cactus ||
              bld == Building::IDs::weed) {
            house_housing += 2;
            break;
          }
        }
        for (uint16_t i = (obj + size - 8); i < (obj + size + 8); i++) {
          if (tiles[i % size].building == Building::IDs::totem) {
            house_housing += 16;
            break;
          }
        }
      }
      if (tiles[obj].building == Building::IDs::house2) {
        house_housing *= 2;
      }
      housing += house_housing;
      if (tiles[obj].building != Building::IDs::empty && 0 == updateProgress) {
        /* If we are about to complete a building, let's increase the built counters. */
        if (tiles[obj].progress == 0b110) {
          uint8_t sel = static_cast<uint8_t>(tiles[obj].building);

          /* If a building just got built, let's show some hints about it. */
          if (0 == buildings[sel].built) {
            Building::IDs upgrade = Building::upgrade(sel);
            /* Check that no upgraded version of the building exists. */
            if (0 == buildings[static_cast<uint8_t>(upgrade)].built) {
              strncpy_P(tutor, Building::description(sel), tutorLen);
              tutorVisible = true;
            }
          }

          buildings[sel].built = min(buildings[sel].built + 1, 1024);
        }
        tiles[obj].progress = min(0b111, tiles[obj].progress + 1);
      }
    }

    int16_t unemployed = population;

    /* First loop to allocate workers to produce food. */
    for (uint16_t obj = 0; obj < size; obj++) {
      if ((tiles[obj].building == Building::IDs::farm) && (unemployed > 0)) {
        food += 22;
        earnings += Building::profit(tiles[obj].building);
        unemployed -= Building::jobs(tiles[obj].building);
        /* If a water tower is close increase food production. */
        for (uint16_t i = (obj + size - 6); i < (obj + size + 16); i++) {
          if (tiles[i % size].building == Building::IDs::water) {
            food += 11;
            break;
          }
        }
        for (uint16_t i = (obj + size - 7); i < (obj + size + 16); i++) {
          if (tiles[i % size].building == Building::IDs::totem) {
            food += 33;
            break;
          }
        }
      } else if ((tiles[obj].building == Building::IDs::farm2) && (unemployed > 0)) {
        food += 44;
        earnings += Building::profit(tiles[obj].building);
        unemployed -= Building::jobs(tiles[obj].building);
        /* If a water tower is close increase food production. */
        for (uint16_t i = (obj + size - 16); i < (obj + size + 16); i++) {
          if (tiles[i % size].building == Building::IDs::water) {
            food += 22;
            break;
          }
        }
        for (uint16_t i = (obj + size - 8); i < (obj + size + 8); i++) {
          if (tiles[i % size].building == Building::IDs::totem) {
            food += 66;
            break;
          }
        }
      } else if ((tiles[obj].building == Building::IDs::mill) && (unemployed > 0)) {
        food += 11;
        earnings += Building::profit(tiles[obj].building);
        unemployed -= Building::jobs(tiles[obj].building);
        /* If a farm is close increase food production. */
        for (uint16_t i = (obj + size - 10); i < (obj + size + 10); i++) {
          if (tiles[i % size].building == Building::IDs::farm) {
            food += 11;
            break;
          }
        }
        for (uint16_t i = (obj + size - 8); i < (obj + size + 8); i++) {
          if (tiles[i % size].building == Building::IDs::totem) {
            food += 33;
            break;
          }
        }
      }
    }

    /* Second loop to allocate workers to other buildings. */
    for (uint16_t obj = 0; obj < size; obj++) {
      if (tiles[obj].building == Building::IDs::bank) {
        max_money += 2500;
      } else if (tiles[obj].building == Building::IDs::bank2) {
        max_money += 5000;
      } else if ((tiles[obj].building == Building::IDs::water ||
                  tiles[obj].building == Building::IDs::saloon) &&
                 unemployed > 0) {
        earnings += Building::profit(tiles[obj].building);
      } else if (tiles[obj].building == Building::IDs::tree ||
                 tiles[obj].building == Building::IDs::cactus ||
                 tiles[obj].building == Building::IDs::weed) {
        vegetation++;
      } else if (tiles[obj].building == Building::IDs::church) {
        /* If vegetation is close increase effectivity. */
        /* If noisy buildings are close decrease effectivity. */
        for (uint16_t i = (obj + size - 8); i < (obj + size + 16); i++) {
          Building::IDs bld = tiles[i % size].building;
          if (bld == Building::IDs::cactus || bld == Building::IDs::weed) {
            churches_effectivity += 4;
          } else if (bld == Building::IDs::tree) {
            churches_effectivity += 8;
          } else if (bld == Building::IDs::stable || bld == Building::IDs::bank) {
            churches_effectivity -= 16;
          } else if (bld == Building::IDs::saloon || bld == Building::IDs::mine) {
            churches_effectivity -= 32;
          }
          if (tiles[i % size].building == Building::IDs::totem) {
            churches_effectivity -= 64;
          }
        }
      } else if (tiles[obj].building == Building::IDs::mine && unemployed > 0) {
        earnings += Building::profit(tiles[obj].building);
        for (uint16_t i = (obj + size - 8); i < (obj + size + 8); i++) {
          if (tiles[i % size].building == Building::IDs::totem) {
            earnings += 55;
            break;
          }
        }
      }
      unemployed -= Building::jobs(tiles[obj].building);
    }

    saloons = buildings[static_cast<uint8_t>(Building::IDs::saloon)].built;
    saloons += 2 * buildings[static_cast<uint8_t>(Building::IDs::saloon2)].built;
    churches = buildings[static_cast<uint8_t>(Building::IDs::church)].built;
    churches += 2 * buildings[static_cast<uint8_t>(Building::IDs::church2)].built;
    sheriffs = buildings[static_cast<uint8_t>(Building::IDs::sheriff)].built;
    sheriffs += 2 * buildings[static_cast<uint8_t>(Building::IDs::sheriff2)].built;

    /* Mines hurts environment. */
    if (vegetation > buildings[static_cast<uint8_t>(Building::IDs::mine)].built * 10) {
      vegetation -= buildings[static_cast<uint8_t>(Building::IDs::mine)].built * 10;
    } else {
      vegetation = 0;
    }

    /* Some vegetation for every 16 people */
    update_statistic(environment, 16, vegetation, population);
    /* A saloon for every 24 people */
    update_statistic(happiness, 24, saloons, population);
    /* A church for every 100 people */
    if (churches_effectivity < 0) {
      churches_effectivity = 0;
    }
    int32_t church_corrected = (static_cast<int32_t>(population) - churches_effectivity) > 0
                                   ? population - churches_effectivity
                                   : population;
    update_statistic(spirituality, 100, churches, church_corrected);
    /* A sheriff for every 100 people with a penalty for banks */
    uint16_t bank_penalty = 32 * buildings[static_cast<uint8_t>(Building::IDs::bank)].built;
    update_statistic(safety, 100, sheriffs, population + bank_penalty);

    /* Use the minimum of all four statistics to change the RGB led color */
    uint8_t ledval = min(min(min(environment, happiness), spirituality), safety) / 4;
    arduboy.setRGBled((25 - ledval)/6, ledval/6, 0);

    if (frame % fps)
      return;

    money += earnings;
    if (money > maintenance) {
      money -= maintenance;
    } else {
      money = 0;
    }

    uint32_t stats =
        (static_cast<uint16_t>(environment) + happiness + spirituality + safety) / 4;
    uint16_t max_housing = (static_cast<uint32_t>(housing) * stats) / 100;

    max_housing = max_housing > jobs ? jobs : max_housing;

    if (population < max_housing) {
      population++;
    } else if (population > 1 && population > max_housing) {
      population--;
    }
    if (population > food) {
      population = food;
    }

    /* Export food-population and cap it for 10*each stable. */
    exports = buildings[static_cast<uint8_t>(Building::IDs::stable)].built * 10;
    exports += buildings[static_cast<uint8_t>(Building::IDs::stable2)].built * 20;
    exports = min((food - population), exports);

    money += exports;

    if (money > max_money) {
      money = max_money;
    }

    if (!tutorVisible) {
      /* Random events happen between 1 and 2 minutes. */
      if (frame > frameNextEvent) {
        frameNextEvent = frame + minute + (rand() % minute);

        uint16_t r = rand() % 3;
        if ((r == 0) && buildings[static_cast<uint8_t>(Building::IDs::sheriff)].enabled) {
          if (safety < 100) {
            /* If the safety is low, simulate a robbery. */
            r = rand() % (money / 2);
            snprintf_P(tutor, tutorLen,                         /**/
                       PSTR("\nYOU HAVE\nBEEN ROBBED!\n"        /**/
                            "\nTHE THIEVES\nSTOLE %4d$\n"       /**/
                            "\nBUILD MORE\nSHERIFF\nPOSTS!\n"), /**/
                       r);                                      /**/
            money -= r;
          } else {
            /* Else inform that the sheriff saved the day. */
            snprintf_P(tutor, tutorLen,                   /**/
                       PSTR("\nTHE SHERIFF\nMANAGED TO\n" /**/
                            "PREVENT A\nBAND OF\n"        /**/
                            "BANDITS FROM\nSTEALING\n"    /**/
                            "YOUR MONEY!"                 /**/
                            ),                            /**/
                       r);                                /**/
          }
          tutorVisible = true;
        } else if ((r == 1) && buildings[static_cast<uint8_t>(Building::IDs::church)].enabled) {
          if (spirituality < 100) {
            /* If the spirituality is low, simulate emigration. */
            r = rand() % (population / 2);
            snprintf_P(tutor, tutorLen,       /**/
                       PSTR("\n%4d PEOPLE\n"  /**/
                            "LOST FAITH\n"    /**/
                            "IN LAGUNITA\n"   /**/
                            "AND DECIDED\n"   /**/
                            "TO FOUND\n"      /**/
                            "THEIR OWN\n"     /**/
                            "TOWN.\n"         /**/
                            "BUILD MORE\n"    /**/
                            "CHURCHES TO\n"   /**/
                            "RISE FAITH!\n"), /**/
                       r);                    /**/
            population -= r;
          } else {
            /* Else inform that the church is bringing people together. */
            snprintf_P(tutor, tutorLen,                    /**/
                       PSTR("\nLAGUNITIANS\nLOVE LIVING\n" /**/
                            "IN YOUR TOWN\nAND THEY\n"     /**/
                            "NEVER FELT\nAS UNITED!\n"     /**/
                            ),                             /**/
                       r);                                 /**/
          }
          tutorVisible = true;
        } else if ((r == 2) && buildings[static_cast<uint8_t>(Building::IDs::tree)].enabled) {
          if (environment < 100) {
            /* If the environment is low, simulate a land slide. */
            snprintf_P(tutor, tutorLen,        /**/
                       PSTR("\nTHERE AREN'T\n" /**/
                            "ENOUGH TREES\n"   /**/
                            "AND PLANTS\n"     /**/
                            "TO STABILIZE\n"   /**/
                            "THE GROUND.\n"    /**/
                            "A LANDSLIDE\n"    /**/
                            "DESTROYED\n"      /**/
                            "SOME OF YOUR\n"   /**/
                            "BUILDINGS."));    /**/

            tutorVisible = true;
            r = rand() % size;
            uint16_t broken = 4 + rand() % 16;
            for (uint16_t i = r; (broken > 0) && (i < (r + size)); i++) {
              uint16_t b = i % size;
              uint8_t bld = static_cast<uint8_t>(tiles[b].building);
              if (bld >= Building::HOUSING &&
                  bld != static_cast<uint8_t>(Building::IDs::totem) &&
                  bld != static_cast<uint8_t>(Building::IDs::tree) &&
                  bld != static_cast<uint8_t>(Building::IDs::cactus) &&
                  bld != static_cast<uint8_t>(Building::IDs::weed)) {
                buildings[bld].built--;
                tiles[b].building = Building::IDs::cactus;
                tiles[b].progress = 0b000;
                broken--;
              }
            }
          }
        }
      } else {
        /* Check if any tutorial event is ready to trigger. */
        for (uint8_t t = 0; t < Events::count; t++) {
          tutorials[t] = Events::update(tutorials[t], t, population, buildings);
          if (tutorials[t] == EventState::justTriggered) {
            uint8_t b = Events::buildingUnlocked(t);
            buildings[b].enabled = true;
            const char* src = Events::getText(t);
            strncpy_P(tutor, src, tutorLen);
            tutorVisible = true;
            break;
          }
        }
      }
    }
  }
}

static const uint8_t PROGMEM dither[9][4] = {
    {0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10001000, 0b00000000, 0b00000000, 0b00000000},
    {0b10001000, 0b00000000, 0b00100010, 0b00000000},
    {0b10101010, 0b00000000, 0b00100010, 0b00000000},
    {0b10101010, 0b00000000, 0b10101010, 0b00000000},
    {0b10101010, 0b01000100, 0b10101010, 0b00000000},
    {0b10101010, 0b01000100, 0b10101010, 0b00010001},
    {0b10101010, 0b01010101, 0b10101010, 0b00010001},
    {0b10101010, 0b01010101, 0b10101010, 0b01010101},
};

static uint8_t getDither(int8_t duskLevel, uint8_t x) {
  if (duskLevel < 0)
    duskLevel = 0;
  if (duskLevel > 8)
    duskLevel = 8;
  return pgm_read_byte(&dither[duskLevel][x & 3]);
}

void Level::renderBackground(uint16_t frame) {
  /* Draw mountains */
  int8_t x_off = (camera_sign ? (1) : (-1)) * (static_cast<int8_t>(camera_off));

  static const uint8_t topsvisible = 8;
  static const uint8_t topwidth = WIDTH / topsvisible;
  static const uint8_t ntops = size / topsvisible;
  static const uint16_t fudge = 0;//1024;

  int16_t mountainpos = camera * 8 - x_off;
  int16_t offset = mountainpos / topwidth;
  int16_t shift = -mountainpos % topwidth - topwidth;

  uint8_t timeOfDay = frame / (dayLength / 256);
  bool day = timeOfDay < 0x80;
  arduboy.invert(day);
  timeOfDay &= 0x7f;
  int8_t duskLevel = -64;
  if (timeOfDay < 0xc)
    duskLevel = 0x8 - timeOfDay;
  else if (timeOfDay > 0x74)
    duskLevel = timeOfDay - 0x77;

  for (int8_t i = -1; i <= ntops; i++) {
    uint8_t val1 = mountains_dat[fudge + offset % ntops];//pgm_read_byte((const void *)(fudge + offset % ntops));
    int16_t x0 = (val1 >> 4) + shift;
    int16_t y0 = (val1 & 0xf) + 12;

    offset++;
    shift += topwidth;
    uint8_t val2 = mountains_dat[fudge + offset % ntops];//pgm_read_byte((const void *)(fudge + offset % ntops));
    int16_t x1 = (val2 >> 4) + shift;
    int16_t y1 = (val2 & 0xf) + 12;

    /* Bresenham's algorithm for shallow lines going from left to right.
     * We use it to determine the height of the mountains, which we use
     * to draw both the mountain ridge and the sky above it during dawn
     * and dusk.
     */
    {
      int16_t dx, dy;
      dx = x1 - x0;
      dy = abs(y1 - y0);

      int16_t err = dx / 2;
      int8_t ystep;

      if (y0 < y1) {
        ystep = 1;
      } else {
        ystep = -1;
      }

      for (; x0 < x1; x0++) {
        if (x0 >= 0) {
          if (x0 >= 128)
            break;
          for (uint8_t y = 0; y < 4; y++) {
            if (y0 < y * 8) {
              /* Do nothing */
            } else if (y0 < y * 8 + 8) {
              uint8_t mask = (255 >> (8 - (y0 & 7)));
              arduboy.sBuffer[x0 + y * 128] =
                  (getDither(duskLevel + (day ? -y : y), x0) & mask) | (1 << (y0 & 7));
            } else {
              arduboy.sBuffer[x0 + y * 128] = getDither(duskLevel + (day ? -y : y), x0);
            }
          }
        }

        err -= dy;
        if (err < 0) {
          y0 += ystep;
          err += dx;
        }
      }
    }
  }
}

void Level::render() {
  int8_t x_off = (camera_sign ? (1) : (-1)) * (static_cast<int8_t>(camera_off));

  renderBackground(frame);

  if (tutorVisible) {
    drawing.excludeVerticalBar(32, 32 + 64);
  }

  for (uint8_t i = 0; i < npc_count; i++) {
    if (!(frame % 1)) {
      flying[i] = (flying[i] + 1) % (size * 8);
    }

    int16_t pos = size + ((i % 2) ? (-flying[i]) : (flying[i]));

    /* Birds */
    drawing.drawBitmap((pos - camera * 8 + x_off) % (size * 8), 1 * 8,
                       &bmp_bird[((frame >> 2) % 4) * 8], 8, 8, i % 2);

    /* Draw two horses for each stable */
    if (buildings[static_cast<uint8_t>(Building::IDs::stable)].built * 2 > i) {
      drawing.drawBitmap((size / 2 + pos - camera * 8 + x_off) % (size * 8), 6 + 4 * 8,
                         &bmp_horse[((frame >> 2) % 4) * 16], 16, 8, i % 2);
    }
  }

  uint8_t cowboys = population / 16;
  cowboys = cowboys > npc_count ? npc_count : cowboys;
  for (uint8_t i = 0; i < cowboys; i++) {
    if (!(frame % 4)) {
      walking[i] = (walking[i] + 1) % (size * 8);
    }

    int16_t pos = size + ((i % 2) ? (-walking[i]) : (walking[i]));

    /* Walking objects */
    drawing.drawBitmap((pos - camera * 8 + x_off) % (size * 8), 6 + 4 * 8,
                       &bmp_man[((frame >> 3) % 4) * 8], 8, 8, i % 2);
  }

  /* Camera affected objects */
  for (int16_t obj = -5; obj < 17; obj++) {
    uint16_t moved = (obj + size + camera) % size;

    const bool in_river = (moved == river_in || moved == river_out);

    /* Area where characters walk */
    const uint8_t* bmp = (in_river ? bmp_bridge : bmp_empty);
    arduboy.drawBitmap(x_off + obj * 8, 4 * 8 + 4, bmp, 8, 8);

    /* Lake shore area */
    /* The river has 3 frames of animation while the ground has only 1. */
    uint8_t frames = in_river ? 3 : 1;
    bmp = in_river ? bmp_river : bmp_ground;
    bmp = bmp + 8 * ((frame >> 2) % frames);
    arduboy.drawBitmap(x_off + obj * 8, 5 * 8, bmp, 8, 8);

    /* Building area */
    Building::IDs b = tiles[moved].building;

    uint8_t id = static_cast<uint8_t>(b);
    bmp = Building::bitmap(id);
    uint8_t w = Building::width(id);
    uint8_t h = Building::height(id);
    uint8_t p = tiles[moved].progress + 1;
    uint8_t y = (4 - h) * 8 + 6;
    drawing.drawBitmapAlpha(x_off + obj * 8, y, bmp, w * 8, h * 8, h * (8 - p));
  }

  uint8_t sel = static_cast<uint8_t>(currBuil);

  /* Current selection */
  for (uint8_t tile = 7; tile < 7 + (Building::width(sel)); tile++) {
    const uint8_t* bmp = bmp_selection;
    arduboy.drawBitmap(tile * 8, 0, bmp, 8, 8);
  }

  /* Calculate the number of tubmleweeds to display as inverse proportional to houses built. */
  uint16_t houses = buildings[static_cast<uint8_t>(Building::IDs::house)].built;
  houses += buildings[static_cast<uint8_t>(Building::IDs::house2)].built;
  int16_t tbweeds = npc_count - (houses / 8);
  if (tbweeds > npc_count) {
    tbweeds = npc_count;
  }

  for (uint8_t i = 0; i < tbweeds; i++) {
    int16_t pos = size + ((i % 2) ? (-flying[i]) : (flying[i]));

    /* Tumbleweed */
    int16_t y_off = ((frame >> 3) % 8) - 4;
    y_off = (y_off < 0) ? (-y_off) : (y_off);
    drawing.drawBitmap((size / 4 + pos - camera * 8 + x_off) % (size * 8), 4 * 8 + y_off,
                       &bmp_tumbleweed[((frame >> 3) % 8) * 8], 8, 8);
  }

  /* GUI */
  char tmp_str[16];

  tinyfont.maskText = true;
  tinyfont.setCursor(1, 1);
  tinyfont.print(Building::name(sel));
  tinyfont.setCursor(1, 6);
  if (currBuil == Building::IDs::back) {
    /* Show nothing */
  } else if (currBuil == Building::IDs::upgrade) {
    Building::IDs current = cursorOverlaps(false);
    Building::IDs upgrade = Building::upgrade(current);
    /* Only show the price for upgradeable buildings. */
    if (upgrade != Building::IDs::empty) {
      tinyfont.print(Building::cost(upgrade));
    } else if (static_cast<uint8_t>(current) >= static_cast<uint8_t>(Building::IDs::upgrades)) {
      tinyfont.print("MAXED");
    } else {
      tinyfont.print("---");
    }
  } else {
    tinyfont.print(Building::cost(sel));
  }

  snprintf_P(tmp_str, 16, PSTR("%9d$"), money);
  tinyfont.setCursor(78, 1);
  tinyfont.print(tmp_str);
  snprintf_P(tmp_str, 16, PSTR("%9dP"), population);
  tinyfont.setCursor(78, 6);
  tinyfont.print(tmp_str);
  tinyfont.maskText = false;

  drawing.waterReflection(frame, camera * 8 - x_off);

  if (tutorVisible) {
    arduboy.fillRoundRect(32, 0, 64, 64, 4, BLACK);
    arduboy.drawRoundRect(32, 0, 64, 64, 4, WHITE);
    tinyfont.setCursor(35, 3, tutorChars);
    tinyfont.print(tutor);
    if (tutorChars < tutorLen) {
      tutorChars++;
    };
  } else {
    tutorChars = 0;
  }

  if (camera_off > 0) {
    camera_off -= camera_scrolls < 8 ? 1 : (camera_scrolls < 32 ? 2 : 4);
  }

  drawing.excludeNothing();
}
