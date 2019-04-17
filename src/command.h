#define MOVE_CMD 0x01
#define ACTION_CMD 0x11
#define TOGGLE_CMD 0x12
#define ABORT_CMD 0x21

void write_packet(uint8_t *packet, int fd);
void move_cmd(float x, float y, int fd);
void action_cmd(int8_t s, int fd);
void toggle_cmd(uint8_t s, int fd);
void abort_cmd(int fd);
