BEGIN {
    atc = ""
    output_dir = "c"
    case_num = 0
    description = ""
    if (!bin_dir_symbol) {
        print "Undefined: bin_dir_symbol (set via CL: -bin_dir_symbol=VALUE"
        exit 1
    }
}

$0 ~ /^[^ ]/ {
    description = $0
    print "READING description: " description
    next
}

$0 ~ /^  \$ .*[a-z_-]*\.cc?\.bin/ {
    case_num += 1;
    body_line_idx = 0;
    delete body_lines;
    atc = substr($0,5);
    tc_written = 0
    printf("READING start   test %2d %s: %s\n",case_num, output_dir, atc);
    next;
}

$0 ~ /CXX_SUPPORT/ {
    print "READING start C++ tests"
    output_dir = "cxx"
    case_num = 0
    description = ""
    next
}

$0 == "" {
    print("<separator>");
    maybe_write_test();
    next;
}

END {
    print("EOF");
    maybe_write_test();
}

$0 ~ /^  / {
    body_lines[body_line_idx++] = substr($0,3);
    next;
}

function maybe_write_test() {
    if (atc != "") {
        printf("WRITING test %3s/ %2d %s\n", output_dir, case_num, atc);
        print "   Description: " description
        file_name = output_dir "/" case_num".case"
        print test_case() > file_name
        close(file_name)
        atc = ""
    }
}

function test_case_code() {
    delete lines;
    lines[0] = "[act]"
    lines[1] = ""
    lines[2] = "@[" bin_dir_symbol "]@/" atc
    lines[3] = ""
    lines[4] = "[assert]"
    lines[5] = ""
    lines[6] = assertion_header()
    lines[7] = "<<EOF"
    lines[8] = expected_str()
    lines[9] = "EOF"

    return concat(lines)
}

function expected_str() {
    if (body_has_version())
        body_lines[0] = "[----] Criterion <VERSION>"
    return concat(body_lines)
}

function test_case() {
    return test_case_description() test_case_code()
}

function test_case_description() {
    if (description == "") {
        return ""
    }
    else {
        return "# " description "\n\n"
    }
}

function body_has_version() {
    return body_lines[0] == "\[----\] Criterion v.* (re)"
}

function assertion_header()
{
    if (body_has_version())
        return "stderr\n  -transformed-by replace_version\n  equals"
    else
        return "stderr equals"
}

function concat(arr) {
    ret_val = ""
    for (i=0; i < length(arr)-1; ++i) {
        ret_val = ret_val arr[i] "\n";
    }
    ret_val = ret_val arr[i];
    
    return ret_val;
}
