all: compile link

compile:
	g++ -I src/include -c src/main.cpp src/hydraulics.cpp
link:
	g++ main.o hydraulics.o -o main -L src/lib -l sfml-graphics -l sfml-window -l sfml-system
