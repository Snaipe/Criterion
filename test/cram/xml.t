Testing multiple samples with --xml

  $ simple.c.bin --xml
  <?xml version="1.0" encoding="UTF-8"?>
  <!-- Tests compiled with Criterion v2.3.0-dev -->
  <testsuites name="Criterion Tests" tests="2" failures="1" errors="0" disabled="0">
    <testsuite name="misc" tests="2" failures="1" errors="0" disabled="0" skipped="0" time="0.000">
      <testcase name="passing" assertions="1" status="PASSED" time="0.000">
      </testcase>
      <testcase name="failing" assertions="1" status="FAILED" time="0.000">
        <failure type="assert" message="1 assertion(s) failed.">simple.c:4: The expression 0 is false.&#10;</failure>
      </testcase>
    </testsuite>
  </testsuites>

  $ signal.c.bin --xml
  <?xml version="1.0" encoding="UTF-8"?>
  <!-- Tests compiled with Criterion v2.3.0-dev -->
  <testsuites name="Criterion Tests" tests="3" failures="2" errors="1" disabled="0">
    <testsuite name="simple" tests="3" failures="2" errors="1" disabled="0" skipped="0" time="0.000">
      <testcase name="wrong_signal" assertions="0" status="FAILED" time="0.000">
        <failure type="assert" message="0 assertion(s) failed."></failure>
      </testcase>
      <testcase name="uncaught" assertions="0" status="ERRORED" time="0.000">
        <error type="crash" message="The test crashed." />    </testcase>
      <testcase name="caught" assertions="0" status="PASSED" time="0.000">
      </testcase>
    </testsuite>
  </testsuites>

  $ asserts.c.bin --xml
  <?xml version="1.0" encoding="UTF-8"?>
  <!-- Tests compiled with Criterion v2.3.0-dev -->
  <testsuites name="Criterion Tests" tests="7" failures="2" errors="0" disabled="0">
    <testsuite name="asserts" tests="7" failures="2" errors="0" disabled="0" skipped="0" time="0.000">
      <testcase name="wstring" assertions="10" status="PASSED" time="0.000">
      </testcase>
      <testcase name="string" assertions="10" status="PASSED" time="0.000">
      </testcase>
      <testcase name="old_school" assertions="2" status="FAILED" time="0.000">
        <failure type="assert" message="2 assertion(s) failed.">asserts.c:18: The conditions for this assertion were not met.&#10;asserts.c:17: You can fail an assertion with a message from anywhere&#10;</failure>
      </testcase>
      <testcase name="native" assertions="8" status="PASSED" time="0.000">
      </testcase>
      <testcase name="float" assertions="2" status="PASSED" time="0.000">
      </testcase>
      <testcase name="base" assertions="6" status="FAILED" time="0.000">
        <failure type="assert" message="2 assertion(s) failed.">asserts.c:12: This assert runs&#10;asserts.c:11: assert is fatal, expect isn't&#10;</failure>
      </testcase>
      <testcase name="array" assertions="3" status="PASSED" time="0.000">
      </testcase>
    </testsuite>
  </testsuites>

  $ more-suites.c.bin --xml
  <?xml version="1.0" encoding="UTF-8"?>
  <!-- Tests compiled with Criterion v2.3.0-dev -->
  <testsuites name="Criterion Tests" tests="3" failures="0" errors="0" disabled="1">
    <testsuite name="suite2" tests="1" failures="0" errors="0" disabled="0" skipped="0" time="0.000">
      <testcase name="test" assertions="1" status="PASSED" time="0.000">
      </testcase>
    </testsuite>
    <testsuite name="suite1" tests="1" failures="0" errors="0" disabled="0" skipped="0" time="0.000">
      <testcase name="test" assertions="1" status="PASSED" time="0.000">
      </testcase>
    </testsuite>
    <testsuite name="disabled" tests="1" failures="0" errors="0" disabled="1" skipped="1" time="0.000">
      <testcase name="test" assertions="0" status="SKIPPED" time="0.000">
        <skipped/>
      </testcase>
    </testsuite>
  </testsuites>

  $ long-messages.c.bin --xml
  <?xml version="1.0" encoding="UTF-8"?>
  <!-- Tests compiled with Criterion v2.3.0-dev -->
  <testsuites name="Criterion Tests" tests="1" failures="1" errors="0" disabled="0">
    <testsuite name="sample" tests="1" failures="1" errors="0" disabled="0" skipped="0" time="0.000">
      <testcase name="long_msg" assertions="1" status="FAILED" time="0.000">
        <failure type="assert" message="1 assertion(s) failed.">long-messages.c:4: This is&#10;        A long message&#10;        Spawning multiple lines.&#10;        Formatting is respected.&#10;</failure>
      </testcase>
    </testsuite>
  </testsuites>

  $ description.c.bin --xml
  <?xml version="1.0" encoding="UTF-8"?>
  <!-- Tests compiled with Criterion v2.3.0-dev -->
  <testsuites name="Criterion Tests" tests="2" failures="1" errors="0" disabled="1">
    <testsuite name="misc" tests="2" failures="1" errors="0" disabled="1" skipped="1" time="0.000">
      <testcase name="skipped" assertions="0" status="SKIPPED" time="0.000">
        <skipped/>
      </testcase>
      <testcase name="failing" assertions="1" status="FAILED" time="0.000">
        <failure type="assert" message="1 assertion(s) failed.">description.c:4: The expression 0 is false.&#10;</failure>
      </testcase>
    </testsuite>
  </testsuites>

