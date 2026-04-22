# Fil-C `filc-simple` mode notes

This repo supports building Criterion with the Fil-C toolchain using Meson option `-Dfilc-simple=true`.

Fil-C simple mode exists because the full Criterion runner (workers, protocol, output processing) is not currently compatible with Fil-C for this project, but we still want:

- A working `libcriterion.a` usable by downstream Fil-C builds
- A test runner that can execute tests in-process
- Some human-readable output while tests run

## Why `pizlonated_dl_iterate_phdr` can be “missing”

Fil-C is **not ABI-compatible** with “Yolo-C” (normal system C). One consequence is **symbol mangling**: functions referenced by Fil-C-compiled code are looked up under a `pizlonated_`-prefixed name.

So if code compiled with Fil-C calls `dl_iterate_phdr()`, the resulting link reference may be to:

- `pizlonated_dl_iterate_phdr`

If the Fil-C slice you are linking against does not provide that symbol, you will see link errors like:

```
undefined reference to `pizlonated_dl_iterate_phdr`
```

This is a known issue in some Fil-C configurations; see upstream Fil-C issue:

- [`Trying to use glibc’s dl_iterate_phdr gives error at link time`](https://github.com/pizlonator/fil-c/issues/111)

Background reading from Fil-C docs:

- Fil-C runtime and ABI model: [`Fil-C Runtime`](https://fil-c.org/runtime)
- Why Fil-C uses an isolated headers/libs “slice”: [`Pizfix`](https://fil-c.org/pizfix.html)

### Impact on Criterion

Criterion’s “section scanning” test discovery (used by the normal runner on ELF via `dl_iterate_phdr`) can trigger this link error when compiled with Fil-C.

To keep `filc-simple` usable under Fil-C, **`filc-simple` test discovery does not rely on `dl_iterate_phdr`**.

Instead, when `CRITERION_FILC_SIMPLE` is enabled, the `Test(...)` macro registers each test using a tiny constructor-time registry. The `filc-simple` runner enumerates tests from that registry.

## `filc-simple` output (“lightweight reporter”)

In `filc-simple`, tests run **in-process** and Criterion prints lightweight progress plus normal Criterion-style `RUN`/`PASS`/`FAIL` lines via the normal logger callbacks.

### What it prints

- **Runner progress** (minimal): `criterion (filc-simple): running ...`
- **During test execution**:
  - `RUN` lines are emitted before each test (logger `pre_init`)
  - `PASS`/`FAIL`/`SKIP` lines after each test (logger `post_test`)
  - `cr_log_info`/`cr_log_warn`/`cr_log_error` messages print during the run
- **End of run**:
  - A small `passed/failed/skipped` summary
  - A normal-logger “Synthesis” summary (`post_all`)

### Controlling verbosity

`filc-simple` respects `criterion_options.logging_threshold`:

- `--quiet` sets `criterion_options.logging_threshold = CRITERION_LOG_LEVEL_QUIET` and suppresses output.
- Other logging threshold values control which log levels print.

## Limitations of `filc-simple`

`filc-simple` is intentionally minimal and does not attempt to replicate the full runner feature set. In particular:

- No worker processes / protocol layer
- No diffs/theories features (as configured in the Fil-C SDK build scripts)
- Parameterized tests are rejected in `filc-simple` (by design)

