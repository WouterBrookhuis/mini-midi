#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum
{
  MM_CHANNEL_0,
  MM_CHANNEL_1,
  MM_CHANNEL_2,
  MM_CHANNEL_3,
  MM_CHANNEL_4,
  MM_CHANNEL_5,
  MM_CHANNEL_6,
  MM_CHANNEL_7,
  MM_CHANNEL_8,
  MM_CHANNEL_9,
  MM_CHANNEL_10,
  MM_CHANNEL_11,
  MM_CHANNEL_12,
  MM_CHANNEL_13,
  MM_CHANNEL_14,
  MM_CHANNEL_15,
} mm_channel_t;

typedef enum
{
  MM_EVENT_TYPE_NOTE_OFF = 0x8,
  MM_EVENT_TYPE_NOTE_ON = 0x9,
  MM_EVENT_TYPE_NOTE_AFTERTOUCH = 0xA,
  MM_EVENT_TYPE_CONTROLLER = 0xB,
  MM_EVENT_TYPE_PROGRAM_CHANGE = 0xC,
  MM_EVENT_TYPE_CHANNEL_AFTERTOUCH = 0xD,
  MM_EVENT_TYPE_PITCH_BLEND = 0xE,
  MM_EVENT_TYPE_META = 0xF,
} mm_event_type_t;

typedef struct
{
  mm_channel_t channel;
  uint8_t note;
  uint8_t amount;
} mm_event_note_t;

typedef struct
{
  mm_event_type_t type;
  uint32_t deltaTime;
  union
  {
    mm_event_note_t note;
  };
} mm_event_t;

typedef struct
{
  uint32_t id;
  uint32_t size;
} mm_chunk_t;

typedef struct
{
  uint16_t format;
  uint16_t trackCount;
  uint16_t timeDivision;
} mm_header_t;

//typedef struct mm_player_t mm_player_t;

size_t mm_read_header(const uint8_t *buffer, size_t bufferSize, mm_header_t *header);

size_t mm_read_track_chunk(const uint8_t *buffer, size_t bufferSize, uint32_t *trackSize);

size_t mm_read_track_event(const uint8_t *buffer, size_t bufferSize, mm_event_t *event);
