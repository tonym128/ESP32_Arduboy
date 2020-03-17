//----------------------------------------------------------------------------
// ROOFTOP RESCUE 
// (c) 2019 Bert vt Veer
//----------------------------------------------------------------------------

#define SECS * 60
#define FRAMES * 1

constexpr uint8_t   CHOPPER_BASELINE = 12;
constexpr uint8_t   CHOPPER_CAPACITY = 3;
constexpr int8_t    CHOPPER_BRAKE_DIST = 16;
constexpr float     CHOPPER_DIAMETER = 10.0;
constexpr float     ROPE_SPEED = 0.5; 
constexpr uint16_t  BUILDING_FIRE1_TIME = 8 SECS;
constexpr uint16_t  BUILDING_FIRE2_TIME = 12 SECS;
constexpr uint16_t  BUILDING_FIRE3_TIME = 15 SECS;
constexpr uint16_t  BUILDING_SHAKE_TIME = 22 SECS;
constexpr uint16_t  BUILDING_COLLAPSE_TIME = 30 SECS;
constexpr uint16_t  DUDE_DEFAULT_SPAWN_TIME = 3 SECS;
constexpr uint16_t  DUDE_SPAWN_RANDOMIZE = 2 SECS;
constexpr int16_t   DUDE_ENTER_TIME = 38 FRAMES;
constexpr uint8_t   DUDES_PER_LEVEL[] = { 1,2,3,3,4,6,3,3,5,7,1,4,5,5,6,6,3,5,7,8,9,10,11,12,0 };
constexpr uint16_t  UFO_INITIAL_TIMEOUT = 120 SECS;
constexpr uint16_t  UFO_SEQUENTIAL_TIMEOUT = 40 SECS;
constexpr int8_t    UFO_BASELINE = 12;
constexpr int8_t    UFO_FREQUENCY = 31;
constexpr int8_t    UFO_AMPLITUDE = 13;
constexpr uint8_t   UFO_SPEED_DIV = 4;
constexpr float     UFO_DIAMETER = 11.0;
constexpr uint16_t  GAMEOVER_DURATION = 7 SECS;
constexpr uint16_t  GAMEOVER_MSG_FRAME = 1 SECS;

// Do not change
constexpr uint8_t   FRAME_DEFAULT_COUNT = 4;
constexpr uint8_t   FRAME_DEFAULT_WAIT = 8 FRAMES;
constexpr uint8_t   MAX_BUILDINGS = 5;
constexpr uint8_t   BUILDING_MAX_FIRES = 3;
constexpr uint8_t   MAX_DUDES = 16;
constexpr int8_t    DIRECTION_LEFT = -1;
constexpr int8_t    DIRECTION_RIGHT = 1;
constexpr int8_t    ROPE_EXTEND = 1;
constexpr int8_t    ROPE_RETRACT = -1;


//----------------------------------------------------------------------------
// Base class for animated objects
//----------------------------------------------------------------------------

struct Animated
{
    uint16_t counter = 0;
    uint8_t current_frame = 0;
    uint8_t frame_count = FRAME_DEFAULT_COUNT;
    uint16_t frames_wait = FRAME_DEFAULT_WAIT;
    
    void animate() {
        if (++counter % frames_wait == 0) {
            current_frame = (current_frame+1) % frame_count;
        }
    }
};

//----------------------------------------------------------------------------
// Rope
//----------------------------------------------------------------------------

struct Rope
{
    int16_t x;
    int16_t y;
    float length;
    float target_length; 
    bool moving;
    bool stopped_moving;
    bool occupied;
    uint16_t timeout;
    bool dude_enter;
    
    bool is_accessible() const {
        return (!moving && !occupied && (length > 0));
    }
    
    void init() {
        length = 0;
        target_length = 0;
        moving = false;
        stopped_moving = false;
        occupied = false;
    } 
    
    void update(int16_t px, int16_t py) {
        x = px;
        y = py;
        dude_enter = false;
        if (abs(length-target_length) > 0.01) {
            moving = true;
            stopped_moving = false;
            length += (target_length > length) ? ROPE_SPEED : -ROPE_SPEED;           
        }
        else {
            length = target_length;
            stopped_moving = moving;
            moving = false;
            if (stopped_moving) {
                if (occupied && length==0) {       
                    dude_enter = true;
                    occupied = false;
                }
            }
        }
    }
};

//----------------------------------------------------------------------------
// Chopper
//----------------------------------------------------------------------------

