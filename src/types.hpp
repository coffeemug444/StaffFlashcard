#pragma once

#include <utility>
#include <vector>

enum class BetterNote
{
   Ab,
   A,
   As,
   Bb,
   B,
   Bs,
   Cb,
   C,
   Cs,
   Db,
   D,
   Ds,
   Eb,
   E,
   Es,
   Fb,
   F,
   Fs,
   Gb,
   G,
   Gs
};

enum class Key
{
   MAJOR,
   MINOR,
   MAJOR_PENTATONIC,
   MINOR_PENTATONIC
};

enum class NoteModifier
{
   FLAT = -1,
   NATURAL = 0,
   SHARP = 1,
};

using NoteOctave = std::pair<BetterNote, int>;

int mapNoteToToneIndex(NoteOctave note_octave);
int mapNoteToStaffIndex(NoteOctave note_octave);
NoteModifier getModifier(BetterNote note);

std::vector<BetterNote> getAllNotes();
std::vector<BetterNote> getNotesForKey(BetterNote note, Key key);
