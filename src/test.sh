#!/bin/sh

#test.sh -- batch testing for programs
#usage: ./test.sh <dir_of_program> <program_name>

dir=$1
program=$2
test_dir=tests

[ -z "$dir" ] && { echo "invalid args"; exit 1; }
[ -z "$program" ] && program=$dir

cd $dir

out=$(mktemp)
yes=0
no=0
#cmd="valgrind --leak-check=yes "
cmd=""
corr=""
incorr=""
for test_case in ./$test_dir/*.in; do
	echo "--- in $test_case ---"

	expected=$(echo $test_case | rev | cut -f1 -d. --complement | rev).res

	echo "program output:"
	{ $cmd ./$(basename $program) < $test_case; } &> $out
	cat -- $out

	if [[ -z "$(diff $out $expected)" ]]; then
        yes=$((yes+1))
        corr+="$(basename $test_case) "
    else
        no=$((no+1))
        incorr+="$(basename $test_case) "
    fi

	echo "expected output:"
	cat $expected
    #echo
done

echo
echo "--- end of tests ---"
echo

echo "correct results ($yes): $corr"
echo "incorrect results ($no): $incorr"
