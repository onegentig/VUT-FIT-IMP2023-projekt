###############################################################################
#
# IXX-Projekt Makefile
# (edit this file to fit the project)
#
# Usage:
#   - `make` or `make all` or `make release` to build the project
#   - `make debug` to build the project with debug flags
#   - `make clean` to remove built binaries
#   - `make format` to format the source code
#   - `make lint` to lint the source code
#   - `make zip` to create a zip archive of the project
#   - `make help` to show Makefile usage
#
###############################################################################

TARGET                 = ixx-projekt
ZIPNAME                = xlogin00.zip

##### Example: C #####

CC                     = gcc
CFLAGS                 = -std=c99
EXTRA_CFLAGS           = -Wall -Wextra -Werror -pedantic \
                    -fdata-sections -ffunction-sections
RELEASE_CFLAGS         = -DNDEBUG -O2 -march=native
DEBUG_CFLAGS           = -g -Og -fsanitize=undefined
LINT_FLAGS             = --format-style=file --fix \
   -checks="bugprone-*,google-*,performance-*,readability-*"
RM                     = rm -f

SRCS                   = $(wildcard *.c)

##### Example: C++ #####

CPP                    = g++
CPPFLAGS               = -std=c++20
EXTRA_CPPFLAGS         = -Wall -Wextra -Werror -pedantic \
                    -fdata-sections -ffunction-sections
RELEASE_CPPFLAGS       = -DNDEBUG -O2 -march=native
DEBUG_CPPFLAGS         = -g -Og -fsanitize=undefined
LINT_FLAGS             = --format-style=file --fix \
    -checks="bugprone-*,google-*,performance-*,readability-*"
RM                     = rm -f

SRCS                   = $(wildcard *.cpp)

##### Example: LaTeX #####

LATEX                  = pdftex
DVIPS                  = dvips -t a4
PS2PDF                 = ps2pdf

SRCS                   = $(wildcard *.tex)

###############################################################################

.PHONY: all release debug help clean zip lint format

all: release

release: EXTRA_CXFLAGS += ${RELEASE_CXFLAGS}
release: ${TARGET}

debug: EXTRA_CXFLAGS += ${DEBUG_CXFLAGS}
debug: ${TARGET}

${TARGET}: ${SRCS}
	@##### C #####
	${CC} ${CFLAGS} ${EXTRA_CFLAGS} ${SRCS} -o ${TARGET}
	@#### C++ ####
	${CPP} ${CPPFLAGS} ${EXTRA_CXFLAGS} ${SRCS} -o ${TARGET}
	@### LaTeX ###
	${LATEX} ${SRCS}
	${LATEX} ${SRCS}
	${DVIPS} ${SRCS}
	${PS2PDF} ${SRCS}
	@#############
	@echo "projcpp compiled!"
	@echo "Run with: ./projcpp -s omething"

help:
	@echo "IXX-Project (Template Makefile)"
	@echo "@author Someone Amazing <xlogin00@vutbr.cz>"
	@echo ""
	@echo "Usage: make [TARGET]"
	@echo "TARGETs:"
	@echo "  all     compile and link the project (default)"
	@echo "  debug   compile and link the project with debug flags"
	@echo "  clean   clean objects and executables"
	@echo "  format  run formatter"
	@echo "  lint    run linter"
	@echo "  zip     create a .zip archive with the source files"
	@echo "  help    print this message"

clean:
	${RM} ${TARGET}

zip:
	zip -q -r ${ZIPNAME}.zip ${SRCS} Makefile

format:
	clang-format -i *.cpp *.hpp

lint:
	clang-tidy ${SRCS} ${LINT_FLAGS} -- ${CFLAGS}
