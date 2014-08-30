DIR=$(shell pwd)

INCLUDES = \
	-I$(DIR) \
	-I$(DIR)/arduino

SRCS = \
	arduino/main.cpp \
	arduino/platform.cpp \
	arduino/Print.cpp \
	arduino/Stream.cpp \
	arduino/Tone.cpp \
	arduino/WInterrupts.c \
	arduino/wiring.c \
	arduino/wiring_analog.c \
	arduino/wiring_digital.c \
	arduino/wiring_pulse.c \
	arduino/wiring_shift.c \
	arduino/WMath.cpp \
	arduino/WString.cpp \
	arduino/Serial.cpp \
	arduino/dht.cpp

OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRCS)))

%.o: %.cpp
	@rm -f $@
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@ -Wno-deprecated-declarations -fPIC -fpermissive

%.o: %.c
	@rm -f $@
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -Wno-deprecated-declarations -fPIC


LIB_STATIC = libarduino.a
LIB_SHARE = libarduino.so
#LIB = $(LIB_STATIC) $(LIB_SHARE)
LIB = $(LIB_STATIC)
PROGRAM=arduigrow

all: $(PROGRAM)

$(PROGRAM): $(LIB)
	$(CXX) $(CFLAGS) $(INCLUDES) -o $(PROGRAM) arduigrow.c $(LIB_STATIC)
	$(CXX) $(CFLAGS) $(INCLUDES) -o ph_calibrate ph_calibrate.c $(LIB_STATIC)


$(LIB): $(OBJS)
	$(AR) cq $(LIB_STATIC) $(OBJS)
#	$(CXX) -shared -Wl,-soname,$(LIB_SHARE) -o $(LIB_SHARE) $(OBJS)

install: $(PREFIX)/usr/bin/$(PROGRAM)
	install $(PROGRAM) $(PREFIX)/usr/bin/$(PROGRAM)

clean:
	rm -f $(LIB_STATIC) $(LIB_SHARE) $(OBJS) $(PROGRAM) ph_calibrate
