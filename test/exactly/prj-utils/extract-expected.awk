BEGIN {
    found = 0
}

/^<<EOF/ {
    found = 1
    next
}

/^EOF/ {
    exit 0
}

{
    if (found == 1)
        print $0
}
