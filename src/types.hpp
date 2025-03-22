#pragma once

#include <utility>
#include <vector>
#include <iosfwd>
#include <span>
#include <string>

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

struct NoteSet {
   std::vector<NoteOctave> notes;
   std::string name;
};

int mapNoteToToneIndex(NoteOctave note_octave);
int mapNoteToStaffIndex(NoteOctave note_octave);
NoteModifier getModifier(Note note);

bool notesAreEnharmonic(Note note1, Note note2);

std::vector<Note> getAllNotes();
std::vector<Note> getNotesForKey(Note note, Key key);

std::vector<NoteOctave> noteOctavesForFirstPosition();
std::vector<NoteOctave> noteOctavesForEString();
std::vector<NoteOctave> noteOctavesForAString();
std::vector<NoteOctave> noteOctavesForDString();
std::vector<NoteOctave> noteOctavesForGString();
std::vector<NoteOctave> noteOctavesForBString();
std::vector<NoteOctave> noteOctavesForeString();

std::vector<NoteOctave> notesInOctaves(std::span<const Note> notes, std::span<const int> octaves);

std::ostream & operator<<(std::ostream &out, Note note);
