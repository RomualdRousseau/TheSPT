include <global.scad>;

use <hardware.scad>;

module bottom_door()
{
    color("white") difference()
    {
        cube([width, depth - 1, thickness]);
        translate([0, 0, -epsilon])
        {
            translate([0.5, 1, 0]) cylinder(d = 0.3, h = thickness + epsilon2);
            translate([0.5, depth - 2, 0]) cylinder(d = 0.3, h = thickness + epsilon2);
            translate([width - 0.5, 1, 0]) cylinder(d = 0.3, h = thickness + epsilon2);
            translate([width - 0.5, depth - 2, 0]) cylinder(d = 0.3, h = thickness + epsilon2);
        }
    }
}

module bottom_door_skeleton()
{
    color("SaddleBrown") translate([0, 0, thickness]) difference()
    {
        union()
        {
            cube(size = [1, depth - 1, 0.5]);
            translate([width - 1, 0, 0]) cube(size = [1, depth - 1, 0.5]);
            translate([1, depth - 1.5, 0]) cube(size = [width - 2, 0.5, 0.5]);
        }
        translate([0, 0, -epsilon])
        {
            translate([0.5, 1, 0]) cylinder(d = 0.5, h = 0.5 + epsilon2);
            translate([0.5, depth - 2, 0]) cylinder(d = 0.5, h = 0.5 + epsilon2);
            translate([width - 0.5, 1, 0]) cylinder(d = 0.5, h = 0.5 + epsilon2);
            translate([width - 0.5, depth - 2, 0]) cylinder(d = 0.5, h = 0.5 + epsilon2);
        }
    }
}

module bottom_door_feet()
{
    translate([0.5, 1, 0]) foot();
    translate([0.5, depth - 2, 0]) foot();
    translate([width - 0.5, 1, 0]) foot();
    translate([width - 0.5, depth - 2, 0]) foot();
    translate([0.5, 1, -0.2]) rotate([180, 0, 0])
    {
        screw(thickness + 0.7, 0.3);
        translate([0, ,0, -thickness - 0.2]) nut(0.5, 0.3);
    }
    translate([0.5, depth - 2, -0.2]) rotate([180, 0, 0])
    {
        screw(thickness + 0.7, 0.3);
        translate([0, ,0, -thickness - 0.2]) nut(0.5, 0.3);
    }
    translate([width - 0.5, 1, -0.2]) rotate([180, 0, 0])
    {
        screw(thickness + 0.7, 0.3);
        translate([0, ,0, -thickness - 0.2]) nut(0.5, 0.3);
    }
    translate([width - 0.5, depth - 2, -0.2]) rotate([180, 0, 0])
    {
        screw(thickness + 0.7, 0.3);
        translate([0, ,0, -thickness - 0.2]) nut(0.5, 0.3);
    }
}

module bottom_door_2d()
{
    projection()
    {
        bottom_door();
        bottom_door_skeleton();
    }
}

bottom_door();
bottom_door_skeleton();
bottom_door_feet();