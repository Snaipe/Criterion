Test C assertions:

  $ asserts.c.bin
  [----] asserts.c:12: Assertion Failed
  [----]   
  [----]   assert is fatal, expect isn't
  [----]   
  [----] asserts.c:13: Assertion Failed
  [----]   
  [----]   This assert runs
  [----]   
  [FAIL] asserts::base
  [----] asserts.c:18: Assertion Failed
  [----]   
  [----]   You can fail an assertion with a message from anywhere
  [----]   
  [----] asserts.c:19: Assertion Failed
  [FAIL] asserts::old_school
  [====] Synthesis: Tested: 7 | Passing: 5 | Failing: 2 | Crashing: 0 

Test C++ assertions:

  $ asserts.cc.bin
  [----] asserts.cc:15: Assertion Failed
  [----]   
  [----]   assert is fatal, expect isn't
  [----]   
  [----] asserts.cc:16: Assertion Failed
  [----]   
  [----]   This assert runs
  [----]   
  [FAIL] asserts::base
  [----] asserts.cc:121: Assertion Failed
  [----]   throw(std::runtime_error, {}): 
  [----]     message: <nothing was thrown>
  [----] asserts.cc:123: Assertion Failed
  [----]   throw(std::bad_alloc, throw std::invalid_argument("some other message")): 
  [----]     message: "some other message"
  [FAIL] asserts::exception
  [----] asserts.cc:21: Assertion Failed
  [----]   
  [----]   You can fail an assertion with a message from anywhere
  [----]   
  [----] asserts.cc:22: Assertion Failed
  [FAIL] asserts::old_school
  [====] Synthesis: Tested: 8 | Passing: 5 | Failing: 3 | Crashing: 0 

Using redirections

  $ redirect.c.bin
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 

  $ redirect.cc.bin
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 

