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

Testing all assert messages

  $ failmessages.c.bin
  [----] failmessages.c:213: Assertion Failed
  [----]   eq(i32, 1, 0): 
  [----]     diff: [-1-]{+0+}
  [----] failmessages.c:214: Assertion Failed
  [----] failmessages.c:215: Assertion Failed
  [----]   eq(i32, 1, 1): 
  [----]     diff: [-1-]{+1+}
  [FAIL] message::compo
  [----] failmessages.c:165: Assertion Failed
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
  [----] failmessages.c:166: Assertion Failed
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
  [----] failmessages.c:167: Assertion Failed
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
  [----] failmessages.c:168: Assertion Failed
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
  [----] failmessages.c:169: Assertion Failed
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
  [----] failmessages.c:170: Assertion Failed
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
  [----] failmessages.c:171: Assertion Failed
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
  [----] failmessages.c:172: Assertion Failed
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
  [----] failmessages.c:173: Assertion Failed
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
  [----] failmessages.c:174: Assertion Failed
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
  [----] failmessages.c:175: Assertion Failed
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
  [----] failmessages.c:176: Assertion Failed
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
  [----] failmessages.c:177: Assertion Failed
  [----]   lt(ldbl, 1 / 3.l, 0): 
  \[----\]     actual: 0\.3.* (re)
  [----]     reference: 0
  [----]   le(ldbl, 1 / 3.l, 0): 
  \[----\]     actual: 0\.3.* (re)
  [----]     reference: 0
  [----]   gt(ldbl, 0, 1 / 3.l): 
  [----]     actual: 0
  \[----\]     reference: 0\.3.* (re)
  [----]   ge(ldbl, 0, 1 / 3.l): 
  [----]     actual: 0
  \[----\]     reference: 0\.3.* (re)
  [----] failmessages.c:180: Assertion Failed
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
  [----] failmessages.c:182: Assertion Failed
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
  [----] failmessages.c:183: Assertion Failed
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
  [----] failmessages.c:185: Assertion Failed
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
  [----] failmessages.c:186: Assertion Failed
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
  [----] failmessages.c:206: Assertion Failed
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
  [----] failmessages.c:219: Assertion Failed
  [----] failmessages.c:220: Assertion Failed
  [----]   
  [----]   foo bar
  [----]   
  [FAIL] messages::default
  [----] failmessages.c:80: Assertion Failed
  [----]   eq(i8, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:81: Assertion Failed
  [----]   eq(i16, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:82: Assertion Failed
  [----]   eq(i32, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:83: Assertion Failed
  [----]   eq(i64, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:84: Assertion Failed
  [----]   eq(u8, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:85: Assertion Failed
  [----]   eq(u16, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:86: Assertion Failed
  [----]   eq(u32, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:87: Assertion Failed
  [----]   eq(u64, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.c:88: Assertion Failed
  [----]   eq(iptr, 0, 1): 
  [----]     diff: [-0x0-]{+0x1+}
  [----] failmessages.c:89: Assertion Failed
  [----]   eq(uptr, 0, 1): 
  [----]     diff: [-0x0-]{+0x1+}
  [----] failmessages.c:90: Assertion Failed
  [----]   eq(flt, 0, 1 / 3.f): 
  [----]     diff: [-0-]{+0.333333343+}
  [----] failmessages.c:91: Assertion Failed
  [----]   eq(dbl, 0, 1 / 3.): 
  [----]     diff: [-0-]{+0.33333333333333331+}
  [----] failmessages.c:92: Assertion Failed
  [----]   eq(ldbl, 0, 1 / 3.l): 
  \[----\]     diff: \[-0-\]\{\+0\.3.*\+\} (re)
  [----] failmessages.c:95: Assertion Failed
  [----]   eq(ptr, (void *) 1, (void *) 0): 
  [----]     diff: [-0x1-]{+0x0+}
  [----] failmessages.c:97: Assertion Failed
  [----]   eq(str, "", "foo"): 
  [----]     diff: [-""-]{+"foo"+}
  [----] failmessages.c:98: Assertion Failed
  [----]   eq(str, "reallyreallylongstringindeedmygoodsirormadam", "yetanotherreallyreallylongstring"): 
  [----]     @@ -1,1 +1,1 @@
  [----]     -"reallyreallylongstringindeedmygoodsirormadam"
  [----]     +"yetanotherreallyreallylongstring"
  [----] failmessages.c:99: Assertion Failed
  [----]   eq(str, "foo\nbar", "foo\nbaz"): 
  [----]     @@ -1,2 +1,2 @@
  [----]      "foo\n"
  [----]     -"bar"
  [----]     +"baz"
  [----] failmessages.c:101: Assertion Failed
  [----]   eq(wcs, L"", L"foo"): 
  [----]     diff: [-L""-]{+L"foo"+}
  [----] failmessages.c:102: Assertion Failed
  [----]   eq(wcs, L"reallyreallylongstringindeedmygoodsirormadam", L"yetanotherreallyreallylongstring"): 
  [----]     @@ -1,1 +1,1 @@
  [----]     -L"reallyreallylongstringindeedmygoodsirormadam"
  [----]     +L"yetanotherreallyreallylongstring"
  [----] failmessages.c:103: Assertion Failed
  [----]   eq(wcs, L"foo\nbar", L"foo\nbaz"): 
  [----]     @@ -1,2 +1,2 @@
  [----]      L"foo\n"
  [----]     -L"bar"
  [----]     +L"baz"
  [----] failmessages.c:111: Assertion Failed
  [----]   eq(mem, ma, mb): 
  [----]     @@ -1,2 +1,2 @@
  [----]     -00: 00000000                             ....            
  [----]     +00: 01000000                             ....            
  [----]      
  [----] failmessages.c:112: Assertion Failed
  [----]   eq(int[1], &a, &b): 
  [----]     @@ -1,3 +1,3 @@
  [----]      (int[1]) {
  [----]     -\t[0] = 0, (esc)
  [----]     +\t[0] = 1, (esc)
  [----]      }
  [----] failmessages.c:125: Assertion Failed
  [----]   eq(mem, marra, marrb): 
  [----]     @@ -1,3 +1,3 @@
  [----]     -00: 00000000 01000000 02000000 03000000  ................
  [----]     -10: 04000000                             ....            
  [----]     +00: 04000000 03000000 02000000 01000000  ................
  [----]     +10: 00000000                             ....            
  [----]      
  [----] failmessages.c:126: Assertion Failed
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
  [----] failmessages.c:131: Assertion Failed
  [----]   eq(type(struct dummy_struct), dummy1, dummy2): 
  [----]     @@ -1,4 +1,4 @@
  [----]      (struct dummy_struct) {
  [----]      \t.a = 42, (esc)
  [----]     -\t.b = 24 (esc)
  [----]     +\t.b = 42 (esc)
  [----]      }
  [----] failmessages.c:132: Assertion Failed
  [----]   eq(type(struct dummy_struct)[1], &dummy1, &dummy2): 
  [----]     @@ -1,6 +1,6 @@
  [----]      (struct dummy_struct[1]) {
  [----]      \t[0] = (struct dummy_struct) { (esc)
  [----]      \t\t.a = 42, (esc)
  [----]     -\t\t.b = 24 (esc)
  [----]     +\t\t.b = 42 (esc)
  [----]      \t}, (esc)
  [----]      }
  [----] failmessages.c:152: Assertion Failed
  [----]   eq(stream, s1, s2): 
  [----]     @@ -1,2 +1,2 @@
  [----]     -00: 68656c6c 6f20776f 726c6400           hello world.    
  [----]     +00: 646c726f 77206f6c 6c656800           dlrow olleh.    
  [----]      
  [FAIL] messages::eq
  [----] failmessages.c:53: Assertion Failed
  [----]   zero(i8, 0): 
  [----]     value: 0
  [----] failmessages.c:54: Assertion Failed
  [----]   zero(i16, 0): 
  [----]     value: 0
  [----] failmessages.c:55: Assertion Failed
  [----]   zero(i32, 0): 
  [----]     value: 0
  [----] failmessages.c:56: Assertion Failed
  [----]   zero(i64, 0): 
  [----]     value: 0
  [----] failmessages.c:57: Assertion Failed
  [----]   zero(u8, 0): 
  [----]     value: 0
  [----] failmessages.c:58: Assertion Failed
  [----]   zero(u16, 0): 
  [----]     value: 0
  [----] failmessages.c:59: Assertion Failed
  [----]   zero(u32, 0): 
  [----]     value: 0
  [----] failmessages.c:60: Assertion Failed
  [----]   zero(u64, 0): 
  [----]     value: 0
  [----] failmessages.c:61: Assertion Failed
  [----]   zero(iptr, 0): 
  [----]     value: 0x0
  [----] failmessages.c:62: Assertion Failed
  [----]   zero(uptr, 0): 
  [----]     value: 0x0
  [----] failmessages.c:63: Assertion Failed
  [----]   zero(flt, 0): 
  [----]     value: 0
  [----] failmessages.c:64: Assertion Failed
  [----]   zero(dbl, 0): 
  [----]     value: 0
  [----] failmessages.c:65: Assertion Failed
  [----]   zero(ldbl, 0): 
  [----]     value: 0
  [----] failmessages.c:68: Assertion Failed
  [----]   zero(ptr, 0): 
  [----]     value: 0x0
  [----] failmessages.c:69: Assertion Failed
  [----]   zero(str, ""): 
  [----]     value: ""
  [----] failmessages.c:70: Assertion Failed
  [----]   zero(wcs, L""): 
  [----]     value: L""
  [----] failmessages.c:75: Assertion Failed
  [----]   zero(type(struct dummy_struct), dummy1): 
  [----]     value: (struct dummy_struct) {
  [----]       \t.a = 0, (esc)
  [----]       \t.b = 0 (esc)
  [----]       }
  [FAIL] messages::zero
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

Using redirections

  $ redirect.c.bin
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 

C++ equivalents

  $ [ "$CXX_SUPPORT" = 1 ] || exit 80

  $ redirect.cc.bin
  [====] Synthesis: Tested: 2 | Passing: 2 | Failing: 0 | Crashing: 0 

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

  $ failmessages.cc.bin
  [----] failmessages.cc:214: Assertion Failed
  [----]   eq(i32, 1, 0): 
  [----]     diff: [-1-]{+0+}
  [----] failmessages.cc:215: Assertion Failed
  [----] failmessages.cc:216: Assertion Failed
  [----]   eq(i32, 1, 1): 
  [----]     diff: [-1-]{+1+}
  [FAIL] message::compo
  [----] failmessages.cc:220: Assertion Failed
  [----]   throw(std::bad_alloc, throw std::invalid_argument("exception message")): 
  [----]     message: "exception message"
  [----] failmessages.cc:221: Assertion Failed
  [----] failmessages.cc:222: Assertion Failed
  [----]   nothrow(throw std::invalid_argument("exception message")): 
  [----]     message: "exception message"
  [----] failmessages.cc:223: Assertion Failed
  [FAIL] message::exception
  [----] failmessages.cc:172: Assertion Failed
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
  [----] failmessages.cc:173: Assertion Failed
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
  [----] failmessages.cc:174: Assertion Failed
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
  [----] failmessages.cc:175: Assertion Failed
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
  [----] failmessages.cc:176: Assertion Failed
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
  [----] failmessages.cc:177: Assertion Failed
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
  [----] failmessages.cc:178: Assertion Failed
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
  [----] failmessages.cc:179: Assertion Failed
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
  [----] failmessages.cc:180: Assertion Failed
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
  [----] failmessages.cc:181: Assertion Failed
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
  [----] failmessages.cc:182: Assertion Failed
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
  [----] failmessages.cc:183: Assertion Failed
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
  [----] failmessages.cc:184: Assertion Failed
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
  [----] failmessages.cc:187: Assertion Failed
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
  [----] failmessages.cc:189: Assertion Failed
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
  [----] failmessages.cc:190: Assertion Failed
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
  [----] failmessages.cc:192: Assertion Failed
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
  [----] failmessages.cc:193: Assertion Failed
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
  [----] failmessages.cc:210: Assertion Failed
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
  [----] failmessages.cc:227: Assertion Failed
  [----] failmessages.cc:228: Assertion Failed
  [----]   
  [----]   foo bar
  [----]   
  [FAIL] messages::default
  [----] failmessages.cc:79: Assertion Failed
  [----]   eq(i8, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:80: Assertion Failed
  [----]   eq(i16, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:81: Assertion Failed
  [----]   eq(i32, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:82: Assertion Failed
  [----]   eq(i64, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:83: Assertion Failed
  [----]   eq(u8, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:84: Assertion Failed
  [----]   eq(u16, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:85: Assertion Failed
  [----]   eq(u32, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:86: Assertion Failed
  [----]   eq(u64, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:87: Assertion Failed
  [----]   eq(iptr, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:88: Assertion Failed
  [----]   eq(uptr, 0, 1): 
  [----]     diff: [-0-]{+1+}
  [----] failmessages.cc:89: Assertion Failed
  [----]   eq(flt, 0, 1 / 3.f): 
  [----]     diff: [-0-]{+0.333333+}
  [----] failmessages.cc:90: Assertion Failed
  [----]   eq(dbl, 0, 1 / 3.): 
  [----]     diff: [-0-]{+0.333333+}
  [----] failmessages.cc:91: Assertion Failed
  [----]   eq(ldbl, 0, 1 / 3.l): 
  [----]     diff: [-0-]{+0.333333+}
  [----] failmessages.cc:94: Assertion Failed
  [----]   eq(ptr, (void *) 1, (void *) 0): 
  [----]     diff: [-@1-]{+nullptr+}
  [----] failmessages.cc:96: Assertion Failed
  [----]   eq(str, "", "foo"): 
  [----]     diff: [-""-]{+"foo"+}
  [----] failmessages.cc:97: Assertion Failed
  [----]   eq(str, "reallyreallylongstringindeedmygoodsirormadam", "yetanotherreallyreallylongstring"): 
  [----]     @@ -1,1 +1,1 @@
  [----]     -"reallyreallylongstringindeedmygoodsirormadam"
  [----]     +"yetanotherreallyreallylongstring"
  [----] failmessages.cc:98: Assertion Failed
  [----]   eq(str, "foo\nbar", "foo\nbaz"): 
  [----]     @@ -1,2 +1,2 @@
  [----]      "foo\n"
  [----]     -"bar"
  [----]     +"baz"
  [----] failmessages.cc:100: Assertion Failed
  [----]   eq(wcs, L"", L"foo"): 
  [----]     diff: [-L""-]{+L"foo"+}
  [----] failmessages.cc:101: Assertion Failed
  [----]   eq(wcs, L"reallyreallylongstringindeedmygoodsirormadam", L"yetanotherreallyreallylongstring"): 
  [----]     @@ -1,1 +1,1 @@
  [----]     -L"reallyreallylongstringindeedmygoodsirormadam"
  [----]     +L"yetanotherreallyreallylongstring"
  [----] failmessages.cc:102: Assertion Failed
  [----]   eq(wcs, L"foo\nbar", L"foo\nbaz"): 
  [----]     @@ -1,2 +1,2 @@
  [----]      L"foo\n"
  [----]     -L"bar"
  [----]     +L"baz"
  [----] failmessages.cc:110: Assertion Failed
  [----]   eq(mem, ma, mb): 
  [----]     @@ -1,2 +1,2 @@
  [----]     -00: 00                                   .               
  [----]     +00: 01                                   .               
  [----]      
  [----] failmessages.cc:111: Assertion Failed
  [----]   eq(int[1], &a, &b): 
  [----]     @@ -1,3 +1,3 @@
  [----]      (int[1]) {
  [----]     -\t[0] = 0, (esc)
  [----]     +\t[0] = 1, (esc)
  [----]      }
  [----] failmessages.cc:124: Assertion Failed
  [----]   eq(mem, marra, marrb): 
  [----]     @@ -1,2 +1,2 @@
  [----]     -00: 00000000 01                          .....           
  [----]     +00: 04000000 03                          .....           
  [----]      
  [----] failmessages.cc:125: Assertion Failed
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
  [----] failmessages.cc:130: Assertion Failed
  [----]   eq(type(struct dummy_struct), dummy1, dummy2): 
  [----]     @@ -1,4 +1,4 @@
  [----]      (struct dummy_struct) {
  [----]      \t.a = 42, (esc)
  [----]     -\t.b = 24 (esc)
  [----]     +\t.b = 42 (esc)
  [----]      }
  [----] failmessages.cc:131: Assertion Failed
  [----]   eq(type(struct dummy_struct)[1], &dummy1, &dummy2): 
  [----]     @@ -1,6 +1,6 @@
  [----]      (struct dummy_struct[1]) {
  [----]      \t[0] = (struct dummy_struct) { (esc)
  [----]      \t\t.a = 42, (esc)
  [----]     -\t\t.b = 24 (esc)
  [----]     +\t\t.b = 42 (esc)
  [----]      \t}, (esc)
  [----]      }
  [----] failmessages.cc:148: Assertion Failed
  [----]   eq(stream, s1, s2): 
  [----]     @@ -1,2 +1,2 @@
  [----]     -00: 68656c6c 6f20776f 726c6400           hello world.    
  [----]     +00: 646c726f 77206f6c 6c656800           dlrow olleh.    
  [----]      
  [----] failmessages.cc:154: Assertion Failed
  [----]   eq(type(int_vect), vec1, vec2): 
  [----]     @@ -1,5 +1,5 @@
  [----]      {
  [----]     -\t1,  (esc)
  [----]     -\t2,  (esc)
  [----]      \t3,  (esc)
  [----]     +\t2,  (esc)
  [----]     +\t1,  (esc)
  [----]      }
  [----] failmessages.cc:159: Assertion Failed
  [----]   eq(type(string_int_map), m1, m2): 
  [----]     @@ -1,4 +1,4 @@
  [----]      {
  [----]     -\t["hello"]: 1,  (esc)
  [----]     -\t["world"]: 2,  (esc)
  [----]     +\t["all"]: 1,  (esc)
  [----]     +\t["hello"]: 2,  (esc)
  [----]      }
  [FAIL] messages::eq
  [----] failmessages.cc:48: Assertion Failed
  [----]   zero(i8, 0): 
  [----]     value: 0
  [----] failmessages.cc:49: Assertion Failed
  [----]   zero(i16, 0): 
  [----]     value: 0
  [----] failmessages.cc:50: Assertion Failed
  [----]   zero(i32, 0): 
  [----]     value: 0
  [----] failmessages.cc:51: Assertion Failed
  [----]   zero(i64, 0): 
  [----]     value: 0
  [----] failmessages.cc:52: Assertion Failed
  [----]   zero(u8, 0): 
  [----]     value: 0
  [----] failmessages.cc:53: Assertion Failed
  [----]   zero(u16, 0): 
  [----]     value: 0
  [----] failmessages.cc:54: Assertion Failed
  [----]   zero(u32, 0): 
  [----]     value: 0
  [----] failmessages.cc:55: Assertion Failed
  [----]   zero(u64, 0): 
  [----]     value: 0
  [----] failmessages.cc:56: Assertion Failed
  [----]   zero(iptr, 0): 
  [----]     value: 0
  [----] failmessages.cc:57: Assertion Failed
  [----]   zero(uptr, 0): 
  [----]     value: 0
  [----] failmessages.cc:58: Assertion Failed
  [----]   zero(flt, 0): 
  [----]     value: 0
  [----] failmessages.cc:59: Assertion Failed
  [----]   zero(dbl, 0): 
  [----]     value: 0
  [----] failmessages.cc:60: Assertion Failed
  [----]   zero(ldbl, 0): 
  [----]     value: 0
  [----] failmessages.cc:63: Assertion Failed
  [----]   zero(ptr, 0): 
  [----]     value: nullptr
  [----] failmessages.cc:64: Assertion Failed
  [----]   zero(str, ""): 
  [----]     value: ""
  [----] failmessages.cc:65: Assertion Failed
  [----]   zero(wcs, L""): 
  [----]     value: L""
  [----] failmessages.cc:67: Assertion Failed
  [----]   zero(type(dummy_struct), dummy_struct{}): 
  [----]     value: (struct dummy_struct) {
  [----]       \t.a = 0, (esc)
  [----]       \t.b = 0 (esc)
  [----]       }
  [----] failmessages.cc:71: Assertion Failed
  [----]   zero(type(int_vect), int_vect{}): 
  [----]     value: {}
  [----] failmessages.cc:74: Assertion Failed
  [----]   zero(type(string_int_map), string_int_map{}): 
  [----]     value: {}
  [FAIL] messages::zero
  [====] Synthesis: Tested: 6 | Passing: 0 | Failing: 6 | Crashing: 0 

Test C++ assertions:

  $ asserts.cc.bin
  [----] asserts.cc:16: Assertion Failed
  [----]   
  [----]   assert is fatal, expect isn't
  [----]   
  [----] asserts.cc:17: Assertion Failed
  [----]   
  [----]   This assert runs
  [----]   
  [FAIL] asserts::base
  [----] asserts.cc:168: Assertion Failed
  [----]   throw(std::runtime_error, {}): 
  [----]     message: <nothing was thrown>
  [----] asserts.cc:170: Assertion Failed
  [----]   throw(std::bad_alloc, throw std::invalid_argument("some other message")): 
  [----]     message: "some other message"
  [FAIL] asserts::exception
  [----] asserts.cc:22: Assertion Failed
  [----]   
  [----]   You can fail an assertion with a message from anywhere
  [----]   
  [----] asserts.cc:23: Assertion Failed
  [FAIL] asserts::old_school
  [====] Synthesis: Tested: 10 | Passing: 7 | Failing: 3 | Crashing: 0 
