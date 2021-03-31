#include "mini_midi.h"

static uint32_t read_u32_be(const uint8_t *buffer, size_t *offset)
{
  uint32_t value = (buffer[(*offset) + 0] << 24) | (buffer[(*offset) + 1] << 16) | (buffer[(*offset) + 2] << 8) | (buffer[(*offset) + 3]);
  (*offset) += 4;
  return value;
}

static uint16_t read_u16_be(const uint8_t *buffer, size_t *offset)
{
  uint16_t value = (buffer[(*offset) + 0] << 8) | (buffer[(*offset) + 1]);
  (*offset) += 2;
  return value;
}

static uint32_t read_u32_variable_be(const uint8_t *buffer, size_t *offset)
{
  uint32_t value = 0;
  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t byte = buffer[*offset];
    (*offset)++;
    value <<= 7;
    value |= byte & 0x7F;
    if ((byte & 0x80) == 0)
      break;
  }
  return value;
}

size_t mm_read_header(const uint8_t *buffer, size_t bufferSize, mm_header_t *header)
{
  size_t offset = 0;
  if (bufferSize < 14)
    return 0;

  uint32_t id = read_u32_be(buffer, &offset);
  if (id != 0x4D546864)
    return 0;

  uint32_t size = read_u32_be(buffer, &offset);
  if (size != 6)
    return 0;

  header->format = read_u16_be(buffer, &offset);
  header->trackCount = read_u16_be(buffer, &offset);
  header->timeDivision = read_u16_be(buffer, &offset);

  return offset;
}

size_t mm_read_track_chunk(const uint8_t *buffer, size_t bufferSize, uint32_t *trackSize)
{
  size_t offset = 0;
  if (bufferSize < 8)
    return 0;

  uint32_t id = read_u32_be(buffer, &offset);
  if (id != 0x4D54726B)
    return 0;

  *trackSize = read_u32_be(buffer, &offset);
  return offset;
}

size_t mm_read_track_event(const uint8_t *buffer, size_t bufferSize, mm_event_t *event)
{
  size_t offset = 0;

  if (bufferSize < 3)
    return 0;

  uint32_t deltaTime = read_u32_variable_be(buffer, &offset);
  if (offset + 1 >= bufferSize)
    return 0;

  mm_event_type_t type = buffer[offset] >> 4;
  mm_channel_t channel = buffer[offset++] & 0xF;

  event->type = type;
  event->deltaTime = deltaTime;

  switch (type)
  {
  case MM_EVENT_TYPE_NOTE_OFF:
  case MM_EVENT_TYPE_NOTE_ON:
  case MM_EVENT_TYPE_NOTE_AFTERTOUCH:
  {
    event->note.channel = channel;
    event->note.note = buffer[offset++];
    event->note.amount = buffer[offset++];
    break;
  }
  default:
    return 0;
  }

  return offset;
}
