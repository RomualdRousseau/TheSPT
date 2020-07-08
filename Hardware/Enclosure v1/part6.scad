include <global.scad>;

module part6()
{
    h = height + 5 * sin(30) - 8 * sin(35);
    l = 8 * cos(35) - 5 * cos(30);
    d = sqrt(h * h + l * l) - 2 * thickness;
    color("SaddleBrown") difference()
    {
        cube(size = [width, d, thickness]);
        // antenna hole
        translate([3, d / 2, -epsilon]) cylinder(d = 0.6, h = thickness + epsilon2);
    }
}

module part6_skeleton()
{
    h = height + 5 * sin(30) - 8 * sin(35);
    l = 8 * cos(35) - 5 * cos(30);
    d = sqrt(h * h + l * l) - 2 * thickness;   
    color("SaddleBrown")
    {
        translate([0.5, 0, -0.5]) cube(size = [width - 1, 0.5, 0.5]);
        translate([0.5, d - 0.5, -0.5]) cube(size = [width - 1, 0.5, 0.5]);
    }
}

module part6_2d()
{
    projection()
    {
        part6();
        part6_skeleton();
    }
}

part6();
part6_skeleton();