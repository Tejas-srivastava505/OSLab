date
echo "Enter the basic and TA"
read basic TA

gs=$(echo "$basic + $TA + (0.1 * $basic)" |  bc -l)

echo "The Gross Salary is: $gs"