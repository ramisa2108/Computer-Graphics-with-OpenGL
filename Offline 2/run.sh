# run with cpp file name as argument

g++ $1 -o main

if [ $? -eq 0 ]; then
    ./main
    exit 0
fi

exit 1