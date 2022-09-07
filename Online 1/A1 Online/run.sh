g++ $1 -o main -lglut -lGLU -lGL

if [ $? -eq 0 ]; then
  ./main
  exit 0
fi

exit 1