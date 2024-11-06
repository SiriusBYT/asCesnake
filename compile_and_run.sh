clear
while true; do:
gcc -o asCesnake main.c -lncurses -lpthread
read -p "Press any key to run the last successfull compiled spaget code" < /dev/tty
./asCesnake
done