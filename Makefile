BUILD_DIR ?= ./build

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)
	find . -type f -name '*.d' -exec rm {} 
	find . -type f -name '*.o' -exec rm {} ++
