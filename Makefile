BUILD_DIR = build

.PHONY: build test clean

.ONESHELL:
build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR)
	# TODO extract options correctly
	cmake -DCMAKE_BUILD_TYPE=Debug ..
	cmake --build . -- -j 4

test: build
	cd $(BUILD_DIR)
	cmake --build . --target tests -- -j 4
	./test/tests

# TODO add target to pull lib up

clean:
	$(RM) -rf $(BUILD_DIR)
