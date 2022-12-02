FLCON = fltk-config
COMPILER = $(shell $(FLCON) --cxx)
CFLAGS = $(shell $(FLCON) --use-images --cxxflags) -I../
GAME_LDFLAGS = -lgametools -lweapons -lnpcs
LDFLAGS = $(GAME_LDFLAGS) $(shell $(FLCON) --use-images --ldstaticflags) \
	  -static-libstdc++ -static-libgcc -L../../lib/

TARGET = zombie_friends_launcher
DEH = ../*.cpp ../*.h
OBJS = main.o launcher_window.o launcher_top_menu.o launcher.o
LAUNCHER = launcher.o
LIB_DIR = ../../lib/

%.o: %.cpp
	@echo compiling $<
	@$(COMPILER) $(CFLAGS) -c -o $@ $<
$(TARGET): $(OBJS)
	@echo making exe
	@$(COMPILER) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@cp $(TARGET) ../../$(TARGET)
	@echo Creating liblauncher.a
	@ar rcs -o $(LIB_DIR)liblauncher.a $(LAUNCHER)
debug:
	$(COMPILER) $(CFLAGS) -c *.cpp -g
	$(COMPILER) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) -g

# Objects.
main.o: *.cpp *.h $(DEH)
launcher_window.o: launcher_window.cpp launcher_window.h launcher.cpp launcher.h $(DEH)
launcher_top_menu.o: launcher_top_menu.cpp launcher_top_menu.h launcher.cpp launcher.h $(DEH)
launcher.o: launcher.cpp launcher.h $(DEH)

clean:
	rm *.o
	rm $(TARGET)
