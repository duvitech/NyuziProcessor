#!/usr/bin/env python3
#
# Copyright 2016 Jeff Bush
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

import sys

sys.path.insert(0, '../..')
import test_harness


@test_harness.test
def kernel_hello(_, target):
    test_harness.build_program(['hello.c'], image_type='user')
    result = test_harness.run_kernel(target=target, timeout=240)
    test_harness.check_result('hello.c', result)

test_harness.execute_tests()
