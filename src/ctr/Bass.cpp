#include "Bass.h"

#include "../Music.h"
#include "../cfg/Chord.h"
#include "../cfg/Instrument.h"

using namespace steve;

Bass::Bass(Music* music)
  : Creator(music) {}
void Bass::init() {
  Creator::init();

// Dasho - Min and Max tone match the instruments
// that are valid for bass (IDs 32-39); original
// values commented out below
  _min_tone = 24;
  _max_tone = 60;

  //_min_tone = 36;
  //_max_tone = _min_tone + 12;
}
Notes Bass::get(size_t start, size_t size) const {
  Notes notes;
  auto times = generate_times(start, size, true);
  uintptr_t i = 0;
  while(i < size) {
    for(uintptr_t j = 0; j < times.size() - 1; j++) {
      const auto duration = times[j + 1] - times[j];
      const Chord& chord = _music->chord_at(start + i);
      const uint8_t tone = _min_tone + chord.key;
      add_note(notes, tone, i, duration);
      i += duration;
    }
  }
  return notes;
}

// Dasho - Select bass by instrument itself instead of tone;
// original function commented out below
bool Bass::is_valid_instrument(const Instrument &instrument) const
{
    return instrument.midi_id >= 32 && instrument.midi_id <= 39;
}

//bool Bass::is_valid_instrument(const Instrument& instrument) const {
//  return instrument.min_tone <= 36 && instrument.max_tone >= 48;
//}
