FLCON = fltk-config
COMPILER = $(shell $(FLCON) --cxx)
CFLAGS = $(shell $(FLCON) --use-images --cxxflags) -I../ -I../../
GAME_LDFLAGS = -lweapons -lnpcs -llauncher -lgametools
LDFLAGS = $(GAME_LDFLAGS) $(shell $(FLCON) --use-images --ldstaticflags) \
	  -static-libstdc++ -static-libgcc -L../../../lib/

TARGET = cmd_launcher
DEH = ../*.cpp ../*.h ../../*.cpp ../../*.h
OBJS = main.o

%.o: %.cpp
	@echo compiling $<
	@$(COMPILER) $(CFLAGS) -c -o $@ $<
$(TARGET): $(OBJS)
	@echo making exe
	@$(COMPILER) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@cp $(TARGET) ../../../$(TARGET)
debug:
	$(COMPILER) $(CFLAGS) -c *.cpp -g
	$(COMPILER) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) -g

main.o: main.cpp $(DEH)

# Objects.
clean:
	rm *.o
	rm $(TARGET)
