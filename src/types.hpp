#pragma once

#include <utility>
#include <vector>
#include <span>
#include <string>
#include <format>

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

template<>
struct std::formatter<Note>
{
   constexpr auto parse(std::format_parse_context& ctx) 
   {
      return ctx.begin();
   }

   constexpr auto format(const Note& obj, std::format_context& ctx) const 
   {
      std::string_view str = [&](){
         switch(obj){
         using enum Note;
         case Ab: return "Ab";
         case  A: return "A";
         case As: return "A#";
         case Bb: return "Bb";
         case  B: return "B";
         case Bs: return "B#";
         case Cb: return "Cb";
         case  C: return "C";
         case Cs: return "C#";
         case Db: return "Db";
         case  D: return "D";
         case Ds: return "D#";
         case Eb: return "Eb";
         case  E: return "E";
         case Es: return "E#";
         case Fb: return "Fb";
         case  F: return "F";
         case Fs: return "F#";
         case Gb: return "Gb";
         case  G: return "G";
         case Gs: return "G#";
         default: std::unreachable();
         }
      }();
      return std::format_to(ctx.out(), "{}", str);
   }
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

Note mapIndexToNote(int index);
int mapNoteToToneIndex(NoteOctave note_octave);
int mapNoteToStaffIndex(NoteOctave note_octave);
NoteModifier getModifier(Note note);

bool notesAreEnharmonic(Note note1, Note note2);

std::vector<Note> getAllNotes();
std::vector<Note> getNotesForKey(Note note, Key key);

std::vector<NoteOctave> noteOctavesForString(int string);
std::vector<NoteOctave> noteOctavesForPosition(int position);

std::vector<NoteOctave> notesInOctaves(std::span<const Note> notes, std::span<const int> octaves);
