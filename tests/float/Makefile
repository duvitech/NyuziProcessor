#
# Copyright 2011-2015 Jeff Bush
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

TOPDIR=../../

include $(TOPDIR)/build/target.mk


LIBS=-lc -los-bare

SRCS=run_tests.c test_cases.s

OBJS := $(CRT0_BARE) $(SRCS_TO_OBJS)
DEPS := $(SRCS_TO_DEPS)

$(OBJ_DIR)/run_tests.hex: $(OBJS) $(DEPS)
	$(LD) $(OBJS) -o $(OBJ_DIR)/run_tests.elf $(LIBS) $(LDFLAGS)
	$(ELF2HEX) -o $@ $(OBJ_DIR)/run_tests.elf

test: $(OBJ_DIR)/run_tests.hex
	$(EMULATOR) $(OBJ_DIR)/run_tests.hex

vtest: $(OBJ_DIR)/run_tests.hex
	$(VERILATOR) +bin=$(OBJ_DIR)/run_tests.hex

test_cases.s: obj/gen_cases
	./obj/gen_cases > test_cases.s

obj/gen_cases:
	make -f gen_cases.mk

clean: FORCE
	rm -f $(OBJ_DIR)/*

FORCE:

-include $(DEPS)
