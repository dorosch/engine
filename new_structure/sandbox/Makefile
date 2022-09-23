TARGET = build/app


.PHONY: $(TARGET)

$(TARGET):
	cmake -B build .
	make -C build
	

run: $(TARGET)
	./$(TARGET)

clean:
	@rm -rf ./build
	@rm -f$(shell find ./src/ -name *.o)
