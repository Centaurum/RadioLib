CC=gcc
#INCLUDE= ../include
INCLUDE=.
#LDFLAGS=-L ../libspeex -lm -lspeex
LDFLAGS=-L. -lm -lspeex
CFLAGS=-c -I $(INCLUDE)
ARFLAGS=-I $(INCLUDE)

SOURCES=modes.c misc.c bits.c  speex_callbacks.c gain_table.c gain_table_lbr.c exc_10_16_table.c exc_10_32_table.c exc_20_32_table.c exc_5_256_table.c exc_5_64_table.c exc_8_128_table.c hexc_table.c hexc_10_32_table.c ltp.c lpc.c lsp.c lsp_tables_nb.c quant_lsp.c nb_celp.c sb_celp.c cb_search.c vbr.c high_lsp_tables.c filters.c vq.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE_ENC=sampleenc.out
EXECUTABLE_DEC=sampledec.out

all: $(SOURCES) $(EXECUTABLE_ENC) $(EXECUTABLE_DEC)

$(EXECUTABLE_ENC): sampleenc.o libspeex.a
	$(CC) sampleenc.o $(LDFLAGS) -o $@

$(EXECUTABLE_DEC): sampledec.o libspeex.a
	$(CC) sampledec.o $(LDFLAGS) -o $@

libspeex.a: $(OBJECTS)
	ar rcs $@ $^

sampleenc.o:
	$(CC) $(CFLAGS) sampleenc.c

sampledec.o:
	$(CC) $(CFLAGS) sampledec.c

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o *.out *.a -rf
