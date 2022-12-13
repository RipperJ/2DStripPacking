CCC = g++ 
FLAGS = -g -Wall -Wextra 
GRBPATH = $(GUROBI_HOME)

exec: main.cpp Floorplan.cpp MILPFloorplan.cpp ShelfFloorplan.cpp
	$(CCC) $(FLAGS) main.cpp Floorplan.cpp MILPFloorplan.cpp ShelfFloorplan.cpp -o floorplan -I$(GRBPATH)/include -L$(GRBPATH)/lib -lgurobi_c++ -lgurobi100 -I$(GRAPHICS_CPP_INCLUDE) -L$(GRAPHICS_CPP_LIB) -lgraph

clean: 
	rm -f floorplan *.log
