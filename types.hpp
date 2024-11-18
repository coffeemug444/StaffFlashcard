#pragma once

#include <iosfwd>

enum class Note
{
   A,
   As,
   B,
   C,
   Cs,
   D,
   Ds,
   E,
   F,
   Fs,
   G,
   Gs
};

enum class NoteModifier
{
   FLAT = -1,
   NATURAL = 0,
   SHARP = 1,
};

Note getNoteFromIndex(int index);
std::ostream& operator<<(std::ostream& out, const Note& note);