all:
	$(CXX) -shared -fPIC --no-gnu-unique ./src/main.cpp -o ./hyprglot.so -g `pkg-config --cflags pixman-1 libdrm hyprland pangocairo libinput libudev wayland-server xkbcommon` -std=c++26 -Wno-narrowing
clean:
	rm ./hyprglot.so
