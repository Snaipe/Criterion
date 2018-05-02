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
  [====] Synthesis: Tested: 8 | Passing: 6 | Failing: 2 | Crashing: 0 

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
  [----] asserts.cc:167: Assertion Failed
  [----]   throw(std::runtime_error, {}): 
  [----]     message: <nothing was thrown>
  [----] asserts.cc:169: Assertion Failed
  [----]   throw(std::bad_alloc, throw std::invalid_argument("some other message")): 
  [----]     message: "some other message"
  [FAIL] asserts::exception
  [----] asserts.cc:21: Assertion Failed
  [----]   
  [----]   You can fail an assertion with a message from anywhere
  [----]   
  [----] asserts.cc:22: Assertion Failed
  [FAIL] asserts::old_school
  [====] Synthesis: Tested: 9 | Passing: 6 | Failing: 3 | Crashing: 0 

Using redirections

  $ redirect.c.bin
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 

  $ redirect.cc.bin
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 

Testing all assert messages

  $ failmessages.c.bin
  [----] failmessages.c:185: Assertion Failed
  [----]   eq(i32, 1, 0): 
  [----]     diff: [-1-]{+0+}
  [----] failmessages.c:186: Assertion Failed
  [----] failmessages.c:187: Assertion Failed
  [----]   eq(i32, 1, 1): 
  [----]     diff: [-1-]{+1+}
  [FAIL] message::compo
  [----] failmessages.c:137: Assertion Failed
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
  [----] failmessages.c:138: Assertion Failed
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
  [----] failmessages.c:139: Assertion Failed
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
  [----] failmessages.c:140: Assertion Failed
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
  [----] failmessages.c:141: Assertion Failed
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
  [----] failmessages.c:142: Assertion Failed
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
  [----] failmessages.c:143: Assertion Failed
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
  [----] failmessages.c:144: Assertion Failed
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
  [----] failmessages.c:145: Assertion Failed
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
  [----] failmessages.c:146: Assertion Failed
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
  [----] failmessages.c:147: Assertion Failed
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
  [----] failmessages.c:148: Assertion Failed
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
  [----] failmessages.c:149: Assertion Failed
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
  [----] failmessages.c:152: Assertion Failed
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
  [----] failmessages.c:154: Assertion Failed
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
  [----] failmessages.c:155: Assertion Failed
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
  [----] failmessages.c:157: Assertion Failed
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
  [----] failmessages.c:158: Assertion Failed
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
  [----] failmessages.c:178: Assertion Failed
  [----]   lt(stream, shi, slo): 
  [----]     actual: 00: 68656c6c 6f20776f 726c6400           hello world.    
  [----]       
  [----]     reference: 00: 646c726f 77206f6c 6c656800           dlrow olleh.    
  [----]       
  [----]   le(stream, shi, slo): 
  [----]     actual: 00: 68656c6c 6f20776f 726c6400           hello world.    
  [----]       
  [----]     reference: 00: 646c726f 77206f6c 6c656800           dlrow olleh.    
  [----]       
  [----]   gt(stream, slo, shi): 
  [----]     actual: 00: 646c726f 77206f6c 6c656800           dlrow olleh.    
  [----]       
  [----]     reference: 00: 68656c6c 6f20776f 726c6400           hello world.    
  [----]       
  [----]   ge(stream, slo, shi): 
  [----]     actual: 00: 646c726f 77206f6c 6c656800           dlrow olleh.    
  [----]       
  [----]     reference: 00: 68656c6c 6f20776f 726c6400           hello world.    
  [----]       
  [FAIL] messages::cmp
  [----] failmessages.c:191: Assertion Failed
  [----] failmessages.c:192: Assertion Failed
  [----]   
  [----]   foo bar
  [----]   
  [FAIL] messages::default
  [----] failmessages.c:48: Assertion Failed
  [----]   eq(i8, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:49: Assertion Failed
  [----]   eq(i16, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:50: Assertion Failed
  [----]   eq(i32, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:51: Assertion Failed
  [----]   eq(i64, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:52: Assertion Failed
  [----]   eq(u8, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:53: Assertion Failed
  [----]   eq(u16, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:54: Assertion Failed
  [----]   eq(u32, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:55: Assertion Failed
  [----]   eq(u64, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:56: Assertion Failed
  [----]   eq(iptr, 0, 1): 
  [----]     diff: [-0x0-]{+0x1+}
  [----] failmessages.c:57: Assertion Failed
  [----]   eq(uptr, 0, 1): 
  [----]     diff: [-0x0-]{+0x1+}
  [----] failmessages.c:58: Assertion Failed
  [----]   eq(flt, 0, 1 / 3.f): 
  [----]     diff: [-0-]{+0.333333343+}
  [----] failmessages.c:59: Assertion Failed
  [----]   eq(dbl, 0, 1 / 3.): 
  [----]     diff: [-0-]{+0.33333333333333331+}
  [----] failmessages.c:60: Assertion Failed
  [----]   eq(ldbl, 0, 1 / 3.l): 
  \[----\]     diff: \[-0-\]\{\+0\.3333333333333333(1|33342)\+\} (re)
  [----] failmessages.c:63: Assertion Failed
  [----]   eq(ptr, (void *) 1, (void *) 0): 
  [----]     diff: [-0x1-]{+0x0+}
  [----] failmessages.c:65: Assertion Failed
  [----]   eq(str, "", "foo"): 
  [----]     diff: [-""-]{+"foo"+}
  [----] failmessages.c:68: Assertion Failed
  [----]   eq(str, "reallyreallylongstringindeedmygoodsirormadam", "yetanotherreallyreallylongstring"): 
  [----]     @@ -1,1 +1,1 @@
  [----]     -"reallyreallylongstringindeedmygoodsirormadam"
  [----]     +"yetanotherreallyreallylongstring"
  [----] failmessages.c:69: Assertion Failed
  [----]   eq(str, "foo\nbar", "foo\nbaz"): 
  [----]     @@ -1,2 +1,2 @@
  [----]      "foo\n"
  [----]     -"bar"
  [----]     +"baz"
  [----] failmessages.c:71: Assertion Failed
  [----]   eq(wcs, L"", L"foo"): 
  [----]     diff: [-L""-]{+L"foo"+}
  [----] failmessages.c:74: Assertion Failed
  [----]   eq(wcs, L"reallyreallylongstringindeedmygoodsirormadam", L"yetanotherreallyreallylongstring"): 
  [----]     @@ -1,1 +1,1 @@
  [----]     -L"reallyreallylongstringindeedmygoodsirormadam"
  [----]     +L"yetanotherreallyreallylongstring"
  [----] failmessages.c:75: Assertion Failed
  [----]   eq(wcs, L"foo\nbar", L"foo\nbaz"): 
  [----]     @@ -1,2 +1,2 @@
  [----]      L"foo\n"
  [----]     -L"bar"
  [----]     +L"baz"
  [----] failmessages.c:83: Assertion Failed
  [----]   eq(mem, ma, mb): 
  [----]     @@ -1,2 +1,2 @@
  [----]     -00: 00000000                             ....            
  [----]     +00: 01000000                             ....            
  [----]      
  [----] failmessages.c:84: Assertion Failed
  [----]   eq(int[1], &a, &b): 
  [----]     @@ -1,3 +1,3 @@
  [----]      (int[1]) {
  [----]     -\t[0] = 0, (esc)
  [----]     +\t[0] = 1, (esc)
  [----]      }
  [----] failmessages.c:97: Assertion Failed
  [----]   eq(mem, marra, marrb): 
  [----]     @@ -1,3 +1,3 @@
  [----]     -00: 00000000 01000000 02000000 03000000  ................
  [----]     -10: 04000000                             ....            
  [----]     +00: 04000000 03000000 02000000 01000000  ................
  [----]     +10: 00000000                             ....            
  [----]      
  [----] failmessages.c:98: Assertion Failed
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
  [----] failmessages.c:103: Assertion Failed
  [----]   eq(type(struct dummy_struct), dummy1, dummy2): 
  [----]     @@ -1,4 +1,4 @@
  [----]      (struct dummy_struct) {
  [----]      \t.a = 42, (esc)
  [----]     -\t.b = 24 (esc)
  [----]     +\t.b = 42 (esc)
  [----]      }
  [----] failmessages.c:104: Assertion Failed
  [----]   eq(type(struct dummy_struct)[1], &dummy1, &dummy2): 
  [----]     @@ -1,6 +1,6 @@
  [----]      (struct dummy_struct[1]) {
  [----]      \t[0] = (struct dummy_struct) { (esc)
  [----]      \t\t.a = 42, (esc)
  [----]     -\t\t.b = 24 (esc)
  [----]     +\t\t.b = 42 (esc)
  [----]      \t}, (esc)
  [----]      }
  [----] failmessages.c:124: Assertion Failed
  [----]   eq(stream, s1, s2): 
  [----]     @@ -1,2 +1,2 @@
  [----]     -00: 68656c6c 6f20776f 726c6400           hello world.    
  [----]     +00: 646c726f 77206f6c 6c656800           dlrow olleh.    
  [----]      
  [FAIL] messages::eq
  [====] Synthesis: Tested: 4 | Passing: 0 | Failing: 4 | Crashing: 0 

  $ failmessages.cc.bin
  [----] failmessages.cc:173: Assertion Failed
  [----]   eq(i32, 1, 0): 
  [----]     diff: [-1-]{+0+}
  [----] failmessages.cc:174: Assertion Failed
  [----] failmessages.cc:175: Assertion Failed
  [----]   eq(i32, 1, 1): 
  [----]     diff: [-1-]{+1+}
  [FAIL] message::compo
  [----] failmessages.cc:179: Assertion Failed
  [----]   throw(std::bad_alloc, throw std::invalid_argument("exception message")): 
  [----]     message: "exception message"
  [----] failmessages.cc:180: Assertion Failed
  [----] failmessages.cc:181: Assertion Failed
  [----]   nothrow(throw std::invalid_argument("exception message")): 
  [----]     message: "exception message"
  [----] failmessages.cc:182: Assertion Failed
  [FAIL] message::exception
  [----] failmessages.cc:131: Assertion Failed
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
  [----] failmessages.cc:132: Assertion Failed
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
  [----] failmessages.cc:133: Assertion Failed
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
  [----] failmessages.cc:134: Assertion Failed
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
  [----] failmessages.cc:135: Assertion Failed
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
  [----] failmessages.cc:136: Assertion Failed
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
  [----] failmessages.cc:137: Assertion Failed
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
  [----] failmessages.cc:138: Assertion Failed
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
  [----] failmessages.cc:139: Assertion Failed
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
  [----] failmessages.cc:140: Assertion Failed
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
  [----] failmessages.cc:141: Assertion Failed
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
  [----] failmessages.cc:142: Assertion Failed
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
  [----] failmessages.cc:143: Assertion Failed
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
  [----] failmessages.cc:146: Assertion Failed
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
  [----] failmessages.cc:148: Assertion Failed
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
  [----] failmessages.cc:149: Assertion Failed
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
  [----] failmessages.cc:151: Assertion Failed
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
  [----] failmessages.cc:152: Assertion Failed
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
  [----] failmessages.cc:169: Assertion Failed
  [----]   lt(stream, shi, slo): 
  [----]     actual: 00: 68656c6c 6f20776f 726c6400           hello world.    
  [----]       
  [----]     reference: 00: 646c726f 77206f6c 6c656800           dlrow olleh.    
  [----]       
  [----]   le(stream, shi, slo): 
  [----]     actual: 00: 68656c6c 6f20776f 726c6400           hello world.    
  [----]       
  [----]     reference: 00: 646c726f 77206f6c 6c656800           dlrow olleh.    
  [----]       
  [----]   gt(stream, slo, shi): 
  [----]     actual: 00: 646c726f 77206f6c 6c656800           dlrow olleh.    
  [----]       
  [----]     reference: 00: 68656c6c 6f20776f 726c6400           hello world.    
  [----]       
  [----]   ge(stream, slo, shi): 
  [----]     actual: 00: 646c726f 77206f6c 6c656800           dlrow olleh.    
  [----]       
  [----]     reference: 00: 68656c6c 6f20776f 726c6400           hello world.    
  [----]       
  [FAIL] messages::cmp
  [----] failmessages.cc:186: Assertion Failed
  [----] failmessages.cc:187: Assertion Failed
  [----]   
  [----]   foo bar
  [----]   
  [FAIL] messages::default
  [----] failmessages.cc:45: Assertion Failed
  [----]   eq(i8, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:46: Assertion Failed
  [----]   eq(i16, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:47: Assertion Failed
  [----]   eq(i32, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:48: Assertion Failed
  [----]   eq(i64, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:49: Assertion Failed
  [----]   eq(u8, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:50: Assertion Failed
  [----]   eq(u16, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:51: Assertion Failed
  [----]   eq(u32, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:52: Assertion Failed
  [----]   eq(u64, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:53: Assertion Failed
  [----]   eq(iptr, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:54: Assertion Failed
  [----]   eq(uptr, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:55: Assertion Failed
  [----]   eq(flt, 0, 1 / 3.f): 
  [----]     diff: [-0-]{+0.333333+}
  [----] failmessages.cc:56: Assertion Failed
  [----]   eq(dbl, 0, 1 / 3.): 
  [----]     diff: [-0-]{+0.333333+}
  [----] failmessages.cc:57: Assertion Failed
  [----]   eq(ldbl, 0, 1 / 3.l): 
  [----]     diff: [-0-]{+0.333333+}
  [----] failmessages.cc:60: Assertion Failed
  [----]   eq(ptr, (void *) 1, (void *) 0): 
  [----]     diff: [-@1-]{+nullptr+}
  [----] failmessages.cc:62: Assertion Failed
  [----]   eq(str, "", "foo"): 
  [----]     diff: [-""-]{+"foo"+}
  [----] failmessages.cc:65: Assertion Failed
  [----]   eq(str, "reallyreallylongstringindeedmygoodsirormadam", "yetanotherreallyreallylongstring"): 
  [----]     @@ -1,1 +1,1 @@
  [----]     -"reallyreallylongstringindeedmygoodsirormadam"
  [----]     +"yetanotherreallyreallylongstring"
  [----] failmessages.cc:66: Assertion Failed
  [----]   eq(str, "foo\nbar", "foo\nbaz"): 
  [----]     @@ -1,2 +1,2 @@
  [----]      "foo\n"
  [----]     -"bar"
  [----]     +"baz"
  [----] failmessages.cc:68: Assertion Failed
  [----]   eq(wcs, L"", L"foo"): 
  [----]     diff: [-L""-]{+L"foo"+}
  [----] failmessages.cc:71: Assertion Failed
  [----]   eq(wcs, L"reallyreallylongstringindeedmygoodsirormadam", L"yetanotherreallyreallylongstring"): 
  [----]     @@ -1,1 +1,1 @@
  [----]     -L"reallyreallylongstringindeedmygoodsirormadam"
  [----]     +L"yetanotherreallyreallylongstring"
  [----] failmessages.cc:72: Assertion Failed
  [----]   eq(wcs, L"foo\nbar", L"foo\nbaz"): 
  [----]     @@ -1,2 +1,2 @@
  [----]      L"foo\n"
  [----]     -L"bar"
  [----]     +L"baz"
  [----] failmessages.cc:80: Assertion Failed
  [----]   eq(mem, ma, mb): 
  [----]     @@ -1,2 +1,2 @@
  [----]     -00: 00                                   .               
  [----]     +00: 01                                   .               
  [----]      
  [----] failmessages.cc:81: Assertion Failed
  [----]   eq(int[1], &a, &b): 
  [----]     @@ -1,3 +1,3 @@
  [----]      (int[1]) {
  [----]     -\t[0] = 0, (esc)
  [----]     +\t[0] = 1, (esc)
  [----]      }
  [----] failmessages.cc:94: Assertion Failed
  [----]   eq(mem, marra, marrb): 
  [----]     @@ -1,2 +1,2 @@
  [----]     -00: 00000000 01                          .....           
  [----]     +00: 04000000 03                          .....           
  [----]      
  [----] failmessages.cc:95: Assertion Failed
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
  [----] failmessages.cc:100: Assertion Failed
  [----]   eq(type(struct dummy_struct), dummy1, dummy2): 
  [----]     @@ -1,4 +1,4 @@
  [----]      (struct dummy_struct) {
  [----]      \t.a = *, (esc)
  [----]     -\t.b = 24 (esc)
  [----]     +\t.b = 42 (esc)
  [----]      }
  [----] failmessages.cc:101: Assertion Failed
  [----]   eq(type(struct dummy_struct)[1], &dummy1, &dummy2): 
  [----]     @@ -1,6 +1,6 @@
  [----]      (struct dummy_struct[1]) {
  [----]      \t[0] = (struct dummy_struct) { (esc)
  [----]      \t\t.a = *, (esc)
  [----]     -\t\t.b = 24 (esc)
  [----]     +\t\t.b = 42 (esc)
  [----]      \t}, (esc)
  [----]      }
  [----] failmessages.cc:118: Assertion Failed
  [----]   eq(stream, s1, s2): 
  [----]     @@ -1,2 +1,2 @@
  [----]     -00: 68656c6c 6f20776f 726c6400           hello world.    
  [----]     +00: 646c726f 77206f6c 6c656800           dlrow olleh.    
  [----]      
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
