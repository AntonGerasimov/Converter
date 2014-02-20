clear
echo "Проект студентов ФОПФ по визуализации звука"
export LD_LIBRARY_PATH="./libs/bass"
./input
export LD_LIBRARY_PATH="./libs/gd"
./output
if test -f out.mp4; then rm -f out.mp4; fi
if test -f picture0001.gif; then avconv -i "picture%04d.gif" -q 1 -s hd720 -threads 8 -y out.mp4; fi
if test -f picture0001.gif; then rm -f picture*.png; fi
./clear.sh
clear
if test -f out.mp4; then echo Videofile was done successfully; fi
echo Thank you for using our program

