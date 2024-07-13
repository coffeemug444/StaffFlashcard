#pragma once
#include <ostream>

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

Note getNoteFromIndex(int index);
std::ostream& operator<<(std::ostream& out, const Note& note);