struct Chopper : public Animated
{
    int16_t x;
    int16_t y;
    int16_t target_x;
    float x_real;
    float x_velocity;
    bool active;
    bool moving;
    bool exploding;
    uint8_t current_stop;
    uint8_t dudes_onboard;
    uint16_t timeout;
    Rope rope;
    
    void init() {
        x = 250;
        y = CHOPPER_BASELINE;
        x_real = x;
        x_velocity = -1.0;
        active = true;
        moving = false;
        exploding = false;
        current_frame = 0;
        current_stop = 4;
        dudes_onboard = 0;
        timeout = 0;
        rope.init();
    }
      
    void update() {
        if (x == target_x) {
            moving = false;
            x_velocity = 0;
        }
        else {               
            moving = true;   
            float force = x < target_x ? 0.05 : -0.05;
            x_velocity = minVal(maxVal(x_velocity+force, -1.0), 1.0);
            if (fabs(target_x - x_real) < CHOPPER_BRAKE_DIST) x_velocity *= 0.9;
            x_real += x_velocity;
            x = round(x_real);   
        }
        if (counter % 6 == 0) {
            if (x_velocity > 0.1) { if (current_frame < 6) current_frame++; }
            else if (x_velocity < -0.1) { if (current_frame > 0) current_frame--; }
        }
        y = round(CHOPPER_BASELINE + sin(counter/24));
        rope.update(x+10, y+8);
        if (rope.dude_enter) {
            dudes_onboard++;
            timeout=10;  
        }     
        if (timeout > 0) timeout--;
        counter++;  
    }
    
    void moveTo(int16_t new_x) {
        target_x = new_x;
    }

    bool collide(int16_t x2, int16_t y2, float diameter2) {
        if (exploding) return (active = false);
        int16_t xdiff = x2 - (x + 10);
        int16_t ydiff = y2 - (y + 6);
        float dist = sqrt((xdiff*xdiff) + (ydiff*ydiff));
        bool colliding = dist < (CHOPPER_DIAMETER/2 + diameter2/2);
        exploding = colliding;
        return colliding;
    }
};

//----------------------------------------------------------------------------
// Buildings
//----------------------------------------------------------------------------

struct Fire : public Animated
{
    int16_t x;
    int16_t y;
    bool active;   
    bool extinguished;
};

struct Building
{
    enum e_state { NOT_USED, NORMAL, ONFIRE, SHAKING, COLLAPSING, COLLAPSED };
    int16_t x;
    int16_t y;
    int16_t offset_x;
    int16_t offset_y;
    e_state state;
    uint16_t counter;
    uint8_t fire_index;
    uint16_t fire_counter;
    uint16_t extinguish_timer;
    uint8_t dude_count;
    Fire fires[BUILDING_MAX_FIRES];

    void init() {
        offset_x = 0;
        offset_y = 0;
        state = NORMAL;
        counter = 0;
        fire_index = 0; 
        fire_counter = 0;
        extinguish_timer = 0;
        dude_count = 0;
        for (uint8_t i=0; i<BUILDING_MAX_FIRES; i++) {
            fires[i].active = false;  
        }
    }

    e_state update() {
        switch(state) {
            case ONFIRE:
                fire_counter++;
                if (fire_counter == BUILDING_FIRE1_TIME) addFire(5,14);
                if (fire_counter == BUILDING_FIRE2_TIME) addFire(10,9);
                if (fire_counter == BUILDING_FIRE3_TIME) addFire(1,6);
                if (fire_counter > BUILDING_SHAKE_TIME) state = SHAKING;
                break;
            case SHAKING:
                fire_counter++;
                if (fire_counter > BUILDING_COLLAPSE_TIME) state = COLLAPSING;
                offset_y = (rand()%10)/8;
                break;
            case COLLAPSING: 
                if (++counter % 4 == 0) {
                    offset_y++;
                    if (y + offset_y > 68) {
                        state = COLLAPSED; 
                        extinguish();
                    }
                }              
                break; 
            default: 
                break;
        }
        for (uint8_t i=0; i<BUILDING_MAX_FIRES; i++) {
            if (fires[i].active) {
                fires[i].animate();
                if (fires[i].current_frame > 5 && fires[i].extinguished) fires[i].active = false;  
            }
        }
        if (extinguish_timer > 0) {
            if (--extinguish_timer == 0) {
                extinguish();
            }
        }
        return state;
    }

