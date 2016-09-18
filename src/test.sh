#!/bin/sh

dir=$1

[ -z "$dir" ] && { echo "invalid args"; exit 1; }

program=$dir

cd $dir

make

out=$(mktemp)
yes=0
no=0
for test_case in tests/*.in; do
	echo "--- in $test_case ---"

	expected=$(echo $test_case | rev | cut -f1 -d. --complement | rev).res

	echo "program output:"
	{ ./$(basename $program) < $test_case; } &> $out
	cat -- $out

	[[ -z "$(diff $out $expected)" ]] && yes=$((yes+1)) || no=$((no+1))

	echo "expected output:"
	cat $expected
	echo
done

echo "correct results: $yes"
echo "incorrect results: $no"
