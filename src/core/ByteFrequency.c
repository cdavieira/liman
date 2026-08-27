#include "core/ByteFrequency.h"
#include "platform/log.h"
#include "platform/mem.h"
#include "platform/posix/FileStream.h"

#include <stdlib.h>
#include <string.h>

#define SET_SIZE 256

struct ByteFrequency {
  unsigned byte;
  size_t count;
};

static int byteFreq_compare_callback(const void *b1, const void *b2);
static int byteFreq_fileStream_handler(int byte, void *smth);
static ByteFrequency *byteFreq_new(void);
static void byteFreq_sort(ByteFrequency *freq);
static void byteFreq_print(ByteFrequency *freq);

ByteFrequency *
byteFreq_from_string(const char *buffer)
{
  ByteFrequency *freq = byteFreq_new();
  size_t len = strlen(buffer);
  for (size_t i = 0; i < len; i++)
  {
    freq[(unsigned int)buffer[i]].count++;
  }

  byteFreq_sort(freq);

  return freq;
}

ByteFrequency *
byteFreq_from_filename(const char *filename)
{
  ByteFrequency *freq = byteFreq_new();

  size_t block_size = 1024 * 1024; // 1 MiB
  FileStream *fs = fs_new(block_size);
  fs_loop_over_all_bytes(
      fs, filename, (void *)freq, byteFreq_fileStream_handler);
  fs_destroy(fs);

  byteFreq_sort(freq);
  // byteFreq_print(freq);

  return freq;
}

ByteFrequency *
byteFreq_destroy(ByteFrequency *f)
{
  mem_free(f);
  return NULL;
}

unsigned
byteFreq_get_byte(ByteFrequency *bf, unsigned ch)
{
  return bf ? bf[ch].byte : '\0';
}

size_t
byteFreq_get_count(ByteFrequency *bf, unsigned ch)
{
  return bf ? bf[ch].count : 0;
}

size_t
byteFreq_get_total_bytes(ByteFrequency *bf)
{
  size_t total = 0;

  for (int i = 0; i < SET_SIZE; i++)
  {
    total += bf[i].count;
  }

  if (total > 0)
  {
    total--;
  }

  return total;
}

static int
byteFreq_compare_callback(const void *b1, const void *b2)
{
  return ((ByteFrequency *)b1)->count - ((ByteFrequency *)b2)->count;
}

static int
byteFreq_fileStream_handler(int byte, void *smth)
{
  ByteFrequency *bf = smth;
  bf[byte].count++;
  return 0;
}

static ByteFrequency *
byteFreq_new(void)
{
  ByteFrequency *freq = mem_zalloc(SET_SIZE * sizeof(ByteFrequency));
  for (int i = 0; i < SET_SIZE; i++)
  {
    freq[i].count = 0;
    freq[i].byte = i;
  }
  return freq;
}

static void
byteFreq_sort(ByteFrequency *freq)
{
  qsort(freq, SET_SIZE, sizeof(ByteFrequency), byteFreq_compare_callback);
}

static void
byteFreq_print(ByteFrequency *freq)
{
  for (int i = 0; i < SET_SIZE; i++)
  {
    log_info("%d: %zu", freq[i].byte, freq[i].count);
  }
}

size_t
byteFreq_get_set_size(void)
{
  return SET_SIZE;
}
