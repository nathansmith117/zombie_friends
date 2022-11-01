FLCON = fltk-config
COMPILER = $(shell $(FLCON) --cxx)
CFLAGS = $(shell $(FLCON) --use-images --cxxflags) -I../
GAME_LDFLAGS = -lgametools -lweapons -lnpcs
LDFLAGS = $(GAME_LDFLAGS) $(shell $(FLCON) --use-images --ldstaticflags) -static-libstdc++ -static-libgcc -L../../lib/

TARGET = level_edit
DEH = ../*.cpp ../*.h
OBJS = main.o editWindow.o edit_menu.o edit_tab_menu.o edit_tile_creater.o edit_commands.o edit_item_creater.o \
       edit_item_command_list.o edit_npc_creater.o

%.o: %.cpp
	@echo compiling $<
	@$(COMPILER) $(CFLAGS) -c -o $@ $<
$(TARGET): $(OBJS)
	@echo making exe
	@$(COMPILER) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@cp $(TARGET) ../../$(TARGET)
debug:
	$(COMPILER) $(CFLAGS) -c *.cpp -g
	$(COMPILER) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) -g

# Objects.
main.o: *.cpp *.h ../*.h ../*.cpp $(DEH)
editWindow.o: editWindow.cpp editWindow.h edit_tab_menu.cpp edit_tab_menu.h edit_tile_creater.cpp edit_tile_creater.h \
	edit_menu.cpp edit_menu.h edit_npc_creater.cpp edit_npc_creater.h $(DEH)
edit_menu.o: edit_menu.cpp edit_menu.h $(DEH)
edit_tab_menu.o: edit_tab_menu.cpp edit_tab_menu.h edit_tile_creater.cpp edit_tile_creater.h edit_commands.cpp edit_commands.h \
	edit_item_creater.cpp edit_item_creater.h  \
	edit_item_command_list.cpp edit_item_command_list.h edit_npc_creater.cpp edit_npc_creater.h $(DEH)
edit_tile_creater.o: edit_tile_creater.cpp edit_tile_creater.h $(DEH)
edit_commands.o: edit_commands.cpp edit_commands.h edit_tile_creater.cpp edit_tile_creater.h edit_tab_menu.cpp edit_tab_menu.h $(DEH)
edit_item_creater.o: edit_item_creater.cpp edit_item_creater.h $(DEH)
edit_item_command_list.o: edit_item_command_list.cpp edit_item_command_list.h $(DEH)
edit_npc_creater.o: edit_npc_creater.cpp edit_npc_creater.h $(DEH)

clean:
	rm *.o
	rm $(TARGET)
