echo "This is question 6"
date
for file in *; do

    if [[ -f "$file" ]]; then

        sed -i -e 's/^ex:/Example:/g' -e 's/\.Example:/.Example:/g' "$file"
    fi
done

echo "Replacement complete."