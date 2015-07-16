CC = gcc
CFLAGS = -g -O2
CPPFLAGS =
LDFLAGS =
LIBS = -lm
INSTALL_DIR = ../bin/

CREATE_OBJS = create_cl_params.o create_cl_params_aux.o sink_utils.o \
              create_sink.o
DATA_OBJS = data_cl_params.o data_cl_params_aux.o sink_utils.o data_sink.o
VACUUM_OBJS = vacuum_cl_params.o vacuum_cl_params_aux.o sink_utils.o \
              vacuum_sink.o
SPLIT_OBJS = split_cl_params.o split_cl_params_aux.o sink_utils.o \
             split_sink.o
INFO_OBJS = info_cl_params.o info_cl_params_aux.o sink_utils.o \
            info_sink.o
WELL_OBJS = well_cl_params.o well_cl_params_aux.o sink_utils.o data_well.o

APPS = data_sink create_sink vacuum_sink split_sink info_sink data_well

all: $(APPS)

data_sink: $(DATA_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(DATA_OBJS) $(LDFLAGS) $(LIBS)

create_sink: $(CREATE_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(CREATE_OBJS) $(LDFLAGS) $(LIBS)

vacuum_sink: $(VACUUM_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(VACUUM_OBJS) $(LDFLAGS) $(LIBS)

split_sink: $(SPLIT_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(SPLIT_OBJS) $(LDFLAGS) $(LIBS)

info_sink: $(INFO_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(INFO_OBJS) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

data_well: $(WELL_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(WELL_OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o $(APPS) core

install: all
	install create_sink $(INSTALL_DIR)
	install data_sink $(INSTALL_DIR)
	install vacuum_sink $(INSTALL_DIR)
	install split_sink $(INSTALL_DIR)
	install info_sink $(INSTALL_DIR)
	install data_well $(INSTALL_DIR)

weave: create_cl_params.h vacuum_cl_params.h data_cl_params.h \
       split_cl_params.h info_cl_params.h well_cl_params.h

%_cl_params.h: %_options.txt
	weave -l C -b `basename $@ .h` -d $<

clean_weave:
	rm -f *_cl_params*
