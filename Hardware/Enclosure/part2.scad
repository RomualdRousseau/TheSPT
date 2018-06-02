include <global.scad>;

module part2()
{
    color("SaddleBrown") difference()
    {
        cube(size = [width, 5, thickness]);
        // hole switch
        translate([width - 3, 2.5, -epsilon]) cylinder(d = 0.5, h = thickness + epsilon2);
    }
}

module part2_2d()
{
    projection() part2();
}

part2();