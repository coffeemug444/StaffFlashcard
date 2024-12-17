#pragma once

#include <utility>
#include <vector>
#include <iosfwd>

enum class Note
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

using NoteOctave = std::pair<Note, int>;

int mapNoteToToneIndex(NoteOctave note_octave);
int mapNoteToStaffIndex(NoteOctave note_octave);
NoteModifier getModifier(Note note);

std::vector<Note> getAllNotes();
std::vector<Note> getNotesForKey(Note note, Key key);

std::ostream & operator<<(std::ostream &out, Note note);
