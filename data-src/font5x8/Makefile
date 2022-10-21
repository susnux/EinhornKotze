.PHONY: all clean

OUTPUT := ../../data/font5x8.bin
FONT := 5x8.bdf
BDF_CONVERTER = python3 ../../tools/convert_bdf_font.py

all: $(OUTPUT)

clean:
	rm -fv $(OUTPUT)

$(OUTPUT): $(FONT)
	$(BDF_CONVERTER) --font $< --output $@
