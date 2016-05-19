GCC=gcc -g
SUBDIRS = main \
		  v4l2 \
		  image_process \
		  obj \

BIN = mainapp
OBJS = main.o v4l2.o image_process.o
OBJS_DIR = obj
BIN_DIR = bin
CFLAG = -I../include

export GCC OBJS OBJS_DIR CFLAG BIN BIN_DIR

all:CHECK_DIR $(SUBDIRS)
CHECK_DIR:
	@mkdir -p $(BIN_DIR)
$(SUBDIRS) : ECHO
	@make -s -C $@
	
ECHO:
	@echo $(SUBDIRS)
	@echo begin compile

clean:
	@$(RM) $(OBJS_DIR)/*.o
	@rm -rf $(BIN_DIR)
	@echo rm -rf $(OBJS_DIR)/*.o

