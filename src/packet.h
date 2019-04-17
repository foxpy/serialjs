#include <stdlib.h>
#define PACKET_SIZE 16

extern int acm_fd;

void crc32(uint32_t *dst, void *src, size_t len);
void build_packet(uint8_t command, uint8_t *args, uint8_t *packet);
void write_packet(uint8_t *packet);
