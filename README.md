# Staff Flashcard #

Flashcard program for memorising fretting positions of notes on the staff

# Installation #

Install SFML:

Arch: `sudo pacman -S sfml`

Ubuntu: `sudo apt install libsfml-dev`

Generate the build files and build the project:
`mkdir build && cd build && cmake .. && cmake --build .`

# Usage #
Navigate to `build/bin` and run `./StaffFlashcard` in your terminal. This will prompt you to select the microphone you wish to use, ie.

```
$ ./main
0. Rocksmith Guitar Adapter Mono
1. Monitor of Starship/Matisse HD Audio Controller Analog Stereo
2. Starship/Matisse HD Audio Controller Analog Stereo
3. Monitor of Navi 21/23 HDMI/DP Audio Controller Digital Stereo (HDMI)
```

Type the number corresponding to the microphone and the program will start. Now just play the notes as they appear on the screen
