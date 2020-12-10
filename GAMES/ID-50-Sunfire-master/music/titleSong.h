#ifndef TITLESONG_H
#define TITLESONG_H
  
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof (a) / sizeof ((a)[0]))
#endif
  
#ifndef NUM_PATTERNS
#define NUM_PATTERNS(struct_) (ARRAY_SIZE( ((struct_ *)0)->patterns_offset))
#endif
  
#ifndef DEFINE_PATTERN
#define DEFINE_PATTERN(pattern_id, values) const uint8_t pattern_id[] = values;
#endif
  
/* pattern (channel) / bytes = 22*/
#define titleSong_pattern0_data { \
    ATM_CMD_M_SET_VOLUME(32), \
    ATM_CMD_M_TREMOLO_ON(3, 15), \
    ATM_CMD_M_SET_TEMPO(24), \
    ATM_CMD_M_SET_TRANSPOSITION(0), \
    ATM_CMD_M_CALL(7), \
    ATM_CMD_M_CALL_REPEAT(7, 2), \
    ATM_CMD_M_SET_TRANSPOSITION(-2), \
    ATM_CMD_M_CALL(7), \
    ATM_CMD_M_CALL_REPEAT(7, 2), \
    ATM_CMD_I_STOP, \
}
DEFINE_PATTERN(titleSong_pattern0_array, titleSong_pattern0_data);
    
/* pattern (channel) / bytes = 19*/
#define titleSong_pattern1_data { \
    ATM_CMD_M_SET_VOLUME(48), \
    ATM_CMD_M_SLIDE_VOL_ON(-16), \
    ATM_CMD_M_SET_TRANSPOSITION(0), \
    ATM_CMD_M_CALL(6), \
    ATM_CMD_M_CALL_REPEAT(6, 2), \
    ATM_CMD_M_SET_TRANSPOSITION(-2), \
    ATM_CMD_M_CALL(6), \
    ATM_CMD_M_CALL_REPEAT(6, 2), \
    ATM_CMD_I_STOP, \
}
DEFINE_PATTERN(titleSong_pattern1_array, titleSong_pattern1_data);
    
/* pattern (channel) / bytes = 1*/
#define titleSong_pattern2_data { \
    ATM_CMD_I_STOP, \
}
DEFINE_PATTERN(titleSong_pattern2_array, titleSong_pattern2_data);
    
/* pattern (channel) / bytes = 15*/
#define titleSong_pattern3_data { \
    ATM_CMD_M_SET_TRANSPOSITION(0), \
    ATM_CMD_M_CALL(8), \
    ATM_CMD_M_CALL_REPEAT(8, 2), \
    ATM_CMD_M_SET_TRANSPOSITION(-2), \
    ATM_CMD_M_CALL(8), \
    ATM_CMD_M_CALL_REPEAT(8, 2), \
    ATM_CMD_I_STOP, \
}
DEFINE_PATTERN(titleSong_pattern3_array, titleSong_pattern3_data);
    
/* pattern (drum) / bytes = 6*/
#define titleSong_pattern4_data { \
    ATM_CMD_M_SET_VOLUME(31), \
    ATM_CMD_M_DELAY_TICKS(1), \
    ATM_CMD_M_SET_VOLUME(0), \
    ATM_CMD_I_RETURN, \
}
DEFINE_PATTERN(titleSong_pattern4_array, titleSong_pattern4_data);
    
/* pattern (drum) / bytes = 10*/
#define titleSong_pattern5_data { \
    ATM_CMD_M_NOISE_RETRIG_ON(4), \
    ATM_CMD_M_SET_VOLUME(31), \
    ATM_CMD_M_SLIDE_VOL_ON(-8), \
    ATM_CMD_M_DELAY_TICKS(4), \
    ATM_CMD_I_NOISE_RETRIG_OFF, \
    ATM_CMD_M_SLIDE_VOL_OFF, \
    ATM_CMD_I_RETURN, \
}
DEFINE_PATTERN(titleSong_pattern5_array, titleSong_pattern5_data);
    
