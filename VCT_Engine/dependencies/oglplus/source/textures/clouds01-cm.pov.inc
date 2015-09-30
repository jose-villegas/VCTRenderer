// Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// Output_Alpha=On

#declare WorldRadius = 6371000;
#declare AtmThickness = 50000;

#declare CameraPosition = <0.00, 10.00, 0.00>;
#include "cube_map_common.pov.inc"

global_settings {
	assumed_gamma 1.0
}

background { rgbt <0.5, 0.5, 0.5, 0.0> }

#macro CloudColor(Altitude, LayerPart, Density)
		function {
			min(max(sqrt(
				pow(x/(AtmThickness), 2)+
				pow(y/(AtmThickness), 2)+
				pow(z/(AtmThickness), 2)
			)-1.0, 0.0), 1.0)
		}
		color_map {
			[0.00 color rgbt <0.0, min(Altitude / AtmThickness, 1), min(LayerPart, 1), max(min(Density, 1), 0)>]
			[1.00 color rgbt <1.0, min(Altitude / AtmThickness, 1), min(LayerPart, 1), max(min(Density, 1), 0)>]
		}
#end

#macro CloudLayer(Altitude, Scale, Coverage, Turbulence, Octaves, Omega, BorderTransparency, CenterTransparency)
	union {
	#declare LayerMax = 30;
	#declare LayerThickness = 15;
	#declare Layer = 0;
	#while(Layer <= LayerMax)
		#declare LayerPart = Layer / LayerMax;
		difference {
			sphere {
				<0, -WorldRadius, 0>, WorldRadius+Altitude+LayerThickness*(Layer+0.45)
			}
			sphere {
				<0, -WorldRadius, 0>, WorldRadius+Altitude+LayerThickness*Layer
			}
			texture {
				pigment {
					pigment_pattern {
						bozo
							turbulence 0.6 + Altitude/6000
							octaves 11 + Altitude / 10000
							omega 0.60 + Altitude / 200000
							lambda 2
						pigment_map {
							[0.00	rgb 0.0]
							[0.20	agate
								turbulence Turbulence-0.1+0.01*LayerPart
								octaves Octaves+LayerPart
								omega Omega-0.1+0.1*LayerPart
								lambda 2
								color_map {
									[0.00 rgb 0.0]
									[0.80 rgb 0.1]
									[1.00 rgb 0.2]
								}
							]
							[0.35	bozo
								turbulence Turbulence+0.01*LayerPart*Altitude/7000
								octaves Octaves+LayerPart*Altitude/7000
								omega Omega+0.04*LayerPart
								lambda 3
								color_map {
									[0.00 rgb 0.0]
									[0.50 rgb 0.2]
									[1.00 rgb 0.5]
								}
							]
							[0.45	bozo
								turbulence Turbulence+0.02*LayerPart*Altitude/7000
								octaves Octaves+1+LayerPart*Altitude/3000
								omega Omega+0.05+0.05*LayerPart
								lambda 4
								color_map {
									[0.00 rgb 0.3]
									[0.80 rgb 0.6]
									[1.00 rgb 0.8]
								}
							]
							[0.55	rgb 1.0]
							[1.00	rgb 1.0]
						}
						scale <0.5, 0.3, 0.7> * WorldRadius * 0.003 * Altitude/3000 * Scale
						translate <Altitude, 0.0, Altitude>
						rotate<0, Altitude / 100 , 0>
					}
					pigment_map {
						[0.00                          CloudColor(Altitude, LayerPart, CenterTransparency)]
						[Coverage-0.05-0.18*LayerPart  CloudColor(Altitude, LayerPart, BorderTransparency-0.01*LayerPart)]
						[Coverage+0.05+0.08*LayerPart  CloudColor(Altitude, LayerPart, 1.00)]
						[1.00                          CloudColor(Altitude, LayerPart, 1.00)]
					}
				}
				finish {ambient 1 diffuse 0}
			}
		}
		#declare Layer = Layer + 1;
	#end
	}
#end

CloudLayer(25000, 29, 0.10, 0.90, 11, 0.7, 0.70, 0.40)
CloudLayer(10000, 19, 0.25, 0.80, 11, 0.5, 0.45, 0.15)
CloudLayer(3000, 2, 0.35, 0.95, 12, 0.6, 0.35, 0.10)

plane {
	y,0
	texture {
		pigment
		{
			function { min(5 * sqrt(pow(x / AtmThickness, 2) + pow(z / AtmThickness, 2)), 1) }
			color_map {
				[0.00 color rgb <0,0,0>]
				[1.00 color rgb <1,0,0>]
			}
		}
		finish {ambient 1 diffuse 0}
	}
}