    void incDudes() { 
        dude_count++; 
        if (state < ONFIRE) state = ONFIRE;
    }
    void decDudes() { 
        dude_count--;
        if (dude_count == 0) {
            extinguish_timer = 60+(rand()%120);
            state = NORMAL;
            fire_counter = 0;
        }
    }

    void extinguish() {
        for (uint8_t i=0; i<BUILDING_MAX_FIRES; i++) fires[i].extinguished = true;
        fire_counter = 0;
    }

    void addFire(int16_t px, int16_t py) {
        fires[fire_index].x = x + px;
        fires[fire_index].y = y + py;        
        fires[fire_index].active = true;
        fires[fire_index].extinguished = false;
        fires[fire_index].frame_count = 8; // last 2 frame not displayed
        fire_index = (fire_index+1) % BUILDING_MAX_FIRES;
    }
};

struct Buildings
{
    Building items[MAX_BUILDINGS];
    uint8_t next_free = 0;
    uint8_t total_dudes;
    
    void init() {
        total_dudes = 0;
        for (uint8_t i=0; i<MAX_BUILDINGS; i++) {
            if (i < next_free) items[i].init();
        }
    }
    
    Building::e_state update(bool increase_fire) {
        Building::e_state ret_event = Building::NOT_USED;
        total_dudes = 0;
        for (uint8_t i=0; i<MAX_BUILDINGS; i++) {
            Building::e_state update_st = items[i].update();
            total_dudes += items[i].dude_count;
            if (!increase_fire) items[i].fire_counter = 0;
            if (update_st >= Building::COLLAPSING) ret_event = update_st;
        } 
        return ret_event;       
    }
    
    void add(int16_t x, int16_t y) {
        items[next_free].x = x;
        items[next_free].y = y;
        items[next_free].state = Building::NORMAL;
        next_free++;
    }
};

//----------------------------------------------------------------------------
// Dudes
//----------------------------------------------------------------------------
    
struct DudeInfo {
    enum e_dudestate { CREATE, IN_CHOPPER, LEAVE, WANDER, ON_ROPE };
    e_dudestate state;
    bool grabbed;
    DudeInfo(e_dudestate state_, bool grabbed_) : state(state_), grabbed(grabbed_) {}    
};

struct Dude : public Animated
{
    bool active;
    DudeInfo::e_dudestate state; 
    Building* building;
    int16_t global_x;
    int16_t global_y;
    int16_t local_x;
    int8_t dir;
    uint16_t lifetime;
    
    DudeInfo update(Rope& rope) {
        bool grabbed = false;
        if (active) {
            switch(state) {
                case DudeInfo::ON_ROPE:
                    local_x = 0;
                    global_x = rope.x-2;
                    global_y = rope.y + rope.length - 7;
                    if (global_y < rope.y-3) state = DudeInfo::IN_CHOPPER;
                    current_frame = 2;
                    break;
                    
                case DudeInfo::IN_CHOPPER:
                    active = false;
                    break;
                    
                case DudeInfo::LEAVE:
                    dir = 0;
                    animate();
                    if (local_x > 20) active = false;
                    if (counter%5 == 0) {
                        local_x++;
                        if (global_y < building->y-7) global_y++;
                    }
                    break;
                    
                case DudeInfo::WANDER: 
                {
                    if (building->state == Building::COLLAPSED) { active = false; break; }
                    global_y = (building->y + building->offset_y) - 7;
                    float new_dir = cos(2.0 + counter/40.0);
                    local_x = round(new_dir * 5);
                    dir = new_dir > cos(2.0 + (counter+1)/40.0) ? 1 : 0;
                    if (lifetime > DUDE_ENTER_TIME) {
                        animate();
                        bool xhit = global_x + local_x == rope.x-1;                  
                        if (building->state < Building::COLLAPSING && rope.is_accessible() && xhit) {
                            rope.occupied = true;
                            state = DudeInfo::ON_ROPE;
                            global_x = rope.x;
                            local_x = 0;
                            grabbed = true;
                        }
                    }
                    break;
                }
                    
                default:
                    break;
            }
            lifetime++;
        }
        return DudeInfo(state, grabbed);
    }
};

struct Dudes
{
    Dude items[MAX_DUDES];
    uint16_t count;

    void init() {
        for (uint8_t i=0; i<MAX_DUDES; i++) {
            items[i].active = false;
        }
    }
    
