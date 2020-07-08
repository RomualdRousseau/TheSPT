include <global.scad>;

use <side_shape.scad>;

module side_l()
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
            // hole switch
            translate([depth + 2 * cos(35), 1.5 + 2 * sin(35), 0]) cylinder(d = 0.5, h = thickness + epsilon2);
        }
    }
}

module side_l_skeleton()
{
    color("SaddleBrown") translate([0, 0, thickness]) difference()
    {
        linear_extrude(height = 0.5, convexity = 10)
        {
            offset(r = -thickness, delta = -thickness) side_shape();
        }
        translate([0, 0, -epsilon]) linear_extrude(height = 0.5 + epsilon2, convexity = 10)
        {
            offset(r = -thickness - 0.5, delta = -thickness) side_shape();
        }
        translate([1, thickness - epsilon, -epsilon]) cube([depth - 1, 1 + epsilon2, 0.5 + epsilon2]);
    }
}

module side_l_2d()
{
    projection()
    {
        side_l();
        side_l_skeleton();
    }
}

side_l();
side_l_skeleton();