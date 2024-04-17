CC = gcc
CFLAGS =

.PHONY: all
all: profile_sample profile_pam libsample.so sample_consumer

CFLAGS=-g -I.

sample_consumer: sample_consumer.c libsample.so
	$(CC) $(CFLAGS) $^ -o $@  -lsample -L .

libsample.so: sample.c
	$(CC) -o $@ $^ -shared -fPIC


profile_sample: profile_sample.c virt_to_phy.c
	$(CC) $(CFLAGS) $^ -o $@


profile_pam: profile_pam.c virt_to_phy.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm profile_sudo || true
	rm profile_pam || true
	rm libsample.so profile_sample sample_consumer || true

