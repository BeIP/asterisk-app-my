MODULE = app_my.so
VERSION = 0.1

HEADERS = config.h database.h query.h
SOURCES = config.c database.c query.c app_my.c
OBJECTS = $(SOURCES:%.c=%.o)
VSCRIPT = $(MODULE:%.so=%.ver)

CFLAGS += -Wall -fPIC -O0 -g
LDFLAGS += -Wall -fPIC -O0 -g

all: $(MODULE)

$(OBJECTS): %.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -D_GNU_SOURCE -DAST_MODULE=\"$(MODULE:%.so=%)\" -DVERSION=\"$(VERSION)\" -o $@ -c $<

$(MODULE): $(OBJECTS) $(VSCRIPT)
	$(CC) $(LDFLAGS) --shared -lmysqlclient_r -Wl,-version-script -Wl,$(VSCRIPT) -o $@ $(OBJECTS)

clean:
	$(RM) -f $(MODULE) $(OBJECTS)

install:
	install -d $(DESTDIR)/usr/lib/asterisk/modules
	cp -a $(MODULE) $(DESTDIR)/usr/lib/asterisk/modules
	install -d $(DESTDIR)/var/lib/asterisk/agi-bin
	install -m 755 $(AGI) $(DESTDIR)/var/lib/asterisk/agi-bin

.PHONY: all install clean
