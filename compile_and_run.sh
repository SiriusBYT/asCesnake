while true; do
clear
echo "=== Compiling ==="
gcc -o asCesnake.lxf-amd64 main.c -lncurses -lpthread
echo "=== Program Compiled ==="
read -p "Press enter to run the last successfull compiled spaget code" < /dev/tty
./asCesnake.lxf-amd64
done