echo "Enter the folder name: "
read folder

echo "Enter the key to search: "
read key

grep -rls "$key" "$folder"

