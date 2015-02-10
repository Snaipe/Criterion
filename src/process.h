#ifndef PROCESS_H_
# define PROCESS_H_

struct process;

enum status_kind {
    EXIT_STATUS,
    STOPPED,
    SIGNAL,
};

struct process_status {
    enum status_kind kind;
    int status;
};

void set_runner_pid(void);
bool is_runner(void);
struct process_status wait_proc(struct process *proc);
struct process *spawn_test_worker(struct criterion_test *test, void (*func)(struct criterion_test *));
struct event *worker_read_event(struct process *proc);

#endif /* !PROCESS_H_ */