    void add(Building* building_, int16_t px, int16_t py, DudeInfo::e_dudestate state_ = DudeInfo::CREATE) {
        for (uint8_t i=0; i<MAX_DUDES; i++) {
            if (items[i].active == false) {
                items[i].active = true;
                items[i].state = state_;
                items[i].building = building_;
                items[i].local_x = 0;
                items[i].global_x = px;
                items[i].global_y = py;
                items[i].lifetime = state_ == DudeInfo::CREATE ? 0 : 1000;
                items[i].counter = 0;
                items[i].current_frame = 0;
                count++;
                break;
            }
        }
    } 
     
    int16_t update(Rope& rope) {
        count = 0;
        int16_t grabbed = -1;
        for (uint8_t i=0; i<MAX_DUDES; i++) {
            DudeInfo info = items[i].update(rope);
            if (items[i].active) count++;
            if (info.grabbed) grabbed = i;
            if (info.state == DudeInfo::CREATE) { 
                count++; 
                items[i].state = DudeInfo::WANDER;
            }
        }
        return grabbed;
    }
};

//----------------------------------------------------------------------------
// Ufo
//----------------------------------------------------------------------------

struct Ufo : public Animated
{
    int16_t x;
    int16_t y;
    bool active;
    uint16_t timeout;
    
    void init() {
        timeout = UFO_INITIAL_TIMEOUT;
        x = -40;
        y = CHOPPER_BASELINE;
        active = false;
        frame_count = 3;
        frames_wait = 10;
    }

    void activate(uint16_t next_timeout = UFO_INITIAL_TIMEOUT) {
        if (!active) {
            x = -40 - rand()%24;
            active = true;
            counter = 0;
            timeout = next_timeout;
        } 
    }
    
    bool update() {
        if (active) {
            animate();
            if (counter % UFO_SPEED_DIV == 0) x++;
            if (x > 130) active = false;
            y = UFO_BASELINE+round(sin((float)counter/UFO_FREQUENCY)*UFO_AMPLITUDE);
            return false;
        }
        else {
            return --timeout == 0;
        }
    }  
};

//----------------------------------------------------------------------------
// Game
//----------------------------------------------------------------------------

struct Game
{
    enum e_state { INIT, SPLASH, TITLE, FOREPLAY, PLAYING, GAMEOVER_WAIT, GAMEOVER };
    enum e_return_event { NONE, END_GAME, POINT_SCORED, DUDE_SPAWNED, UFO_SPAWNED, PLAYER_HIT, PLAYER_DIED,
                          BUILDING_COLLAPSING, BUILDING_COLLAPSED };    
    Chopper chopper;
    Buildings buildings;
    Dudes dudes;
    Ufo ufo;

    e_state state = e_state(0);
    uint16_t score;
    uint8_t level;
    uint8_t level_dudes;
    uint16_t newdude_timer;
    uint16_t newdude_init;
    uint16_t spawn_timeout;
    uint16_t hiscore;
    bool dude_grabbed = false;
    uint8_t stops[5] = { 5, 28, 52, 77, 101 };
    uint16_t gameover_counter;
    uint16_t wait_counter;

    void setState(e_state st) { 
        state = st;
        switch(state) {
            case TITLE: break;
            case PLAYING: 
                score = 0;
                level = 0;
                level_dudes = DUDES_PER_LEVEL[level];
                newdude_init = DUDE_DEFAULT_SPAWN_TIME;
                newdude_timer = newdude_init;
                spawn_timeout = 0;
                dude_grabbed = false;
                gameover_counter = 0;
                wait_counter = 0;
                chopper.init(); 
                buildings.init();
                dudes.init();
                ufo.init();
                break;
            case GAMEOVER_WAIT:
                wait_counter = 0;
                break;
            case GAMEOVER:
                gameover_counter = 0;
                chopper.exploding = false;
                chopper.rope.init();
                chopper.active = false;
                ufo.active = false;
                break;
            default:
                break;
        }
    }

    Building& getBuilding(uint8_t index) {
        return buildings.items[index];
    }
    
