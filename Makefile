# all: compile link run

# compile:
# 	g++ -static -I src/include -c  src/main.cpp src/aircraft.cpp src/simvar.cpp src/Hydraulics.cpp src/structure.cpp src/render.cpp src/interp.cpp src/user.cpp  src/RungeKutta.cpp src/pid.cpp src/interact_var.cpp src/gui/gui.cpp
# link:
# 	g++ main.o aircraft.o simvar.o hydraulics.o structure.o render.o interp.o user.o RungeKutta.o pid.o interact_var.o gui.o  -o main -L src/lib -l sfml-graphics -l sfml-window -l sfml-system  
# run:
# 	./main.exe

all: compile link run

IMGUI_DIR := imgui

compile:
	g++ -static -I src/include -I $(IMGUI_DIR) \
	    -c src/main.cpp src/aircraft.cpp src/simvar.cpp src/Hydraulics.cpp \
	    src/structure.cpp src/render.cpp src/interp.cpp src/user.cpp \
	    src/RungeKutta.cpp src/pid.cpp src/interact_var.cpp src/helper.cpp src/gui/gui.cpp \
	    $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp \
	    $(IMGUI_DIR)/imgui-SFML.cpp 

link:
	g++ main.o aircraft.o simvar.o hydraulics.o structure.o render.o interp.o user.o RungeKutta.o pid.o interact_var.o helper.o gui.o imgui.o imgui_draw.o imgui_tables.o imgui_widgets.o imgui-SFML.o  \
	    -o main -L src/lib -l sfml-graphics -l sfml-window -l sfml-system -l opengl32

run:
	./main.exe


