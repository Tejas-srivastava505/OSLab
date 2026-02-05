date
echo "This is the solution for the 2nd Question"
echo "Enter the path"
read Path 
echo "Enter the String to find"
read word

grep -rls "$word" "$Path"

