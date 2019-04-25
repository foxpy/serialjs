#include <stdlib.h>
#define PACKET_SIZE 16

extern int acm_fd;

void crc32(uint32_t *dst, void *src, size_t len);
void build_packet(uint8_t command, void *args, void *dst);
void write_packet(void *packet);
