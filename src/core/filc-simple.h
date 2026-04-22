#ifndef CRITERION_FILC_SIMPLE_H_
#define CRITERION_FILC_SIMPLE_H_

void cri_filc_reset_test_state(void);
void cri_filc_mark_failed(void);
void cri_filc_mark_skipped(void);
int cri_filc_test_failed(void);
int cri_filc_test_skipped(void);

#endif
