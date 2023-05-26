#!/bin/bash

TESTS="_tests"
PARSER="./bin/lcpl-parser"

TEST_COUNT=0
FAILED_TESTS=0

function fail {
	FAILED_TESTS=$((FAILED_TESTS+1))
	printf "FAIL (%s)\n" "$1"
}

function pass {
	printf "PASS\n"
}

test()
{
  TEST_COUNT=$((TEST_COUNT+1))
  echo === $1 ===
  FNAME=`basename $1`
  $PARSER $1 $FNAME.ast 2> $FNAME.err

   if [ -e $FNAME.ast ]; then
   	if [ -z "$( diff -w -q $FNAME.ast ${1%lcpl}ast.ref )" ]; then
		pass
   	else
		fail "Different program output. First lines of diff below"
		echo "-----"
		diff -w -q $FNAME.ast ${1%lcpl}ast.ref | head -n 30
		echo "-- Error log: --"
		cat $FNAME.err
		echo "-----"
		echo
	fi
   else
   	fail "No ast file generated"
   	echo "-- Error log: --"
	cat $FNAME.err
	echo "-----"
	echo
   fi
   rm $FNAME.err  
   rm $FNAME.ast
}

echo "******************************"
echo "**** Running simple tests ****"
echo "******************************"
echo 

for l in $TESTS/*.lcpl
do
	test $l
done
for l in $TESTS/simple/*.lcpl
do
	test $l
done

echo
echo

echo "********************************"
echo "**** Running advanced tests ****"
echo "********************************"
echo

for l in $TESTS/advanced/*.lcpl
do
	test $l
done

echo
echo

echo "*******************************"
echo "**** Running complex tests ****"
echo "*******************************"
echo
for l in $TESTS/complex/*.lcpl
do
	test $l
done

echo "$FAILED_TESTS tests out of $TEST_COUNT failed"

