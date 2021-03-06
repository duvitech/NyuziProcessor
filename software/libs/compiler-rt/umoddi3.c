//
// Copyright 2011-2015 Jeff Bush
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

extern unsigned long long int __udivdi3(unsigned long long int dividend, unsigned long long int divisor);

// Unsigned 64-bit integer modulus
unsigned long long int __umoddi3(unsigned long long int value1, unsigned long long int value2)
{
    return value1 - __udivdi3(value1, value2) * value2;
}
