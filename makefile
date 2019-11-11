TARGETS = server client

all: $(TARGETS)

server:
	$(MAKE) -C server

client:
	mvn -f ./client/pom.xml clean package

.PHONY: all $(TARGETS)