Testing all assert messages

  $ failmessages.c.bin
  [----] failmessages.c:121: Assertion Failed
  [----]   eq(i32, 1, 0): 
  [----]     diff: [-1-]{+0+}
  [----] failmessages.c:122: Assertion Failed
  [----] failmessages.c:123: Assertion Failed
  [----]   eq(i32, 1, 1): 
  [----]     diff: [-1-]{+1+}
  [FAIL] message::compo
  [----] failmessages.c:96: Assertion Failed
  [----]   lt(i8, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(i8, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(i8, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(i8, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.c:97: Assertion Failed
  [----]   lt(i16, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(i16, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(i16, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(i16, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.c:98: Assertion Failed
  [----]   lt(i32, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(i32, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(i32, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(i32, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.c:99: Assertion Failed
  [----]   lt(i64, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(i64, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(i64, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(i64, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.c:100: Assertion Failed
  [----]   lt(u8, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(u8, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(u8, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(u8, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.c:101: Assertion Failed
  [----]   lt(u16, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(u16, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(u16, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(u16, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.c:102: Assertion Failed
  [----]   lt(u32, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(u32, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(u32, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(u32, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.c:103: Assertion Failed
  [----]   lt(u64, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(u64, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(u64, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(u64, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.c:104: Assertion Failed
  [----]   lt(iptr, 1, 0): 
  [----]     actual: 0x1
  [----]     reference: 0x0
  [----]   le(iptr, 1, 0): 
  [----]     actual: 0x1
  [----]     reference: 0x0
  [----]   gt(iptr, 0, 1): 
  [----]     actual: 0x0
  [----]     reference: 0x1
  [----]   ge(iptr, 0, 1): 
  [----]     actual: 0x0
  [----]     reference: 0x1
  [----] failmessages.c:105: Assertion Failed
  [----]   lt(uptr, 1, 0): 
  [----]     actual: 0x1
  [----]     reference: 0x0
  [----]   le(uptr, 1, 0): 
  [----]     actual: 0x1
  [----]     reference: 0x0
  [----]   gt(uptr, 0, 1): 
  [----]     actual: 0x0
  [----]     reference: 0x1
  [----]   ge(uptr, 0, 1): 
  [----]     actual: 0x0
  [----]     reference: 0x1
  [----] failmessages.c:106: Assertion Failed
  [----]   lt(flt, 1 / 3.f, 0): 
  [----]     actual: 0.333333343
  [----]     reference: 0
  [----]   le(flt, 1 / 3.f, 0): 
  [----]     actual: 0.333333343
  [----]     reference: 0
  [----]   gt(flt, 0, 1 / 3.f): 
  [----]     actual: 0
  [----]     reference: 0.333333343
  [----]   ge(flt, 0, 1 / 3.f): 
  [----]     actual: 0
  [----]     reference: 0.333333343
  [----] failmessages.c:107: Assertion Failed
  [----]   lt(dbl, 1 / 3., 0): 
  [----]     actual: 0.33333333333333331
  [----]     reference: 0
  [----]   le(dbl, 1 / 3., 0): 
  [----]     actual: 0.33333333333333331
  [----]     reference: 0
  [----]   gt(dbl, 0, 1 / 3.): 
  [----]     actual: 0
  [----]     reference: 0.33333333333333331
  [----]   ge(dbl, 0, 1 / 3.): 
  [----]     actual: 0
  [----]     reference: 0.33333333333333331
  [----] failmessages.c:108: Assertion Failed
  [----]   lt(ldbl, 1 / 3.l, 0): 
  \[----\]     actual: 0\.3333333333333333(33342|1) (re)
  [----]     reference: 0
  [----]   le(ldbl, 1 / 3.l, 0): 
  \[----\]     actual: 0\.3333333333333333(33342|1) (re)
  [----]     reference: 0
  [----]   gt(ldbl, 0, 1 / 3.l): 
  [----]     actual: 0
  \[----\]     reference: 0\.3333333333333333(33342|1) (re)
  [----]   ge(ldbl, 0, 1 / 3.l): 
  [----]     actual: 0
  \[----\]     reference: 0\.3333333333333333(33342|1) (re)
  [----] failmessages.c:111: Assertion Failed
  [----]   lt(ptr, (void *) 1, (void *) 0): 
  [----]     actual: 0x1
  [----]     reference: 0x0
  [----]   le(ptr, (void *) 1, (void *) 0): 
  [----]     actual: 0x1
  [----]     reference: 0x0
  [----]   gt(ptr, (void *) 0, (void *) 1): 
  [----]     actual: 0x0
  [----]     reference: 0x1
  [----]   ge(ptr, (void *) 0, (void *) 1): 
  [----]     actual: 0x0
  [----]     reference: 0x1
  [----] failmessages.c:113: Assertion Failed
  [----]   lt(str, "cba", "abc"): 
  [----]     actual: "cba"
  [----]     reference: "abc"
  [----]   le(str, "cba", "abc"): 
  [----]     actual: "cba"
  [----]     reference: "abc"
  [----]   gt(str, "abc", "cba"): 
  [----]     actual: "abc"
  [----]     reference: "cba"
  [----]   ge(str, "abc", "cba"): 
  [----]     actual: "abc"
  [----]     reference: "cba"
  [----] failmessages.c:114: Assertion Failed
  [----]   lt(str, "cba\ncba", "abc\nabc"): 
  [----]     actual: "cba\n"
  [----]       "cba"
  [----]     reference: "abc\n"
  [----]       "abc"
  [----]   le(str, "cba\ncba", "abc\nabc"): 
  [----]     actual: "cba\n"
  [----]       "cba"
  [----]     reference: "abc\n"
  [----]       "abc"
  [----]   gt(str, "abc\nabc", "cba\ncba"): 
  [----]     actual: "abc\n"
  [----]       "abc"
  [----]     reference: "cba\n"
  [----]       "cba"
  [----]   ge(str, "abc\nabc", "cba\ncba"): 
  [----]     actual: "abc\n"
  [----]       "abc"
  [----]     reference: "cba\n"
  [----]       "cba"
  [----] failmessages.c:116: Assertion Failed
  [----]   lt(wcs, L"cba", L"abc"): 
  [----]     actual: L"cba"
  [----]     reference: L"abc"
  [----]   le(wcs, L"cba", L"abc"): 
  [----]     actual: L"cba"
  [----]     reference: L"abc"
  [----]   gt(wcs, L"abc", L"cba"): 
  [----]     actual: L"abc"
  [----]     reference: L"cba"
  [----]   ge(wcs, L"abc", L"cba"): 
  [----]     actual: L"abc"
  [----]     reference: L"cba"
  [----] failmessages.c:117: Assertion Failed
  [----]   lt(wcs, L"cba\ncba", L"abc\nabc"): 
  [----]     actual: L"cba\n"
  [----]       L"cba"
  [----]     reference: L"abc\n"
  [----]       L"abc"
  [----]   le(wcs, L"cba\ncba", L"abc\nabc"): 
  [----]     actual: L"cba\n"
  [----]       L"cba"
  [----]     reference: L"abc\n"
  [----]       L"abc"
  [----]   gt(wcs, L"abc\nabc", L"cba\ncba"): 
  [----]     actual: L"abc\n"
  [----]       L"abc"
  [----]     reference: L"cba\n"
  [----]       L"cba"
  [----]   ge(wcs, L"abc\nabc", L"cba\ncba"): 
  [----]     actual: L"abc\n"
  [----]       L"abc"
  [----]     reference: L"cba\n"
  [----]       L"cba"
  [FAIL] messages::cmp
  [----] failmessages.c:127: Assertion Failed
  [----] failmessages.c:128: Assertion Failed
  [----]   
  [----]   foo bar
  [----]   
  [FAIL] messages::default
  [----] failmessages.c:27: Assertion Failed
  [----]   eq(i8, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:28: Assertion Failed
  [----]   eq(i16, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:29: Assertion Failed
  [----]   eq(i32, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:30: Assertion Failed
  [----]   eq(i64, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:31: Assertion Failed
  [----]   eq(u8, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:32: Assertion Failed
  [----]   eq(u16, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:33: Assertion Failed
  [----]   eq(u32, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:34: Assertion Failed
  [----]   eq(u64, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:35: Assertion Failed
  [----]   eq(iptr, 0, 1): 
  [----]     diff: [-0x0-]{+0x1+}
  [----] failmessages.c:36: Assertion Failed
  [----]   eq(uptr, 0, 1): 
  [----]     diff: [-0x0-]{+0x1+}
  [----] failmessages.c:37: Assertion Failed
  [----]   eq(flt, 0, 1 / 3.f): 
  [----]     diff: [-0-]{+0.333333343+}
  [----] failmessages.c:38: Assertion Failed
  [----]   eq(dbl, 0, 1 / 3.): 
  [----]     diff: [-0-]{+0.33333333333333331+}
  [----] failmessages.c:39: Assertion Failed
  [----]   eq(ldbl, 0, 1 / 3.l): 
  \[----\]     diff: \[-0-\]\{\+0\.3333333333333333(1|33342)\+\} (re)
  [----] failmessages.c:42: Assertion Failed
  [----]   eq(ptr, (void *) 1, (void *) 0): 
  [----]     diff: [-0x1-]{+0x0+}
  [----] failmessages.c:44: Assertion Failed
  [----]   eq(str, "", "foo"): 
  [----]     diff: [-""-]{+"foo"+}
  [----] failmessages.c:47: Assertion Failed
  [----]   eq(str, "reallyreallylongstringindeedmygoodsirormadam", "yetanotherreallyreallylongstring"): 
  [----]     @@ -1,1 +1,1 @@
  [----]     -"reallyreallylongstringindeedmygoodsirormadam"
  [----]     +"yetanotherreallyreallylongstring"
  [----] failmessages.c:48: Assertion Failed
  [----]   eq(str, "foo\nbar", "foo\nbaz"): 
  [----]     @@ -1,2 +1,2 @@
  [----]      "foo\n"
  [----]     -"bar"
  [----]     +"baz"
  [----] failmessages.c:50: Assertion Failed
  [----]   eq(wcs, L"", L"foo"): 
  [----]     diff: [-L""-]{+L"foo"+}
  [----] failmessages.c:53: Assertion Failed
  [----]   eq(wcs, L"reallyreallylongstringindeedmygoodsirormadam", L"yetanotherreallyreallylongstring"): 
  [----]     @@ -1,1 +1,1 @@
  [----]     -L"reallyreallylongstringindeedmygoodsirormadam"
  [----]     +L"yetanotherreallyreallylongstring"
  [----] failmessages.c:54: Assertion Failed
  [----]   eq(wcs, L"foo\nbar", L"foo\nbaz"): 
  [----]     @@ -1,2 +1,2 @@
  [----]      L"foo\n"
  [----]     -L"bar"
  [----]     +L"baz"
  [----] failmessages.c:62: Assertion Failed
  [----]   eq(mem, ma, mb): 
  [----]     @@ -1,2 +1,2 @@
  [----]     -00: 00000000                             ....            
  [----]     +00: 01000000                             ....            
  [----]      
  [----] failmessages.c:63: Assertion Failed
  [----]   eq(int[1], &a, &b): 
  [----]     @@ -1,3 +1,3 @@
  [----]      (int[1]) {
  [----]     -\t[0] = 0, (esc)
  [----]     +\t[0] = 1, (esc)
  [----]      }
  [----] failmessages.c:76: Assertion Failed
  [----]   eq(mem, marra, marrb): 
  [----]     @@ -1,3 +1,3 @@
  [----]     -00: 00000000 01000000 02000000 03000000  ................
  [----]     -10: 04000000                             ....            
  [----]     +00: 04000000 03000000 02000000 01000000  ................
  [----]     +10: 00000000                             ....            
  [----]      
  [----] failmessages.c:77: Assertion Failed
  [----]   eq(int[sizeof (arra) / sizeof (int)], arra, arrb): 
  [----]     @@ -1,7 +1,7 @@
  [----]      (int[5]) {
  [----]     -\t[0] = 0, (esc)
  [----]     -\t[1] = 1, (esc)
  [----]     +\t[0] = 4, (esc)
  [----]     +\t[1] = 3, (esc)
  [----]      \t[2] = 2, (esc)
  [----]     -\t[3] = 3, (esc)
  [----]     -\t[4] = 4, (esc)
  [----]     +\t[3] = 1, (esc)
  [----]     +\t[4] = 0, (esc)
  [----]      }
  [----] failmessages.c:82: Assertion Failed
  [----]   eq(type(struct dummy_struct), dummy1, dummy2): 
  [----]     @@ -1,4 +1,4 @@
  [----]      (struct dummy_struct) {
  [----]      \t.a = 42, (esc)
  [----]     -\t.b = 24 (esc)
  [----]     +\t.b = 42 (esc)
  [----]      }
  [----] failmessages.c:83: Assertion Failed
  [----]   eq(type(struct dummy_struct)[1], &dummy1, &dummy2): 
  [----]     @@ -1,6 +1,6 @@
  [----]      (struct dummy_struct[1]) {
  [----]      \t[0] = (struct dummy_struct) { (esc)
  [----]      \t\t.a = 42, (esc)
  [----]     -\t\t.b = 24 (esc)
  [----]     +\t\t.b = 42 (esc)
  [----]      \t}, (esc)
  [----]      }
  [FAIL] messages::eq
  [====] Synthesis: Tested: 4 | Passing: 0 | Failing: 4 | Crashing: 0 

  $ failmessages.cc.bin
  [----] failmessages.cc:118: Assertion Failed
  [----]   eq(i32, 1, 0): 
  [----]     diff: [-1-]{+0+}
  [----] failmessages.cc:119: Assertion Failed
  [----] failmessages.cc:120: Assertion Failed
  [----]   eq(i32, 1, 1): 
  [----]     diff: [-1-]{+1+}
  [FAIL] message::compo
  [----] failmessages.cc:124: Assertion Failed
  [----]   throw(std::bad_alloc, throw std::invalid_argument("exception message")): 
  [----]     message: "exception message"
  [----] failmessages.cc:125: Assertion Failed
  [----] failmessages.cc:126: Assertion Failed
  [----]   nothrow(throw std::invalid_argument("exception message")): 
  [----]     message: "exception message"
  [----] failmessages.cc:127: Assertion Failed
  [FAIL] message::exception
  [----] failmessages.cc:93: Assertion Failed
  [----]   lt(i8, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(i8, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(i8, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(i8, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.cc:94: Assertion Failed
  [----]   lt(i16, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(i16, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(i16, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(i16, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.cc:95: Assertion Failed
  [----]   lt(i32, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(i32, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(i32, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(i32, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.cc:96: Assertion Failed
  [----]   lt(i64, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(i64, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(i64, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(i64, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.cc:97: Assertion Failed
  [----]   lt(u8, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(u8, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(u8, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(u8, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.cc:98: Assertion Failed
  [----]   lt(u16, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(u16, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(u16, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(u16, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.cc:99: Assertion Failed
  [----]   lt(u32, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(u32, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(u32, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(u32, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.cc:100: Assertion Failed
  [----]   lt(u64, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(u64, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(u64, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(u64, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.cc:101: Assertion Failed
  [----]   lt(iptr, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(iptr, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(iptr, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(iptr, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.cc:102: Assertion Failed
  [----]   lt(uptr, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   le(uptr, 1, 0): 
  [----]     actual: 1
  [----]     reference: 0
  [----]   gt(uptr, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----]   ge(uptr, 0, 1): 
  [----]     actual: 0
  [----]     reference: 1
  [----] failmessages.cc:103: Assertion Failed
  [----]   lt(flt, 1 / 3.f, 0): 
  [----]     actual: 0.333333
  [----]     reference: 0
  [----]   le(flt, 1 / 3.f, 0): 
  [----]     actual: 0.333333
  [----]     reference: 0
  [----]   gt(flt, 0, 1 / 3.f): 
  [----]     actual: 0
  [----]     reference: 0.333333
  [----]   ge(flt, 0, 1 / 3.f): 
  [----]     actual: 0
  [----]     reference: 0.333333
  [----] failmessages.cc:104: Assertion Failed
  [----]   lt(dbl, 1 / 3., 0): 
  [----]     actual: 0.333333
  [----]     reference: 0
  [----]   le(dbl, 1 / 3., 0): 
  [----]     actual: 0.333333
  [----]     reference: 0
  [----]   gt(dbl, 0, 1 / 3.): 
  [----]     actual: 0
  [----]     reference: 0.333333
  [----]   ge(dbl, 0, 1 / 3.): 
  [----]     actual: 0
  [----]     reference: 0.333333
  [----] failmessages.cc:105: Assertion Failed
  [----]   lt(ldbl, 1 / 3.l, 0): 
  [----]     actual: 0.333333
  [----]     reference: 0
  [----]   le(ldbl, 1 / 3.l, 0): 
  [----]     actual: 0.333333
  [----]     reference: 0
  [----]   gt(ldbl, 0, 1 / 3.l): 
  [----]     actual: 0
  [----]     reference: 0.333333
  [----]   ge(ldbl, 0, 1 / 3.l): 
  [----]     actual: 0
  [----]     reference: 0.333333
  [----] failmessages.cc:108: Assertion Failed
  [----]   lt(ptr, (void *) 1, (void *) 0): 
  [----]     actual: @1
  [----]     reference: nullptr
  [----]   le(ptr, (void *) 1, (void *) 0): 
  [----]     actual: @1
  [----]     reference: nullptr
  [----]   gt(ptr, (void *) 0, (void *) 1): 
  [----]     actual: nullptr
  [----]     reference: @1
  [----]   ge(ptr, (void *) 0, (void *) 1): 
  [----]     actual: nullptr
  [----]     reference: @1
  [----] failmessages.cc:110: Assertion Failed
  [----]   lt(str, "cba", "abc"): 
  [----]     actual: "cba"
  [----]     reference: "abc"
  [----]   le(str, "cba", "abc"): 
  [----]     actual: "cba"
  [----]     reference: "abc"
  [----]   gt(str, "abc", "cba"): 
  [----]     actual: "abc"
  [----]     reference: "cba"
  [----]   ge(str, "abc", "cba"): 
  [----]     actual: "abc"
  [----]     reference: "cba"
  [----] failmessages.cc:111: Assertion Failed
  [----]   lt(str, "cba\ncba", "abc\nabc"): 
  [----]     actual: "cba\n"
  [----]       "cba"
  [----]     reference: "abc\n"
  [----]       "abc"
  [----]   le(str, "cba\ncba", "abc\nabc"): 
  [----]     actual: "cba\n"
  [----]       "cba"
  [----]     reference: "abc\n"
  [----]       "abc"
  [----]   gt(str, "abc\nabc", "cba\ncba"): 
  [----]     actual: "abc\n"
  [----]       "abc"
  [----]     reference: "cba\n"
  [----]       "cba"
  [----]   ge(str, "abc\nabc", "cba\ncba"): 
  [----]     actual: "abc\n"
  [----]       "abc"
  [----]     reference: "cba\n"
  [----]       "cba"
  [----] failmessages.cc:113: Assertion Failed
  [----]   lt(wcs, L"cba", L"abc"): 
  [----]     actual: L"cba"
  [----]     reference: L"abc"
  [----]   le(wcs, L"cba", L"abc"): 
  [----]     actual: L"cba"
  [----]     reference: L"abc"
  [----]   gt(wcs, L"abc", L"cba"): 
  [----]     actual: L"abc"
  [----]     reference: L"cba"
  [----]   ge(wcs, L"abc", L"cba"): 
  [----]     actual: L"abc"
  [----]     reference: L"cba"
  [----] failmessages.cc:114: Assertion Failed
  [----]   lt(wcs, L"cba\ncba", L"abc\nabc"): 
  [----]     actual: L"cba\n"
  [----]       L"cba"
  [----]     reference: L"abc\n"
  [----]       L"abc"
  [----]   le(wcs, L"cba\ncba", L"abc\nabc"): 
  [----]     actual: L"cba\n"
  [----]       L"cba"
  [----]     reference: L"abc\n"
  [----]       L"abc"
  [----]   gt(wcs, L"abc\nabc", L"cba\ncba"): 
  [----]     actual: L"abc\n"
  [----]       L"abc"
  [----]     reference: L"cba\n"
  [----]       L"cba"
  [----]   ge(wcs, L"abc\nabc", L"cba\ncba"): 
  [----]     actual: L"abc\n"
  [----]       L"abc"
  [----]     reference: L"cba\n"
  [----]       L"cba"
  [FAIL] messages::cmp
  [----] failmessages.cc:131: Assertion Failed
  [----] failmessages.cc:132: Assertion Failed
  [----]   
  [----]   foo bar
  [----]   
  [FAIL] messages::default
  [----] failmessages.cc:24: Assertion Failed
  [----]   eq(i8, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:25: Assertion Failed
  [----]   eq(i16, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:26: Assertion Failed
  [----]   eq(i32, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:27: Assertion Failed
  [----]   eq(i64, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:28: Assertion Failed
  [----]   eq(u8, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:29: Assertion Failed
  [----]   eq(u16, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:30: Assertion Failed
  [----]   eq(u32, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:31: Assertion Failed
  [----]   eq(u64, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:32: Assertion Failed
  [----]   eq(iptr, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:33: Assertion Failed
  [----]   eq(uptr, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:34: Assertion Failed
  [----]   eq(flt, 0, 1 / 3.f): 
  [----]     diff: [-0-]{+0.333333+}
  [----] failmessages.cc:35: Assertion Failed
  [----]   eq(dbl, 0, 1 / 3.): 
  [----]     diff: [-0-]{+0.333333+}
  [----] failmessages.cc:36: Assertion Failed
  [----]   eq(ldbl, 0, 1 / 3.l): 
  [----]     diff: [-0-]{+0.333333+}
  [----] failmessages.cc:39: Assertion Failed
  [----]   eq(ptr, (void *) 1, (void *) 0): 
  [----]     diff: [-@1-]{+nullptr+}
  [----] failmessages.cc:41: Assertion Failed
  [----]   eq(str, "", "foo"): 
  [----]     diff: [-""-]{+"foo"+}
  [----] failmessages.cc:44: Assertion Failed
  [----]   eq(str, "reallyreallylongstringindeedmygoodsirormadam", "yetanotherreallyreallylongstring"): 
  [----]     @@ -1,1 +1,1 @@
  [----]     -"reallyreallylongstringindeedmygoodsirormadam"
  [----]     +"yetanotherreallyreallylongstring"
  [----] failmessages.cc:45: Assertion Failed
  [----]   eq(str, "foo\nbar", "foo\nbaz"): 
  [----]     @@ -1,2 +1,2 @@
  [----]      "foo\n"
  [----]     -"bar"
  [----]     +"baz"
  [----] failmessages.cc:47: Assertion Failed
  [----]   eq(wcs, L"", L"foo"): 
  [----]     diff: [-L""-]{+L"foo"+}
  [----] failmessages.cc:50: Assertion Failed
  [----]   eq(wcs, L"reallyreallylongstringindeedmygoodsirormadam", L"yetanotherreallyreallylongstring"): 
  [----]     @@ -1,1 +1,1 @@
  [----]     -L"reallyreallylongstringindeedmygoodsirormadam"
  [----]     +L"yetanotherreallyreallylongstring"
  [----] failmessages.cc:51: Assertion Failed
  [----]   eq(wcs, L"foo\nbar", L"foo\nbaz"): 
  [----]     @@ -1,2 +1,2 @@
  [----]      L"foo\n"
  [----]     -L"bar"
  [----]     +L"baz"
  [----] failmessages.cc:59: Assertion Failed
  [----]   eq(mem, ma, mb): 
  [----]     @@ -1,2 +1,2 @@
  [----]     -00: 00                                   .               
  [----]     +00: 01                                   .               
  [----]      
  [----] failmessages.cc:60: Assertion Failed
  [----]   eq(int[1], &a, &b): 
  [----]     @@ -1,3 +1,3 @@
  [----]      (int[1]) {
  [----]     -\t[0] = 0, (esc)
  [----]     +\t[0] = 1, (esc)
  [----]      }
  [----] failmessages.cc:73: Assertion Failed
  [----]   eq(mem, marra, marrb): 
  [----]     @@ -1,2 +1,2 @@
  [----]     -00: 00000000 01                          .....           
  [----]     +00: 04000000 03                          .....           
  [----]      
  [----] failmessages.cc:74: Assertion Failed
  [----]   eq(int[sizeof (arra) / sizeof (int)], arra, arrb): 
  [----]     @@ -1,7 +1,7 @@
  [----]      (int[5]) {
  [----]     -\t[0] = 0, (esc)
  [----]     -\t[1] = 1, (esc)
  [----]     +\t[0] = 4, (esc)
  [----]     +\t[1] = 3, (esc)
  [----]      \t[2] = 2, (esc)
  [----]     -\t[3] = 3, (esc)
  [----]     -\t[4] = 4, (esc)
  [----]     +\t[3] = 1, (esc)
  [----]     +\t[4] = 0, (esc)
  [----]      }
  [----] failmessages.cc:79: Assertion Failed
  [----]   eq(type(struct dummy_struct), dummy1, dummy2): 
  [----]     @@ -1,4 +1,4 @@
  [----]      (struct dummy_struct) {
  [----]      \t.a = *, (esc)
  [----]     -\t.b = 24 (esc)
  [----]     +\t.b = 42 (esc)
  [----]      }
  [----] failmessages.cc:80: Assertion Failed
  [----]   eq(type(struct dummy_struct)[1], &dummy1, &dummy2): 
  [----]     @@ -1,6 +1,6 @@
  [----]      (struct dummy_struct[1]) {
  [----]      \t[0] = (struct dummy_struct) { (esc)
  [----]      \t\t.a = *, (esc)
  [----]     -\t\t.b = 24 (esc)
  [----]     +\t\t.b = 42 (esc)
  [----]      \t}, (esc)
  [----]      }
  [FAIL] messages::eq
  [====] Synthesis: Tested: 5 | Passing: 0 | Failing: 5 | Crashing: 0 

Long assert messages should wrap

  $ long-messages.c.bin
  [----] long-messages.c:4: Assertion Failed
  [----]   
  [----]   This is
  [----]   A long message
  [----]   Spawning multiple lines.
  [----]   Formatting is respected.
  [----]   
  [FAIL] sample::long_msg
  [====] Synthesis: Tested: 1 | Passing: 0 | Failing: 1 | Crashing: 0 

  $ long-messages.cc.bin
  [----] long-messages.cc:4: Assertion Failed
  [----]   
  [----]   This is
  [----]   A long message
  [----]   Spawning multiple lines.
  [----]   Formatting is respected.
  [----]   
  [FAIL] sample::long_msg
  [====] Synthesis: Tested: 1 | Passing: 0 | Failing: 1 | Crashing: 0 

Using redirections

  $ redirect.c.bin
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 

  $ redirect.cc.bin
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 