/* pattern (tune) / "Pattern 1" / bytes = 33*/
#define titleSong_pattern6_data { \
    ATM_CMD_I_NOTE_G3, \
    ATM_CMD_M_DELAY_TICKS(3), \
    ATM_CMD_I_NOTE_OFF, \
    ATM_CMD_M_DELAY_TICKS(1), \
    ATM_CMD_I_NOTE_G3, \
    ATM_CMD_M_DELAY_TICKS(3), \
    ATM_CMD_I_NOTE_OFF, \
    ATM_CMD_M_DELAY_TICKS(1), \
    ATM_CMD_I_NOTE_D4, \
    ATM_CMD_M_DELAY_TICKS(3), \
    ATM_CMD_I_NOTE_OFF, \
    ATM_CMD_M_DELAY_TICKS(1), \
    ATM_CMD_I_NOTE_G3, \
    ATM_CMD_M_DELAY_TICKS(3), \
    ATM_CMD_I_NOTE_OFF, \
    ATM_CMD_M_DELAY_TICKS(1), \
    ATM_CMD_I_NOTE_D4_, \
    ATM_CMD_M_DELAY_TICKS(3), \
    ATM_CMD_I_NOTE_OFF, \
    ATM_CMD_M_DELAY_TICKS(1), \
    ATM_CMD_I_NOTE_G3, \
    ATM_CMD_M_DELAY_TICKS(3), \
    ATM_CMD_I_NOTE_OFF, \
    ATM_CMD_M_DELAY_TICKS(1), \
    ATM_CMD_I_NOTE_D4, \
    ATM_CMD_M_DELAY_TICKS(3), \
    ATM_CMD_I_NOTE_OFF, \
    ATM_CMD_M_DELAY_TICKS(1), \
    ATM_CMD_I_NOTE_G3, \
    ATM_CMD_M_DELAY_TICKS(3), \
    ATM_CMD_I_NOTE_OFF, \
    ATM_CMD_M_DELAY_TICKS(1), \
    ATM_CMD_I_RETURN, \
}
DEFINE_PATTERN(titleSong_pattern6_array, titleSong_pattern6_data);
    
/* pattern (tune) / "Pattern 2" / bytes = 3*/
#define titleSong_pattern7_data { \
    ATM_CMD_I_NOTE_G3, \
    ATM_CMD_M_DELAY_TICKS(32), \
    ATM_CMD_I_RETURN, \
}
DEFINE_PATTERN(titleSong_pattern7_array, titleSong_pattern7_data);
    
/* pattern (tune) / "Pattern 3" / bytes = 10*/
#define titleSong_pattern8_data { \
    ATM_CMD_M_CALL(4), \
    ATM_CMD_M_DELAY_TICKS(3), \
    ATM_CMD_M_CALL(5), \
    ATM_CMD_M_DELAY_TICKS(8), \
    ATM_CMD_M_CALL(4), \
    ATM_CMD_M_DELAY_TICKS(15), \
    ATM_CMD_I_RETURN, \
}
DEFINE_PATTERN(titleSong_pattern8_array, titleSong_pattern8_data);
    
const PROGMEM struct titleSong_score_data {
  uint8_t fmt;
  uint8_t num_patterns;
  uint16_t patterns_offset[9];
  uint8_t num_channels;
  uint8_t start_patterns[4];
  uint8_t titleSong_pattern0[sizeof(titleSong_pattern0_array)];
  uint8_t titleSong_pattern1[sizeof(titleSong_pattern1_array)];
  uint8_t titleSong_pattern2[sizeof(titleSong_pattern2_array)];
  uint8_t titleSong_pattern3[sizeof(titleSong_pattern3_array)];
  uint8_t titleSong_pattern4[sizeof(titleSong_pattern4_array)];
  uint8_t titleSong_pattern5[sizeof(titleSong_pattern5_array)];
  uint8_t titleSong_pattern6[sizeof(titleSong_pattern6_array)];
  uint8_t titleSong_pattern7[sizeof(titleSong_pattern7_array)];
  uint8_t titleSong_pattern8[sizeof(titleSong_pattern8_array)];
} titleSong = {
  .fmt = ATM_SCORE_FMT_FULL,
  .num_patterns = NUM_PATTERNS(struct titleSong_score_data),
  .patterns_offset = {
      offsetof(struct titleSong_score_data, titleSong_pattern0),
      offsetof(struct titleSong_score_data, titleSong_pattern1),
      offsetof(struct titleSong_score_data, titleSong_pattern2),
      offsetof(struct titleSong_score_data, titleSong_pattern3),
      offsetof(struct titleSong_score_data, titleSong_pattern4),
      offsetof(struct titleSong_score_data, titleSong_pattern5),
      offsetof(struct titleSong_score_data, titleSong_pattern6),
      offsetof(struct titleSong_score_data, titleSong_pattern7),
      offsetof(struct titleSong_score_data, titleSong_pattern8),
  },
  .num_channels = 4,
  .start_patterns = {
    0x00,                         // Channel 0 entry pattern (SQUARE)
    0x01,                         // Channel 1 entry pattern (SQUARE)
    0x02,                         // Channel 2 entry pattern (SQUARE)
    0x03,                         // Channel 3 entry pattern (NOISE)
  },
  .titleSong_pattern0 = titleSong_pattern0_data,
  .titleSong_pattern1 = titleSong_pattern1_data,
  .titleSong_pattern2 = titleSong_pattern2_data,
  .titleSong_pattern3 = titleSong_pattern3_data,
  .titleSong_pattern4 = titleSong_pattern4_data,
  .titleSong_pattern5 = titleSong_pattern5_data,
  .titleSong_pattern6 = titleSong_pattern6_data,
  .titleSong_pattern7 = titleSong_pattern7_data,
  .titleSong_pattern8 = titleSong_pattern8_data,
};

#endif