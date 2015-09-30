// Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "functions.inc"

#declare WorldRadius = 10000;
#declare SunPosition = <-15000, 25000, -25000>;

#declare CameraPosition = <0.00, 10.00, 0.00>;
#include "cube_map_common.pov.inc"

global_settings {
	assumed_gamma 1.0
}

light_source {
	SunPosition
	color rgb <1.0, 1.0, 1.0> * 2
}

sky_sphere {
	pigment {
		gradient +y
		color_map {
			[0.00 rgbt <1.0, 1.0, 0.0, 0.0> * 0.8]
			[0.10 rgbt <0.1, 0.3, 0.7, 0.0> * 0.6]
			[1.00 rgbt <0.1, 0.2, 0.4, 0.0> * 0.4]
		}
		scale 1.1
	}
}

plane {
	<0,1,0>,2
	texture {
		pigment {
			bozo turbulence 0.98
			octaves 12  omega 0.7 lambda 3
			color_map {
				[0.0 color rgbt <0.80, 0.80, 0.80, 0.00>]
				[0.8 color rgbt <0.80, 0.80, 0.80, 0.30>]
				[0.9 color rgbt <0.85, 0.85, 0.85, 1.00>]
				[1.0 color rgbt <0.90, 0.90, 0.90, 1.00>]
			}
			rotate<20,20,0> translate<0,0,-5.1>
			scale <0.9, 0.3, 0.4>*7
		}
		finish {ambient 1 diffuse 0}
	}
	hollow
	scale 1800
}

plane {
	<0,1,0>,2
	texture {
		pigment {
			bozo turbulence 0.85
			octaves 14  omega 0.6 lambda 4
			color_map {
				[0.0 color rgbt <0.15, 0.15, 0.15, 0.00>]
				[0.7 color rgbt <0.60, 0.60, 0.60, 0.00>]
				[0.8 color rgbt <0.80, 0.80, 0.80, 1.00>]
				[1.0 color rgbt <0.90, 0.90, 0.90, 1.00>]
			}
			rotate<20,20,0> translate<0,0,-0.4>
			scale <0.3, 0.4, 0.4>*8
		}
		finish {ambient 1 diffuse 0}
	}
	hollow
	scale 1400
}

plane {
	<0,1,0>,2
	texture {
		pigment {
			bozo turbulence 0.75
			octaves 14  omega 0.7 lambda 2
			color_map {
				[0.0 color rgbt <0.05, 0.05, 0.05, 0.00>]
				[0.2 color rgbt <0.35, 0.35, 0.34, 0.00>]
				[0.4 color rgbt <0.50, 0.50, 0.50, 0.10>]
				[0.5 color rgbt <0.65, 0.65, 0.65, 1.00>]
				[1.0 color rgbt <0.85, 0.85, 0.80, 1.00>]
			}
			rotate<40,10,0> translate<0,0,+1.6>
			scale <0.5, 0.5, 0.3>*6
		}
		finish {ambient 1 diffuse 0}
	}
	hollow
	scale 1000
}


isosurface {
	function {
		- f_noise3d(x*0.0004, 0, z*0.0004)*(pow(x/WorldRadius,2) + pow(0.5*z/WorldRadius,2))*3000.0
		+ f_noise3d(x*0.0001, 0, z*0.0001)*sqrt(pow(2*x/WorldRadius,2) + pow(2*z/WorldRadius,2))*500.0
		- f_noise3d(x*0.001, 0, z*0.001)*sqrt(pow(x/WorldRadius,2) + pow(z/WorldRadius,2))*100.0
		- f_noise3d(x*0.01, 0, z*0.01)*sqrt(pow(4*x/WorldRadius,2) + pow(4*z/WorldRadius,2))*100.0
		+ f_noise3d(x*0.1, 0, z*0.1)*5.0
		+ f_noise3d(x, 0, z)*0.5
		+ f_noise3d(x*10, 0, z*10)*0.1
		+ y
	}
	contained_by {sphere {CameraPosition, WorldRadius}}
	max_gradient 5.0
	texture {
		pigment {
			bozo turbulence 0.75
			octaves 18  omega 0.8 lambda 2
			color_map {
				[0.0 color rgb <0.25, 0.15, 0.05>]
				[0.2 color rgb <0.65, 0.35, 0.14>]
				[0.5 color rgb <0.85, 0.70, 0.45>]
				[1.0 color rgb <0.90, 0.85, 0.60>]
			}
			scale 100

		}
		normal { bumps 0.15 scale 0.025  }
		finish { ambient 0.3 phong 0.05 }
	}
}

