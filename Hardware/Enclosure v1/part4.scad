include <global.scad>;

module part4()
{
    color("SaddleBrown")
    {
        cube(size = [width, height - 2, thickness]);
    }
}

module part4_skeleton()
{
    color("SaddleBrown")
    {
        translate([3, 0.1, -0.5]) cube(size = [1, height - 2.2, 0.5]);
        translate([width - 4, 0.1, -0.5]) cube(size = [1, height - 2.2, 0.5]);
    }
}

module part4_2d()
{
    projection() part4();
}

part4();
part4_skeleton();