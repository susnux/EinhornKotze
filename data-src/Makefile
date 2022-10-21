.PHONY: all clean

FONTS := font8x8 font5x8

all:
	for d in $(FONTS); do \
		pushd "$$d"; \
		$(MAKE) all; \
		popd; \
	done

clean:
	for d in $(FONTS); do \
		pushd "$$d"; \
		$(MAKE) clean; \
		popd; \
	done

