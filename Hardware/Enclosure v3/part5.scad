include <global.scad>;

module part5()
{
    color("SaddleBrown") difference()
    {
        cube(size = [width, 1 / cos(45), thickness]);
    }
}

module part5_2d()
{
    projection() part5();
}

part5();