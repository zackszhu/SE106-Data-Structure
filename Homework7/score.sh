score=0

if (./rpn < ./test_case/test_case_1.txt | grep "1 2 + 3 +")
then
	echo "test 1 pass, +10 point"
	score=`expr 10 + $score`
else
	echo "test 1 error"
fi

if (./rpn < ./test_case/test_case_2.txt | grep "2 x \* 3 y \* + 10 /")
then
	echo "test 2 pass, +10 point"
	score=`expr 10 + $score`
else
	echo "test 2 error"
fi

if (./rpn < ./test_case/test_case_3.txt | grep "1 2 + 3 / 4 5 \* +")
then
	echo "test 3 pass, +10 point"
	score=`expr 10 + $score`
else
	echo "test 3 error"
fi

echo " "
echo "Score: $score/30"
