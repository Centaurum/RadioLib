CC=gcc -w
CFLAGS=-c -w  -I ./include/ 
LDFLAGS= -lm
SOURCES=testenc.c modes.c speex_callbacks.c bits.c misc.c gain_table.c gain_table_lbr.c exc_10_16_table.c exc_20_32_table.c exc_10_32_table.c exc_5_64_table.c exc_8_128_table.c exc_5_256_table.c hexc_table.c hexc_10_32_table.c quant_lsp.c ltp.c cb_search.c nb_celp.c vbr.c sb_celp.c lsp_tables_nb.c high_lsp_tables.c filters.c vq.c lpc.c lsp.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=a.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o -rf
