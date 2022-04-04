#!/bin/sh

while true; do
	if test "$1" = ""; then
		break
	fi
	cat "$1"  | sed -e 's/__APPLE__/__MACOS__/' > .tmp
	rm -f "$1"
	mv .tmp "$1"
	shift
done

rm -f .tmp

