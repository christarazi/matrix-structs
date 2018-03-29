FILES=`find include tests -type f -type f \( -iname "*.cpp" -o -iname "*.hpp" \)`

build-debug:
	@cmake -GNinja -DBUILD_TESTS=ON -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	@cp build/compile_commands.json .
	@cmake --build build

build-release-debug:
	@cmake -DBUILD_TESTS=ON -H. -Bbuild -DCMAKE_BUILD_TYPE=RelWithDebInfo
	@cmake --build build

docs:
	@cmake -DBUILD_DOC=ON -H. -Bbuild
	@cmake --build build

build-release:
	@cmake -DBUILD_TESTS=ON -H. -Bbuild -DCMAKE_BUILD_TYPE=Release
	@cmake --build build

test:
	@cd build && GTEST_COLOR=1 ctest --verbose

lint:
	@clang-format -i ${FILES} && git diff --exit-code

docker-test:
	docker build -t matrix-structs-build .
	docker run -v `pwd`:/build -t matrix-structs-build

clean:
	rm -rf build

.PHONY: build
