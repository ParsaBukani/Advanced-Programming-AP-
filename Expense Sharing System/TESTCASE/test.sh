test_directory=$(pwd)/TESTCASE

echo "Input the number of test cases, please:"
read k

names_file="$test_directory/names.csv"
for ((i = 1; i <= k; i++))
do
	spendings_file="$test_directory/spendings/$i.csv"
	output_file="$test_directory/output/$i.csv"	
	
	g++ --std=c++20 A3-810102460.cpp -o A3.out
	time ./A3.out $names_file $spendings_file > output.csv
	diff -bBq output.csv "$output_file"
	
	if [ $? == 0 ]
	then 
		echo "Test $i Accepted!"
	else
        	echo "Test $i Failed!"
        echo
        echo
        echo "Test: $i"
        echo 
        cat  "$spendings_file"
	echo 
        echo "Output:"
        echo 
        cat output.csv
	echo
        echo "Answer:"
        echo
        cat "$output_file"
	echo 
fi
	
rm output.csv
done    
	
	
