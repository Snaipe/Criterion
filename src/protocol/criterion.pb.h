/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.5-dev at Fri Dec 11 17:13:49 2015. */

#ifndef PB_CRITERION_PB_H_INCLUDED
#define PB_CRITERION_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
typedef enum _criterion_protocol_death_result_type {
    criterion_protocol_death_result_type_NORMAL = 0,
    criterion_protocol_death_result_type_CRASH = 1,
    criterion_protocol_death_result_type_TIMEOUT = 2
} criterion_protocol_death_result_type;

/* Struct definitions */
typedef struct _criterion_protocol_birth {
    uint8_t dummy_field;
} criterion_protocol_birth;

typedef struct _criterion_protocol_post_fini {
    uint8_t dummy_field;
} criterion_protocol_post_fini;

typedef struct _criterion_protocol_pre_init {
    uint8_t dummy_field;
} criterion_protocol_pre_init;

typedef struct _criterion_protocol_assert {
    char *message;
    bool passed;
    char *file;
    bool has_line;
    int64_t line;
} criterion_protocol_assert;

typedef struct _criterion_protocol_death {
    bool has_result;
    criterion_protocol_death_result_type result;
    bool has_status;
    int64_t status;
} criterion_protocol_death;

typedef struct _criterion_protocol_post_test {
    bool has_timestamp;
    int64_t timestamp;
} criterion_protocol_post_test;

typedef struct _criterion_protocol_pre_test {
    bool has_timestamp;
    int64_t timestamp;
} criterion_protocol_pre_test;

typedef struct _criterion_protocol_submessage {
    pb_size_t which_value;
    union {
        criterion_protocol_birth birth;
        criterion_protocol_pre_init pre_init;
        criterion_protocol_pre_test pre_test;
        criterion_protocol_post_test post_test;
        criterion_protocol_post_fini post_fini;
        criterion_protocol_death death;
        criterion_protocol_assert assert;
    } value;
} criterion_protocol_submessage;

typedef struct _criterion_protocol_msg {
    int32_t version;
    pb_size_t which_id;
    union {
        int64_t pid;
    } id;
    criterion_protocol_submessage data;
} criterion_protocol_msg;

/* Default values for struct fields */
extern const int32_t criterion_protocol_msg_version_default;

/* Initializer values for message structs */
#define criterion_protocol_birth_init_default    {0}
#define criterion_protocol_pre_init_init_default {0}
#define criterion_protocol_pre_test_init_default {false, 0}
#define criterion_protocol_post_test_init_default {false, 0}
#define criterion_protocol_post_fini_init_default {0}
#define criterion_protocol_death_init_default    {false, (criterion_protocol_death_result_type)0, false, 0}
#define criterion_protocol_assert_init_default   {NULL, 0, NULL, false, 0}
#define criterion_protocol_submessage_init_default {0, {criterion_protocol_birth_init_default}}
#define criterion_protocol_msg_init_default      {1, 0, {0}, criterion_protocol_submessage_init_default}
#define criterion_protocol_birth_init_zero       {0}
#define criterion_protocol_pre_init_init_zero    {0}
#define criterion_protocol_pre_test_init_zero    {false, 0}
#define criterion_protocol_post_test_init_zero   {false, 0}
#define criterion_protocol_post_fini_init_zero   {0}
#define criterion_protocol_death_init_zero       {false, (criterion_protocol_death_result_type)0, false, 0}
#define criterion_protocol_assert_init_zero      {NULL, 0, NULL, false, 0}
#define criterion_protocol_submessage_init_zero  {0, {criterion_protocol_birth_init_zero}}
#define criterion_protocol_msg_init_zero         {0, 0, {0}, criterion_protocol_submessage_init_zero}

/* Field tags (for use in manual encoding/decoding) */
#define criterion_protocol_assert_message_tag    1
#define criterion_protocol_assert_passed_tag     2
#define criterion_protocol_assert_file_tag       3
#define criterion_protocol_assert_line_tag       4
#define criterion_protocol_death_result_tag      1
#define criterion_protocol_death_status_tag      2
#define criterion_protocol_post_test_timestamp_tag 1
#define criterion_protocol_pre_test_timestamp_tag 1
#define criterion_protocol_submessage_birth_tag  1
#define criterion_protocol_submessage_pre_init_tag 2
#define criterion_protocol_submessage_pre_test_tag 3
#define criterion_protocol_submessage_post_test_tag 4
#define criterion_protocol_submessage_post_fini_tag 5
#define criterion_protocol_submessage_death_tag  6
#define criterion_protocol_submessage_assert_tag 7
#define criterion_protocol_msg_pid_tag           2
#define criterion_protocol_msg_version_tag       1
#define criterion_protocol_msg_data_tag          16

/* Struct field encoding specification for nanopb */
extern const pb_field_t criterion_protocol_birth_fields[1];
extern const pb_field_t criterion_protocol_pre_init_fields[1];
extern const pb_field_t criterion_protocol_pre_test_fields[2];
extern const pb_field_t criterion_protocol_post_test_fields[2];
extern const pb_field_t criterion_protocol_post_fini_fields[1];
extern const pb_field_t criterion_protocol_death_fields[3];
extern const pb_field_t criterion_protocol_assert_fields[5];
extern const pb_field_t criterion_protocol_submessage_fields[8];
extern const pb_field_t criterion_protocol_msg_fields[4];

/* Maximum encoded size of messages (where known) */
#define criterion_protocol_birth_size            0
#define criterion_protocol_pre_init_size         0
#define criterion_protocol_pre_test_size         11
#define criterion_protocol_post_test_size        11
#define criterion_protocol_post_fini_size        0
#define criterion_protocol_death_size            13
#define criterion_protocol_msg_size              (29 + criterion_protocol_submessage_size)

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define CRITERION_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif