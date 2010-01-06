#!/bin/sh
# $Id$

base=
logfile=
map=

while :; do
    case "$1" in
        -base ) base=$2; shift 2 ;;
        -log  ) logfile=$2; shift 2 ;;
        -map  ) map=$2; shift 2 ;;
        *     ) break ;;
    esac
done
: ${base:=`basename "$1"`}

clean_up () {
    rm -rf "$base.lock"
}

case $0 in
    */*) get_lock="`dirname $0`/get_lock.sh" ;;
    *) get_lock=get_lock.sh ;;
esac

if [ -f "$map" ]; then
    while read old new; do
        if [ "x$base" = "xmake_$old" ]; then
            echo "$0: adjusting base from $base to make_$new per $map."
            base=make_$new
            break
        fi
    done < "$map"
fi

if "$get_lock" "$base" $$; then
    trap 'clean_up' 1 2 15
    if [ -n "$logfile" ]; then
        status_file=$base.lock/status
        ("$@"; echo $? > "$status_file") 2>&1 | tee "$logfile"
        if [ -s "$status_file" ]; then
            status=`cat "$status_file"`
        else
            status=1
        fi
    else
        "$@"
        status=$?
    fi
    clean_up
    exit $status
else
    exit 1
fi
