gcc input.c -o input -m32 -L\./libs/bass/ -l bass
gcc main.c outputvideo.c -o output -lm -L\./libs/gd/lib -l gd
