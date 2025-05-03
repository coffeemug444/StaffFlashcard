#include "types.hpp"

#include <stdexcept>
#include <utility>
#include <ostream>
#include <ranges>
#include <cassert>

int mapNoteToStaffIndex(NoteOctave note_octave)
{
   auto [note, octave] = note_octave;

   int offset = 7*octave;

   switch (note)
   {
      using enum Note;
      case Eb:
      case E:
      case Es:
         return 0 + offset;
      case Fb:
      case F:
      case Fs:
         return 1 + offset;
      case Gb:
      case G:
      case Gs:
         return 2 + offset;
      case Ab:
      case A:
      case As:
         return 3 + offset;
      case Bb:
      case B:
      case Bs:
         return 4 + offset;
      case Cb:
      case C:
      case Cs:
         return 5 + offset;
      case Db:
      case D:
      case Ds:
         return 6 + offset;
   }
   std::unreachable();
}


int mapNoteToToneIndex(NoteOctave note_octave)
{
   auto [note, octave] = note_octave;

   switch (note)
   {
      using enum Note;
      case A : return 0;
      case As: return 1;
      case Bb: return 1;
      case B : return 2;
      case Bs: return 3;
      case Cb: return 2;
      case C : return 3;
      case Cs: return 4;
      case Db: return 4;
      case D : return 5;
      case Ds: return 6;
      case Eb: return 6;
      case E : return 7;
      case Es: return 8;
      case Fb: return 7;
      case F : return 8;
      case Fs: return 9;
      case Gb: return 9;
      case G : return 10;
      case Gs: return 11;
      case Ab: return 11;
   }
   std::unreachable();
}

NoteModifier getModifier(Note note)
{
   int note_as_num = static_cast<int>(note);
   switch (note_as_num % 3)
   {
   case 0: return NoteModifier::FLAT;
   case 1: return NoteModifier::NATURAL;
   case 2: return NoteModifier::SHARP;
   }
   std::unreachable();
}

bool notesAreEnharmonic(Note note1, Note note2)
{
   if (note1 == note2) return true;
   switch (note1)
   {
   using enum Note;
   case Ab: return note2 == Gs;
   case A:  return false;
   case As: return note2 == Bb;
   case Bb: return note2 == As;
   case B:  return note2 == Cb;
   case Bs: return note2 == C;
   case Cb: return note2 == B;
   case C:  return note2 == Bs;
   case Cs: return note2 == Db;
   case Db: return note2 == Cs;
   case D:  return false;
   case Ds: return note2 == Eb;
   case Eb: return note2 == Ds;
   case E:  return note2 == Fb;
   case Es: return note2 == F;
   case Fb: return note2 == E;
   case F:  return note2 == Es;
   case Fs: return note2 == Gb;
   case Gb: return note2 == Fs;
   case G:  return false;
   case Gs: return note2 == Ab;
   }

   return false;
}

std::vector<NoteOctave> notesInOctaves(std::span<const Note> notes, std::span<const int> octaves)
{
   std::vector<NoteOctave> out;
   for (auto [octave, note] : std::ranges::views::cartesian_product(octaves, notes))
   {
      out.push_back(NoteOctave{note, octave});
   }

   return out;
}

std::vector<NoteOctave> noteOctavesForFretPosition(int string, int fret)
{
   int note = fret;
   note += [string]() {
      switch (string)
      {
      case 0: return 0;
      case 1: return 5;
      case 2: return 10;
      case 3: return 15;
      case 4: return 19;
      case 5: return 24;
      default: assert(false);
      }
   }();

   int octave = note / 12;
   note = note % 12;

   switch (note)
   {
   using enum Note;
   case 0:  return {{E,  octave}, {Fb, octave}};
   case 1:  return {{Es, octave}, {F,  octave}};
   case 2:  return {{Fs, octave}, {Gb, octave}};
   case 3:  return {{G,  octave}              };
   case 4:  return {{Gs, octave}, {Ab, octave}};
   case 5:  return {{A,  octave}              };
   case 6:  return {{As, octave}, {Bb, octave}};
   case 7:  return {{B,  octave}, {Cb, octave}};
   case 8:  return {{Bs, octave}, {C,  octave}};
   case 9:  return {{Cs, octave}, {Db, octave}};
   case 10: return {{D,  octave}              };
   case 11: return {{Ds, octave}, {Eb, octave+1}};
   default: assert(false);
   }
}


std::vector<NoteOctave> noteOctavesForString(int string)
{
   using enum Note;
   namespace rv = std::ranges::views;

   return rv::join(rv::iota(0,13) | rv::transform([string](int x) { return noteOctavesForFretPosition(string, x); })) | std::ranges::to<std::vector>();
}



std::vector<NoteOctave> noteOctavesForPosition(int position)
{
   using enum Note;
   namespace rv = std::ranges::views;
   
   auto notes = rv::join(
      std::ranges::views::cartesian_product(rv::iota(0, 6), rv::iota(position, position + 4)) 
    | rv::transform([](auto string_fret) {
         auto [string, fret] = string_fret;
         return noteOctavesForFretPosition(string, fret);
      })) 
    | std::ranges::to<std::vector>();
   
   if (position == 1)
   {
      // add open strings
      notes.push_back({E, 0});
      notes.push_back({A, 0});
      notes.push_back({D, 0});
      notes.push_back({G, 1});
      notes.push_back({E, 2});
   }

   return notes;
}

