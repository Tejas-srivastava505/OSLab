date
echo "This is question 1"

echo "Enter name:"
read item

if [ -d "$item" ]; then
    echo "$item is a directory."
elif [ -f "$item" ]; then
    echo "$item is a file."
else
    echo "$item is neither."
fi