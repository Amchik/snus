# makefile
#
# Usage:
#   $ make
#     Build bin/snus with target RELEASE
#   $ TARGET=DEBUG make
#     Build bin/snus with target DEBUG
#   $ make info
#     Prints info about current TARGET
#   $ make clean
#   	Cleans bin/ and obj/ dirs
#
#   Creating own target:
#   $ export _mytarget_CFLAGS="-O2"
#   $ export _mytarget_CFLAGS_DEFINES="-D INT_CALCULATE_SPACES"
#   $ export _mytarget_LDFLAGS="-v"
#   $ export _mytarget_CC="clang"
#   $ export _mytarget_STD="gnu99"
#   $ export TARGET=mytarget

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))

ifdef TARGET
	TARGET := $(TARGET)
else
	TARGET := RELEASE
endif

_STD=c89

_CFLAGS_LOG = -Wall -Wextra -pedantic -Wpedantic -Werror
_CFLAGS_DEFINES = -D _DEFAULT_SOURCE

_RELEASE_CFLAGS = -O3 -march=native -pipe
_RELEASE_CFLAGS_DEFINES = -D INT_FAIL_ZEROID -D INT_CALCULATE_SPACES -D INT_CALCULATE_SPACES_THREE
_RELEASE_LDFLAGS = 
_RELEASE_CC = $(CC)
_RELEASE_STD = $(_STD)

_DEBUG_CFLAGS = -pipe -O0 -g
_DEBUG_CFLAGS_DEFINES = -D INT_FAIL_ZEROID -D INT_REPORT_RUNTIME -D INT_CALCULATE_SPACES -D INT_CALCULATE_SPACES_THREE
_DEBUG_LDFLAGS = 
_DEBUG_CC = $(CC)
_DEBUG_STD = $(_STD)

CFLAGS := -std=$(_$(TARGET)_STD) $(_CFLAGS_LOG) $(_CFLAGS_DEFINES) $(_$(TARGET)_CFLAGS) $(_$(TARGET)_CFLAGS_DEFINES) $(CFLAGS)
LDFLAGS := $(_$(TARGET)_LDFLAGS) $(LDFLAGS)

__DONE := \e[0;32m[DONE]\e[0m
__UNDONE := ...

obj/%.o: src/%.c
	@mkdir -p $(@D)
	@printf "\e[0;32mCompiling\e[0m \e[0;40m$@\e[0m$(__UNDONE)         \r"
	@$(_$(TARGET)_CC) $(CFLAGS) -c -o $@ $<
	@printf "\e[0;32mCompiling\e[0m \e[0;40m$@\e[0m $(__DONE)          \n"

bin/snus: $(OBJ)
	@mkdir -p $(@D)
	@printf "\e[0;35mLinking\e[0m   \e[0;40m$@\e[0m$(__UNDONE)         \r"
	@$(_$(TARGET)_CC) -o $@ $^ $(LDFLAGS)
	@printf "\e[0;35mLinking\e[0m   \e[0;40m$@\e[0m $(__DONE)          \n"

clean:
	@printf "\e[0;34mRemoving obj files\e[0m$(__UNDONE)                \r"
	@rm -f $(OBJ)
	@printf "\e[0;34mRemoving obj files\e[0m $(__DONE)                 \n"
	@printf "\e[0;34mRemoving binary files\e[0m$(__UNDONE)             \r"
	@rm -f bin/snus
	@printf "\e[0;34mRemoving binary files\e[0m $(__DONE)              \n"

info:
	@printf "\e[0;37m# Compiling for target \e[0;32m$(TARGET)\e[0m\n"
	@printf "\e[0;37m#  Target special CFLAGS:  \e[0;40m$(_$(TARGET)_CFLAGS)\e[0m\n"
	@printf "\e[0;37m#  Target special LDFLAGS: \e[0;40m$(_$(TARGET)_LDFLAGS)\e[0m\n"
	@printf "\e[0;37m#  Target special defines: \e[0;40m$(_$(TARGET)_CFLAGS_DEFINES)\e[0m\n"
	@printf "\e[0;37m# Using C standart \e[0;32m$(_$(TARGET)_STD)\e[0;37m via \e[0;32m$(_$(TARGET)_CC)\e[0m\n"
	@printf "\e[0;37m# Default log level: \e[0;40m$(_CFLAGS_LOG)\e[0m\n"
	@printf "\e[0;37m# Default defines:   \e[0;40m$(_CFLAGS_DEFINES)\e[0m\n"
	@printf "\e[0;37m# Full CFLAGS:  \e[0;40m$(CFLAGS)\e[0m\n"
	@printf "\e[0;37m# Full LDFLAGS: \e[0;40m$(LDFLAGS)\e[0m\n"

