# Animation

An animation system that allows playing of frame based animations similar to playing a song

struct Animation {
    const uint8_t* frames;
    const uint8_t* durations;
   
    
};

Animation a;
a.frames = shopOwner_tiles;
a.durations = { 0, 10, 1, 10, 2, 10 };
