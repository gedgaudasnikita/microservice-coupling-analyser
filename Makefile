matavimo_irankis: clean compile;

all: matavimo_irankis;

compile: prepare_worktree;
	cd build; g++ -c */*.cpp */*/*.cpp */*/*/*.cpp */*/*/*/*.cpp -lstdc++fs -I./lib/yaml-cpp/include -Wfatal-errors -std=gnu++0x; g++ -o matavimo_irankis *.o -lstdc++fs ;
	mv build/matavimo_irankis target/matavimo_irankis;

prepare_worktree:
	mkdir -p build;
	mkdir -p build/src;
	mkdir -p build/lib;
	mkdir -p target;
	cp -R src/. build/src;
	cp -R lib/. build/lib;

clean: 
	rm -rf build target;

run: matavimo_irankis;
	chmod +x target/matavimo_irankis;
	./target/matavimo_irankis --type=kubernetes --working-dir=./test/kubernetes;