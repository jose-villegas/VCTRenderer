To build the example do the following:

android update project --path . --target android-$N
# where N >= 18
ndk-build
ant debug

To clean the generated files do:

rm -rf bin gen obj libs
rm -f build.xml
rm -f *.properties
rm -f proguard-project.txt



