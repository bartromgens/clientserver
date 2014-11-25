find -name 'Makefile' -delete
find . -depth -name 'tmp' -exec rm -rf '{}' \;
find . -depth -name 'generated' -exec rm -rf '{}' \;
rm -rf ./lib