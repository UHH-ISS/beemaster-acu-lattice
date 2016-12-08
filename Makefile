BUILD_DIR = build

.PHONY: build test clean

.ONESHELL:
build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR)
	# TODO extract options correctly
	cmake -DCMAKE_BUILD_TYPE=Debug ..
	cmake --build .

test: build
	# TODO to be tested!
	cd $(BUILD_DIR)
	cmake --target test .

# TODO add target to pull lib up

clean:
	$(RM) -rf $(BUILD_DIR)
