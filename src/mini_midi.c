#include "mini_midi.h"

static inline uint32_t read_u32_be(const uint8_t *buffer, size_t *offset)
{
  uint32_t value = (buffer[(*offset) + 0] << 24) | (buffer[(*offset) + 1] << 16) | (buffer[(*offset) + 2] << 8) | (buffer[(*offset) + 3]);
  (*offset) += 4;
  return value;
}

static inline uint16_t read_u16_be(const uint8_t *buffer, size_t *offset)
{
  uint16_t value = (buffer[(*offset) + 0] << 8) | (buffer[(*offset) + 1]);
  (*offset) += 2;
  return value;
}

bool mm_read_header(const uint8_t *buffer, size_t bufferSize, mm_header_t *header)
{
  size_t offset = 0;
  if (bufferSize < 14)
    return false;

  uint32_t id = read_u32_be(buffer, &offset);
  if (id != 0x4D546864)
    return false;

  uint32_t size = read_u32_be(buffer, &offset);
  if (size != 6)
    return false;

  header->format = read_u16_be(buffer, &offset);
  header->trackCount = read_u16_be(buffer, &offset);
  header->timeDivision = read_u16_be(buffer, &offset);

  return true;
}
