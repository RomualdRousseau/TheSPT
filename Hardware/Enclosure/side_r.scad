include <global.scad>;

use <side_shape.scad>;

module side_r()
{
    color("SaddleBrown") difference()
    {
        linear_extrude(height = thickness, convexity = 10) side_shape();
        translate([0, 0, -epsilon])
        {
            // hole trim
            translate([depth - 1.5, thickness + 1, 0])
            {
                translate([-0.5, -0.25, 0]) cube(size = [1, 0.5, thickness + epsilon2]);
                translate([-1.15, 0, 0]) cylinder(d = 0.15, h = thickness + epsilon2);
                translate([1.15, 0, 0]) cylinder(d = 0.15, h = thickness + epsilon2);
            }
            // hole I2C port
            translate([8.5, thickness + 1.3, 0])
            {
                translate([-1, -0.55, 0]) cube(size = [2, 1.1, thickness + epsilon2]);
                translate([-1.3, 0, 0]) cylinder(d = 0.3, h = thickness + epsilon2);
                translate([1.3, 0, 0]) cylinder(d = 0.3, h = thickness + epsilon2);
            }
            // hole USB Serial port
            translate([5, thickness + 0.75, 0]) cube(size = [1.2, 1.1, thickness + epsilon2]);
            // hole Switch
            translate([depth + 2 * cos(35), 2 * sin(35) + 1.5, 0]) cylinder(d = 0.5, h = thickness + epsilon2);
        }
    }
    
}

module side_r_skeleton()
{
    color("SaddleBrown") translate([0, 0, -0.5]) difference()
    {
        linear_extrude(height = 0.5, convexity = 10)
        {
            offset(r = -thickness, delta = -thickness) side_shape();
        }
        translate([0, 0, -epsilon])
        {
            linear_extrude(height = 0.5 + epsilon2, convexity = 10)
            {
                offset(r = -thickness - 0.5, delta = -thickness) side_shape();
            }
            translate([1, thickness - epsilon, 0]) cube([depth - 1, 1 + epsilon2, 0.5 + epsilon2]);
        }
    }
}

module side_r_2d()
{
    projection()
    {
        side_r();
        side_r_skeleton();
    }
}

side_r();
side_r_skeleton();