std::vector<Note> getAllNotes()
{
   using enum Note;

   return {Ab, A, As, Bb, B, Bs, Cb, C, Cs, Db, D, Ds, Eb, E, Es, Fb, F, Fs, Gb, G, Gs};
}

std::vector<Note> getNotesForKey(Note note, Key key)
{
   using enum Note;
   
   const static std::vector a_major = {A , B , Cs, D , E , Fs, Gs};
   const static std::vector b_major = {B , Cs, Ds, E , Fs, Gs, As};
   const static std::vector c_major = {C , D , E , F , G , A , B };
   const static std::vector d_major = {D , E , Fs, G , A , B , Cs};
   const static std::vector e_major = {E , Fs, Gs, A , B , Cs, Ds};
   const static std::vector f_major = {F , G , A , Bb, C , D , E };
   const static std::vector g_major = {G , A , B , C , D , E , Fs};

   const static std::vector a_minor = {A , B , C , D , E , F , G };
   const static std::vector b_minor = {B , Cs, D , E , Fs, G , A };
   const static std::vector c_minor = {C , D , Eb, F , G , Ab, Bb};
   const static std::vector d_minor = {D , E , F , G , A , Bb, C };
   const static std::vector e_minor = {E , Fs, G , A , B , C , D };
   const static std::vector f_minor = {F , G , Ab, Bb, C , Db, Eb};
   const static std::vector g_minor = {G , A , Bb, C , D , Eb, F };

   const static std::vector a_major_pentatonic = {A , B , Cs, E , Fs};
   const static std::vector b_major_pentatonic = {B , Cs, Ds, Fs, Gs};
   const static std::vector c_major_pentatonic = {C , D , E , G , A };
   const static std::vector d_major_pentatonic = {D , E , Fs, A , B };
   const static std::vector e_major_pentatonic = {E , Fs, Gs, B , Cs};
   const static std::vector f_major_pentatonic = {F , G , A , C , D };
   const static std::vector g_major_pentatonic = {G , A , B , D , E };

   const static std::vector a_minor_pentatonic = {A , C , D , E , G };
   const static std::vector b_minor_pentatonic = {B , D , E , Fs, A };
   const static std::vector c_minor_pentatonic = {C , Eb, F , G , Bb};
   const static std::vector d_minor_pentatonic = {D , F , G , A , C };
   const static std::vector e_minor_pentatonic = {E , G , A , B , D };
   const static std::vector f_minor_pentatonic = {F , Ab, Bb, C , Eb};
   const static std::vector g_minor_pentatonic = {G , Bb, C , D , F };

   switch (note)
   {
      case Ab:
      case As:
      case Bb:
      case Bs:
      case Cb:
      case Cs:
      case Db:
      case Ds:
      case Eb:
      case Es:
      case Fb:
      case Fs:
      case Gb:
      case Gs:
         throw std::runtime_error("Sharps and flats aren't supported (yet (ever?))");
      default:
         break;
   }

   switch (key)
   {
   case Key::MAJOR:
   {
      switch (note)
      {
         case A: return a_major;
         case B: return b_major;
         case C: return c_major;
         case D: return d_major;
         case E: return e_major;
         case F: return f_major;
         case G: return g_major;
         default: return {};
      }
   }
   case Key::MINOR:
   {
      switch (note)
      {
         case A: return a_minor;
         case B: return b_minor;
         case C: return c_minor;
         case D: return d_minor;
         case E: return e_minor;
         case F: return f_minor;
         case G: return g_minor;
         default: return {};
      }
   }
   case Key::MAJOR_PENTATONIC:
   {
      switch (note)
      {
         case A: return a_major_pentatonic;
         case B: return b_major_pentatonic;
         case C: return c_major_pentatonic;
         case D: return d_major_pentatonic;
         case E: return e_major_pentatonic;
         case F: return f_major_pentatonic;
         case G: return g_major_pentatonic;
         default: return {};
      }
   }
   case Key::MINOR_PENTATONIC:
   {
      switch (note)
      {
         case A: return a_minor_pentatonic;
         case B: return b_minor_pentatonic;
         case C: return c_minor_pentatonic;
         case D: return d_minor_pentatonic;
         case E: return e_minor_pentatonic;
         case F: return f_minor_pentatonic;
         case G: return g_minor_pentatonic;
         default: return {};
      }
   }
   }
   std::unreachable();
}

std::ostream & operator<<(std::ostream &out, Note note)
{
   using enum Note;
   switch (note)
   {
   case Ab: return out << "Ab";
   case A : return out << "A" ;
   case As: return out << "As";
   case Bb: return out << "Bb";
   case B : return out << "B" ;
   case Bs: return out << "Bs";
   case Cb: return out << "Cb";
   case C : return out << "C" ;
   case Cs: return out << "Cs";
   case Db: return out << "Db";
   case D : return out << "D" ;
   case Ds: return out << "Ds";
   case Eb: return out << "Eb";
   case E : return out << "E" ;
   case Es: return out << "Es";
   case Fb: return out << "Fb";
   case F : return out << "F" ;
   case Fs: return out << "Fs";
   case Gb: return out << "Gb";
   case G : return out << "G" ;
   case Gs: return out << "Gs";
   }
   return out;
}
