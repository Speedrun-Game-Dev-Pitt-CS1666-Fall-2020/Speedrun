BUILD_DIR ?= ./build

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)
