# face-landmark-detection-using-webcamera

# Installation
After you download it, type:

cd webcamera_face_landmark_detection

mkdir build

cd build

cmake ..

make

cd ..

to build this project

# Or bash install

Type:

bash install.sh

# Running
To detect one face landmark points by using webcamera, type:

./build/face camera ./model/helen.model ./model/haarcascade_frontalface_alt.xml 1

in terminal under the prject folder

If you want to detect more than one face, just change the last number to a bigger value.
