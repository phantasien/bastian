CURDIR := $(shell pwd)

lowercase = $(shell echo $1 | tr A-Z a-z)

GTEST_LIBS_PATH := deps/gtest/cbuild
SYS_NAME := $(shell uname -s)
SYS_NAME_LOWER := $(call lowercase,${SYS_NAME})


ifeq (${SYS_NAME_LOWER},linux)
	SYS_CMAKE_FLAGS :=
else
	SYS_CMAKE_FLAGS := -DCMAKE_CXX_FLAGS="-std=c++11 -stdlib=libstdc++ -U__STRICT_ANSI__"
endif


clean:
	@rm -rf out/Debug/obj.target/bastian
	@rm -rf build
	@rm -rf out/jsc-x64
	@rm -rf ${RUN_TEST}

./out/v8-x64/Debug/test-bastian: ${GTEST_LIBS_PATH}
	@./tools/gyp_bastian test/test.gyp -Dbastian_project=${CURDIR} -Dbastian_engine=v8 -Dtarget_arch=x64

test-v8: ./out/v8-x64/Debug/test-bastian
	@make -C out/v8-x64
	@./out/v8-x64/Debug/test-bastian

./out/jsc-x64/Debug/test-bastian: ${GTEST_LIBS_PATH}
	@./tools/gyp_bastian test/test.gyp -Dbastian_project=${CURDIR} -Dbastian_engine=jsc -Dtarget_arch=x64
	@make -C out/jsc-x64

test-jsc: ./out/jsc-x64/Debug/test-bastian
	@./out/jsc-x64/Debug/test-bastian

test: test-v8 test-jsc

${GTEST_LIBS_PATH}:
	@mkdir ${GTEST_LIBS_PATH}
	@cd ${GTEST_LIBS_PATH} && cmake -G"Unix Makefiles" ${SYS_CMAKE_FLAGS} .. && make


.PHONY: test
