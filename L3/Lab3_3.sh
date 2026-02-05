echo "This is question 3"
date
# 1. Find all .txt files recursively
# 2. Pipe the list of files into a while loop
find . -type f -name "*.txt" | while read file; do
    # Rename each file by replacing .txt with .text
    mv "$file" "${file%.txt}.text"
done

echo "Done!"