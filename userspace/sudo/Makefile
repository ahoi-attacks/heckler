CC = gcc
CFLAGS =

.PHONY: all
all: profile_pam

CFLAGS=-g -I.

profile_pam: profile_pam.c virt_to_phy.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm profile_sudo || true
	rm profile_pam || true
	rm libsample.so profile_sample sample_consumer || true

