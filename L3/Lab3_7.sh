
echo "Enter the filename:"
read filename

sed -i '2~2d' "$filename"

echo "Done."