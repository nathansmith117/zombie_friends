all:
	@make -C src
	@make -C src/level_edit
game:
	@make -C src
editor:
	@make -C src/level_edit
clean:
	@make -C src clean
	@make -C src/level_edit clean
%:
	@make -C src $@
