# GDB script to help with multi-inferior debugging on Criterion
# run with gdb ./your-test -x criterion.gdb

set follow-fork-mode child
set detach-on-fork off
set non-stop on
set target-async on
set schedule-multiple on
set print inferior-events off
set print thread-events off
handle SIGSTOP SIGCONT nostop noprint pass

define hookpost-start
    set criterion_options.crash = 1
end

define hook-run
    set breakpoint pending on
    tbreak main
    commands
        silent
        set criterion_options.crash = 1
        continue
    end
end