    e_return_event update() 
    {
        e_return_event ret_event = NONE;
        if (!chopper.exploding) chopper.update();
        else {
            if (chopper.counter++ % 18 == 0) chopper.y++;
            if (chopper.y > CHOPPER_BASELINE+12) {
                chopper.active = false;
                setState(GAMEOVER);
                return PLAYER_DIED;
            }
        }
        Building::e_state buildings_event = buildings.update(state <= PLAYING);
        int16_t which_dude_grabbed = dudes.update(chopper.rope);
        if (!chopper.exploding && chopper.active && ufo.update()) {
            ufo.activate(UFO_SEQUENTIAL_TIMEOUT);
            spawn_timeout = 60; 
            ret_event = UFO_SPAWNED;
        }
                          
        switch(state) {
            case PLAYING:              
                if (buildings_event == Building::COLLAPSED) {
                    setState(GAMEOVER_WAIT);
                    return BUILDING_COLLAPSED;
                }
                else {                   
                    if (chopper.exploding) return ret_event;  
                    if (buildings_event == Building::COLLAPSING) return BUILDING_COLLAPSING; 
                                 
                    // Check if any dudes grabbed the rope                 
                    dude_grabbed = which_dude_grabbed >= 0;
                    if (dude_grabbed) {
                        chopper.timeout = 10;
                        dudes.items[which_dude_grabbed].building->decDudes();
                    }
                    // Check if it's time to spawn a new dude
                    if (spawn_timeout > 0) {
                        spawn_timeout--; 
                    }
                    else if (buildings.total_dudes + chopper.dudes_onboard < DUDES_PER_LEVEL[level] && newdude_timer-- == 0) {
                        if (spawnDude()) {
                            newdude_timer = newdude_init + (rand() % DUDE_SPAWN_RANDOMIZE);
                            ret_event = DUDE_SPAWNED;
                        }
                        else newdude_timer = 1; // try again next frame
                    }                    
                    // Check if dudes need disembarked
                    if (chopper.timeout==0) {            
                        if (chopper.current_stop==4 && !chopper.moving && chopper.dudes_onboard>0) {
                            dudes.add(&(buildings.items[4]), chopper.x+8, chopper.y+7, DudeInfo::LEAVE);
                            chopper.dudes_onboard--;
                            chopper.timeout = 33; 
                            score++;
                            if (--level_dudes == 0) {
                                if (DUDES_PER_LEVEL[level+1] > 0) level++;
                                level_dudes = DUDES_PER_LEVEL[level];
                            }
                            ret_event = POINT_SCORED;
                        }
                    }
                    // Check if collided with ufo
                    if (ufo.active && chopper.collide(ufo.x+UFO_DIAMETER/2, ufo.y+UFO_DIAMETER/2, UFO_DIAMETER)) {
                        ret_event = PLAYER_HIT;
                        ufo.active = false;
                        chopper.counter = 0;
                        chopper.rope.init();
                        for (uint8_t i=0; i<MAX_DUDES; i++) { // Remove dudes on the rope
                            if (dudes.items[i].state == DudeInfo::ON_ROPE) dudes.items[i].active = false;
                        }
                    }
                    return ret_event;
                }
                break;
            case GAMEOVER_WAIT:
                if (wait_counter++ > 120) {
                    chopper.active = false;
                    setState(GAMEOVER);
                }
                break;
            case GAMEOVER:
                if (gameover_counter++ > GAMEOVER_DURATION) {
                    return END_GAME;
                }
                break;
            default:
                break;
        }
        return NONE;
    }

    void moveChopper(int8_t dir) {
        if (chopper.rope.length > 0) return;
        switch(dir) {
            case DIRECTION_LEFT:               
                if ((chopper.x - chopper.target_x) < CHOPPER_BRAKE_DIST && chopper.current_stop > 0)
                    chopper.moveTo(stops[--chopper.current_stop]);
                break;
            case DIRECTION_RIGHT:
                if ((chopper.target_x - chopper.x) < CHOPPER_BRAKE_DIST && chopper.current_stop < 4) 
                    chopper.moveTo(stops[++chopper.current_stop]); 
                break;
        }
    }

    bool commandRope(int8_t com) {
        if (!chopper.moving && chopper.current_stop<4  && !chopper.rope.moving && chopper.dudes_onboard < CHOPPER_CAPACITY) {
            if (com == ROPE_EXTEND && chopper.rope.target_length == 0) {
                chopper.rope.target_length = buildings.items[chopper.current_stop].y - CHOPPER_BASELINE-9;
                return true;             
            }
            if (com == ROPE_RETRACT && chopper.rope.target_length > 0) {
                chopper.rope.target_length = 0;
                return true;
            }
        }
        return false;
    }
    
    bool spawnDude() {
        uint8_t building_idx = rand() % 4;
        Building& building = buildings.items[building_idx];
        uint8_t current_max_dudes = DUDES_PER_LEVEL[level] < 8 ? 2 : 3;
        if (building.state < Building::SHAKING && building.dude_count < current_max_dudes) {
            dudes.add(&building, stops[building_idx]+8, 64-building.y-7);
            building.incDudes();
            return true;
        }
        return false;
    }
};
