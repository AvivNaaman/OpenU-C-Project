#!/usr/bin/env bash

# File extension to concatenate
declare -a file_extensions=("ob" "ext" "ent")
prefix_of_extension="expected"

read -p "Enter file name, extensionless: " file_prefix

for i in "${file_extensions[@]}"
do
  diff "$file_prefix.$i" "$file_prefix.$prefix_of_extension.$i"
done
