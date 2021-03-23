typedef struct
{
  void (*_create)();
  void (*_update)();
  void (*_draw)();
  void (*_delete)();
  unsigned int high_score = 0;
  unsigned long time = 0;
} Game;

void send_message(char * string);
void add_number_to_string(char * string, int number);
void add_string_to_string(char * string1, char * string2);

void set_state_game_over();

