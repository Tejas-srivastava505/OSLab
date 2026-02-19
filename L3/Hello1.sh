# echo "Enter two numbers"
read a b
echo a is $a and b is $b
c=$((a/b))
echo $c
c=`expr $a \* $b`
echo $c
c=$(echo $a/$b | bc -l)
echo $c
c=`bc -l <<< $a/$b`
echo $c
c=$(bc -l <<< $a/$b)
echo $c

# # #Control statements
if (($a > $b)); then
	echo $a is greater than $b
#}
fi

# # #Control statements
if (($a > $b)); then
	echo $a is greater than $b
else
    echo $b is greater than $a
fi

# # read c

# # #Control statements
if (( a > b && a > c )); then
    echo "$a is the greatest"
elif (( b > a && b > c )); then
    echo "$b is the greatest"
elif (( c > a && c > b )); then
    echo "$c is the greatest"
else
    echo "Two or more numbers are equal and greatest"
fi

#Switch case
read x
case $x in
1 ) echo "You entered one.";; 
2|3 ) echo "You entered two.";;
3 ) echo "You entered three.";; 
"Hello" ) echo -n “You entered two.” echo “Just to show multiple com- mands”;;
'A' ) echo "You entered three.";;
* ) echo "You did not enter a number between 1 and 3."
esac

#For loop
read N
for (( i=1; i<=N; i++ ))
do
	echo "Number $i"
done
IFS=$'\n'
Files=`ls`
for i in (5 6 7 $Files
do
	echo "File $i"
done

until
echo "Enter N:"
read N
i=0
until (( i >= N ))
do
    ((i++))
    echo "Number $i"
done