// Originally taken from https://github.com/ntamas/unibinlog

/* The MIT License (MIT)
 *
 * Copyright (c) 2014 Tamas Nepusz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <math.h>
#include <stdio.h>
#include <stdint.h>

int main(void) {
	const double numbers[8] = {
		/* the IEEE-754 representation of the number below consists of the
		 * first eight letters of the uppercase alphabet. Their order will
		 * reveal the endianness we are dealing with. */
		2.39373654120722785592079162598E6, 0, 0, 0,
		/* these are just dummies */
		1234567, 2345678, 3456789, 4567890 };
	double result;
	const double *ptr = numbers, *end = numbers + 8;

	result = 0.0;
	while (ptr < end) {
		result += *ptr;
		ptr++;
	}

	return (result == 12345.0);
}
