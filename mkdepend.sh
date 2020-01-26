#! /bin/sh
# Maintainer helper script for updating project dependencies. Not needed for
# just building the project.
#
# Version 2020.26.1
# Copyright (c) 2020 Guenther Brunthaler. All rights reserved.
#
# This script is free software.
# Distribution is permitted under the terms of the GPLv3.

set -e
trap 'test $? = 0 || echo "\"$0\" failed!"' 0

println() {
	printf '%s\n' "$*"
}

for src in *.c
do
	obj=${src%.*}.o
	rm -f -- "$obj"
	make -s CPPFLAGS="$CPPFLAGS -MM" "$obj"
	cat < "$obj"
done \
| while read line
do
	println "$line" # Unfold continuation lines.
done | LC_COLLATE=C sort \
| {
	set -f
	while read line
	do
		set -- $line
		echo
		{
			println "$1"; shift
			for dep
			do
				println " $dep"
			done \
			| LC_COLLATE=C sort
		} | sed '$ !s/$/ \\/'
	done
}
