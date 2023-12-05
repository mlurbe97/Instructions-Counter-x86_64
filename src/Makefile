## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022

TOPDIR  := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)/..

include $(TOPDIR)/config.mk
include $(TOPDIR)/rules.mk

DIRS=

ifeq ($(ARCH),ia64)
#DIRS +=ia64
endif

ifeq ($(ARCH),x86_64)
DIRS += x86
endif

ifeq ($(ARCH),i386)
DIRS += x86
endif

CFLAGS+= -I. -D_GNU_SOURCE -pthread

ifeq ($(BENCHES),GEEK_BENCHMARKS)
CFLAGS+=-DGEEK_BENCHMARKS
endif

ifeq ($(BENCHES),SPEC_BENCHMARKS)
CFLAGS+=-DSPEC_BENCHMARKS
endif

ifeq ($(BENCHES),GAP_BENCHMARKS)
CFLAGS+=-DGAP_BENCHMARKS
endif

PERF_EVENT_HDR=$(TOPDIR)/include/perfmon/pfmlib_perf_event.h

LPC_UTILS=perf_util.o
LPC_UTILS_HDR=perf_util.h

TARGETS+=Instructions_counter_x86_64

# Make rtop conditional on ncurses development package installed
ifeq ($(shell /bin/echo -e '\#include <ncurses.h>\nint main(void) { return 0;}' | $(CC) -o /dev/null -xc - 2>/dev/null && echo -n yes), yes)
RTOP=rtop
endif

EXAMPLESDIR=$(DESTDIR)$(DOCDIR)/perf_examples

all: $(TARGETS) $(RTOP)
	@set -e ; for d in $(DIRS) ; do $(MAKE) -C $$d $@ ; done

rtop: rtop.o $(LPC_UTILS) $(PFMLIB) $(PERF_EVENT_HDR)
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $< $(LPC_UTILS) $(PFMLIB) $(LIBS) -lpthread -lncurses -ltinfo -lm

$(TARGETS): %:%.o $(LPC_UTILS) $(PFMLIB)
	-$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $< $(LPC_UTILS) $(PFMLIB) $(LIBS) 

$(LPC_UTILS): $(LPC_UTILS_HDR)

clean:
	@set -e ; for d in $(DIRS) ; do $(MAKE) -C $$d $@ ; done
	$(RM) -f *.o $(TARGETS) rtop *~

distclean: clean

install-examples install_examples: $(TARGETS)
	@echo installing: $(TARGETS)
	-mkdir -p $(EXAMPLESDIR)
	$(INSTALL) -m 755 $(TARGETS) $(EXAMPLESDIR)
	@set -e ; for d in $(DIRS) ; do $(MAKE) -C $$d $@ ; done
#
# examples are installed as part of the RPM install, typically in /usr/share/doc/libpfm-X.Y/
#
.PHONY: install depend install_examples install-examples
