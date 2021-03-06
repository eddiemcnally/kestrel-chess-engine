cmake_minimum_required(VERSION 3.0)

project(kestrel)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_COMPILER "/usr/bin/clang")

include_directories("src" "src/core" "src/fen" "src/utils" "src/board")


# debug options
set(debug ON)

if(debug)
	#
	# debug compile options
	#
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
		-g \
		-std=c11 \
		-Wpedantic \
		-Wall \
		-Wextra \
		-fsanitize=undefined \
		-fno-omit-frame-pointer \
		-Wstrict-prototypes \
		-Wmissing-prototypes \
		-Wswitch-default \
		-Wunused-macros \
		-Wextra \
		-Wconversion \
		-Wsign-conversion \
		-Wshadow \
		-Wcast-qual \
		-Wcast-align \
		-Wmissing-include-dirs \
		-Wpointer-arith")

else()

		#
		# optimised compile
		#
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
		-g \
		-std=c11 \
		-O2 \
		-flto \
		-march=native \
		-mtune=native \
		-Wpedantic \
		-Wall \
		-Wextra \
		-fsanitize=undefined \
		-Wstrict-prototypes \
		-Wmissing-prototypes \
		-Wswitch-default \
		-Wunused-macros \
		-Wextra \
		-Wconversion \
		-Wsign-conversion \
		-Wshadow \
		-Wcast-qual \
		-Wcast-align \
		-Wmissing-include-dirs \
		-Wpointer-arith" )
endif()


#
# sources and headers shared between the main binary (kestrel)
# and the test runner binary (test_kestrel)
#
set(COMMON_SOURCES
			src/core/kestrel.h
			src/fen/fen.h
			src/fen/fen.c
			src/utils/utils.h
			src/utils/utils.c
			src/board/occupancy_mask_gen.c
            src/board/occupancy_mask_gen.h
            src/board/occupancy_mask.c
            src/board/occupancy_mask.h
            src/board/bitboard.c
            src/board/bitboard.h
            src/board/board.c
            src/board/board.h
            src/board/board_utils.c
            src/board/board_utils.h
            src/pieces.c
            src/pieces.h
            src/attack.c
            src/attack.h
            src/move_gen.c
            src/move_gen.h
            src/move_gen_utils.c
            src/move_gen_utils.h
            src/evaluate.c
            src/evaluate.h
            src/hashkeys.c
            src/hashkeys.h
            src/search.c
            src/search.h
            src/tt.c
            src/tt.h
            src/uci_protocol.c
            src/uci_protocol.h)


#
# main entry point for kestrel engine
#
set(TARGET_SOURCES src/kestrel.c)


#
# source and header files for the test runner
#
set(TEST_SOURCES
        test/all_tests.c
        test/attack_test_fixture.c
        test/board_test_fixture.c
        test/move_test_fixture.c
        test/occupancy_mask_test_fixture.c
        test/performance_tests.c
        test/piece_test_fixture.c
        test/search_tests.c
        test/seatest.c
        test/utils_test_feature.c
        test/all_tests.h
        test/attack_test_fixture.h
        test/board_test_fixture.h
        test/move_test_fixture.h
        test/occupancy_mask_test_fixture.h
        test/performance_tests.h
        test/piece_test_fixture.h
        test/search_tests.h
        test/seatest.h
        test/utils_test_feature.h
)

add_executable(kestrel ${COMMON_SOURCES} ${TARGET_SOURCES})
add_executable(test_kestrel ${COMMON_SOURCES} ${TEST_SOURCES})

# enable runtime asserts
set_target_properties(test_kestrel PROPERTIES COMPILE_DEFINITIONS "ENABLE_ASSERTS=1")


#
# set up so the test runner app sees the header files in
# the main src directory
#
target_include_directories(test_kestrel BEFORE PUBLIC src)
