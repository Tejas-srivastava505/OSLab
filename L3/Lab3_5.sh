date
echo "Enter the file extension (e.g., .text):"
read ext

echo "Enter the destination folder name:"
read folder

if [[ ! -d "$folder" ]]; then
    mkdir -p "$folder"
fi

find . -maxdepth 1 -type f -name "*$ext" -exec cp {} "$folder/" \;

echo "All files with $ext have been copied to $folder."