Testing --output=xml

  $ simple.c.bin --output=xml:-
  <?xml version="1.0" encoding="UTF-8"?>
  <!-- Tests compiled with Criterion v2.3.0-dev -->
  <testsuites name="Criterion Tests" tests="2" failures="1" errors="0" disabled="0">
    <testsuite name="misc" tests="2" failures="1" errors="0" disabled="0" skipped="0" time="0.000">
      <testcase name="passing" assertions="1" status="PASSED" time="0.000">
      </testcase>
      <testcase name="failing" assertions="1" status="FAILED" time="0.000">
        <failure type="assert" message="1 assertion(s) failed.">simple.c:4: The expression 0 is false.&#10;</failure>
      </testcase>
    </testsuite>
  </testsuites>

Testing CRITERION_OUTPUTS

  $ CRITERION_OUTPUTS=xml:- simple.c.bin
  <?xml version="1.0" encoding="UTF-8"?>
  <!-- Tests compiled with Criterion v2.3.0-dev -->
  <testsuites name="Criterion Tests" tests="2" failures="1" errors="0" disabled="0">
    <testsuite name="misc" tests="2" failures="1" errors="0" disabled="0" skipped="0" time="0.000">
      <testcase name="passing" assertions="1" status="PASSED" time="0.000">
      </testcase>
      <testcase name="failing" assertions="1" status="FAILED" time="0.000">
        <failure type="assert" message="1 assertion(s) failed.">simple.c:4: The expression 0 is false.&#10;</failure>
      </testcase>
    </testsuite>
  </testsuites>



  $ skip.c.bin --xml
  <?xml version="1.0" encoding="UTF-8"?>
  <!-- Tests compiled with Criterion v2.3.0-dev -->
  <testsuites name="Criterion Tests" tests="2" failures="0" errors="0" disabled="2">
    <testsuite name="misc" tests="2" failures="0" errors="0" disabled="2" skipped="2" time="0.000">
      <testcase name="skipping" assertions="0" status="SKIPPED" time="0.000">
        <skipped/>
      </testcase>
      <testcase name="message" assertions="0" status="SKIPPED" time="0.000">
        <skipped/>
      </testcase>
    </testsuite>
  </testsuites>
