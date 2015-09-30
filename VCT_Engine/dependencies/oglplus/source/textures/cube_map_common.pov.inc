// Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#declare CameraAngle = 90;

#declare CameraDirections = array[6];
#declare CameraRightSides = array[6];
#declare CameraUpSides    = array[6];

#declare CameraDirections[0] = +x;
#declare CameraRightSides[0] = +x;
#declare CameraUpSides   [0] = +y;

#declare CameraDirections[1] = -x;
#declare CameraRightSides[1] = +x;
#declare CameraUpSides   [1] = +y;

#declare CameraDirections[2] = +y;
#declare CameraRightSides[2] = -x;
#declare CameraUpSides   [2] = -y;

#declare CameraDirections[3] = -y;
#declare CameraRightSides[3] = -x;
#declare CameraUpSides   [3] = -y;

#declare CameraDirections[4] = -z;
#declare CameraRightSides[4] = +x;
#declare CameraUpSides   [4] = +y;

#declare CameraDirections[5] = +z;
#declare CameraRightSides[5] = +x;
#declare CameraUpSides   [5] = +y;

camera {
	location CameraPosition
	right CameraRightSides[CubeMapFace]
	up CameraUpSides[CubeMapFace]
	angle CameraAngle
	look_at CameraPosition+CameraDirections[CubeMapFace]
}

