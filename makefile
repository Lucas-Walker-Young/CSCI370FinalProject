G = g++-4.9 -std=c++11
C = -I/usr/local/lib/Oracle/instantclient_11_2/sdk/include
L = -L/usr/local/lib/Oracle/instantclient_11_2 -locci -lociei

project: build/main.o build/AsteroidControl.o build/AsteroidSelect.o build/DestinationSelect.o build/DroneControl.o build/DroneHaul.o build/DroneMove.o build/DroneSelect.o build/RefineryControl.o build/RefinerySelect.o build/StockpileControl.o build/StockpileSelect.o build/TimeController.o build/Facility.o build/Ore.o build/OreType.o build/Refinery.o build/Stockpile.o build/IO.o
	$(G) $(L) -o project build/*.o

build/main.o: src/main.cpp include/AsteroidControl.hpp include/AsteroidSelect.hpp include/DestinationSelect.hpp include/DroneControl.hpp include/DroneHaul.hpp include/DroneMove.hpp include/DroneSelect.hpp include/RefineryControl.hpp include/RefinerySelect.hpp include/StockpileControl.hpp include/StockpileSelect.hpp include/TimeController.hpp include/IO.hpp
	$(G) $(C) -c src/main.cpp -o build/main.o

build/AsteroidControl.o: src/Controllers/AsteroidControl.cpp include/AsteroidControl.hpp include/IO.hpp include/Asteroid.hpp include/OreType.hpp
	$(G) $(C) -c src/Controllers/AsteroidControl.cpp -o build/AsteroidControl.o

build/AsteroidSelect.o: src/Controllers/AsteroidSelect.cpp include/AsteroidSelect.hpp include/IO.hpp include/AsteroidControl.hpp include/Asteroid.hpp
	$(G) $(C) -c src/Controllers/AsteroidSelect.cpp -o build/AsteroidSelect.o

build/DestinationSelect.o: src/Controllers/DestinationSelect.cpp include/DestinationSelect.hpp include/IO.hpp include/Drone.hpp include/Ore.hpp include/Facility.hpp
	$(G) $(C) -c src/Controllers/DestinationSelect.cpp -o build/DestinationSelect.o

build/DroneControl.o: src/Controllers/DroneControl.cpp include/DroneControl.hpp include/IO.hpp include/Drone.hpp include/DroneHaul.hpp include/DroneMove.hpp
	$(G) $(C) -c src/Controllers/DroneControl.cpp -o build/DroneControl.o

build/DroneHaul.o: src/Controllers/DroneHaul.cpp include/DroneHaul.hpp include/IO.hpp include/Drone.hpp include/Ore.hpp
	$(G) $(C) -c src/Controllers/DroneHaul.cpp -o build/DroneHaul.o

build/DroneMove.o: src/Controllers/DroneMove.cpp include/DroneMove.hpp include/IO.hpp include/Drone.hpp include/Facility.hpp
	$(G) $(C) -c src/Controllers/DroneMove.cpp -o build/DroneMove.o

build/DroneSelect.o: src/Controllers/DroneSelect.cpp include/DroneSelect.hpp include/IO.hpp include/DroneControl.hpp include/Drone.hpp
	$(G) $(C) -c src/Controllers/DroneSelect.cpp -o build/DroneSelect.o

build/RefineryControl.o: src/Controllers/RefineryControl.cpp include/RefineryControl.hpp include/IO.hpp include/Refinery.hpp include/Ore.hpp
	$(G) $(C) -c src/Controllers/RefineryControl.cpp -o build/RefineryControl.o

build/RefinerySelect.o: src/Controllers/RefinerySelect.cpp include/RefinerySelect.hpp include/IO.hpp include/RefineryControl.hpp include/Refinery.hpp
	$(G) $(C) -c src/Controllers/RefinerySelect.cpp -o build/RefinerySelect.o

build/StockpileControl.o: src/Controllers/StockpileControl.cpp include/StockpileControl.hpp include/IO.hpp include/Stockpile.hpp include/Ore.hpp
	$(G) $(C) -c src/Controllers/StockpileControl.cpp -o build/StockpileControl.o

build/StockpileSelect.o: src/Controllers/StockpileSelect.cpp include/StockpileSelect.hpp include/IO.hpp include/StockpileControl.hpp include/Stockpile.hpp
	$(G) $(C) -c src/Controllers/StockpileSelect.cpp -o build/StockpileSelect.o

build/TimeController.o: src/Controllers/TimeController.cpp include/TimeController.hpp include/IO.hpp
	$(G) $(C) -c src/Controllers/TimeController.cpp -o build/TimeController.o

build/Facility.o: src/Queries/Facility.cpp include/Facility.hpp 
	$(G) $(C) -c src/Queries/Facility.cpp -o build/Facility.o

build/Ore.o: src/Queries/Ore.cpp include/Ore.hpp 
	$(G) $(C) -c src/Queries/Ore.cpp -o build/Ore.o

build/OreType.o: src/Queries/OreType.cpp include/OreType.hpp 
	$(G) $(C) -c src/Queries/OreType.cpp -o build/OreType.o

build/Refinery.o: src/Queries/Refinery.cpp include/Refinery.hpp 
	$(G) $(C) -c src/Queries/Refinery.cpp -o build/Refinery.o

build/Stockpile.o: src/Queries/Stockpile.cpp include/Stockpile.hpp 
	$(G) $(C) -c src/Queries/Stockpile.cpp -o build/Stockpile.o

build/IO.o: src/Utilities/IO.cpp include/IO.hpp include/IO.hpp
	$(G) $(C) -c src/Utilities/IO.cpp -o build/IO.o
