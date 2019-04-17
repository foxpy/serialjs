#define MOVE_CMD 0x01
#define ACTION_CMD 0x11
#define TOGGLE_CMD 0x12
#define ABORT_CMD 0x21

void write_packet(uint8_t *packet);
void move_cmd(float x, float y);
void action_cmd(int8_t s);
void toggle_cmd(uint8_t s);
void abort_cmd();
