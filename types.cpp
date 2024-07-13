#include "types.hpp"



Note getNoteFromIndex(int index)
{
   switch (index)
   {
   case 0:  return Note::A;
   case 1:  return Note::As;
   case 2:  return Note::B;
   case 3:  return Note::C;
   case 4:  return Note::Cs;
   case 5:  return Note::D;
   case 6:  return Note::Ds;
   case 7:  return Note::E;
   case 8:  return Note::F;
   case 9:  return Note::Fs;
   case 10: return Note::G;
   case 11: return Note::Gs;
   default: return Note::A;
   }
}

std::ostream& operator<<(std::ostream& out, const Note& note)
{
   switch(note)
   {
      case Note::A:  out << "A";  break;
      case Note::As: out << "A#"; break;
      case Note::B:  out << "B";  break;
      case Note::C:  out << "C";  break;
      case Note::Cs: out << "C#"; break;
      case Note::D:  out << "D";  break;
      case Note::Ds: out << "D#"; break;
      case Note::E:  out << "E";  break;
      case Note::F:  out << "F";  break;
      case Note::Fs: out << "F#"; break;
      case Note::G:  out << "G";  break;
      case Note::Gs: out << "G#"; break;
   }
   return out;
}