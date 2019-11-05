TARGETS = server

all: $(TARGETS)

server:
	$(MAKE) -C server

.PHONY: all $(TARGETS)