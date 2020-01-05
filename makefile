TARGETS = server client

all: $(TARGETS)

server:
	$(MAKE) -C server

client:
	$(MAKE) -C client

.PHONY: all $(TARGETS)