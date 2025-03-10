.PHONY: all debug release clean dev

debug:
	cmake -S . -B ./debug -DCMAKE_BUILD_TYPE=Debug
	cmake --build ./debug -j
	rm -rf ./compile_commands.json
	rm -rf ./hyprglot.so
	ln -s ./debug/compile_commands.json .
	ln -s ./debug/hyprglot.so .

release:
	cmake -S . -B ./release -DCMAKE_BUILD_TYPE=Release
	cmake --build ./release -j
	rm -rf ./compile_commands.json
	rm -rf ./hyprglot.so
	ln -s ./release/compile_commands.json .
	ln -s ./release/hyprglot.so .

all: clean release

clean:
	rm -rf ./release
	rm -rf ./debug
	rm -f ./compile_commands.json
	rm -f ./hyprglot.so

dev: clean debug
