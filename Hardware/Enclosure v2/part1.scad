include <global.scad>;

module part1()
{
    color("SaddleBrown") difference()
    {
        cube(size = [width, depth - 1, thickness]);
        // cut joystick right
        translate([width * 3 / 4, depth * 2 / 3 + 0.45, -epsilon])
        {
            cylinder(d = 4.9, h = thickness + epsilon2);     
            translate([-1.1, -2.65, 0]) cylinder(d = 0.15, h = thickness + epsilon2);
            translate([1.1, 2.65, 0]) cylinder(d = 0.15, h = thickness + epsilon2);
            translate([1.1, -2.65, 0]) cylinder(d = 0.15, h = thickness + epsilon2);
            translate([-1.1, 2.65, 0]) cylinder(d = 0.15, h = thickness + epsilon2); 
        }
        // cut trim y right
        translate([width * 3 / 4 - 0.5, depth * 2 / 3 - 3.25 , -epsilon])
        {
            cube(size = [1, 0.5, thickness + epsilon2]);
            translate([-0.65, 0.25, 0]) cylinder(d = 0.15, h = thickness + epsilon2);
            translate([1.65, 0.25, 0]) cylinder(d = 0.15, h = thickness + epsilon2);
        }
        // cut joystick left
        translate([width / 4, depth * 2 / 3 + 0.45, -epsilon])
        {
            cylinder(d = 4.9, h = thickness + epsilon2);     
            translate([-1.1, -2.65, 0]) cylinder(d = 0.15, h = thickness + epsilon2);
            translate([1.1, 2.65, 0]) cylinder(d = 0.15, h = thickness + epsilon2);
            translate([1.1, -2.65, 0]) cylinder(d = 0.15, h = thickness + epsilon2);
            translate([-1.1, 2.65, 0]) cylinder(d = 0.15, h = thickness + epsilon2); 
        }
        // cut trim y left
        translate([width / 4 - 0.5, depth * 2 / 3 - 3.25 , -epsilon])
        {
            cube(size = [1, 0.5, thickness + epsilon2]);
            translate([-0.65, 0.25, 0]) cylinder(d = 0.15, h = thickness + epsilon2);
            translate([1.65, 0.25, 0]) cylinder(d = 0.15, h = thickness + epsilon2);
        }
        // cut main switch
        translate([width / 2, depth * 2 / 3 - 2, -epsilon]) cylinder(d = 0.5, h = thickness + epsilon2);
        // cut power led
        translate([width / 2, depth * 2 / 3, -epsilon]) cylinder(d = 0.3, h = thickness + epsilon2);  
        // cut pcb
        translate([1, 3, -epsilon])
        {
            translate([0.5, 0, 0]) cylinder(d = 0.3, h = thickness + epsilon2);
            translate([width - 7, 0, 0]) cylinder(d = 0.3, h = thickness + epsilon2);
            translate([0.5, depth / 2 - 1, 0]) cylinder(d = 0.3, h = thickness + epsilon2);
            translate([width - 2.5, depth / 2 - 1, 0]) cylinder(d = 0.3, h = thickness + epsilon2);
        }
        // cut lcd
        translate([3.55, 4.2, -epsilon])
        {
            translate([0.45, 0.6, 0]) cube(size = [7.1, 2.4, thickness + epsilon2]);
            translate([0.3, 0.3, 0]) cylinder(d = 0.3, h = thickness + epsilon2);
            translate([7.7, 0.3, 0]) cylinder(d = 0.3, h = thickness + epsilon2);
            translate([7.7, 3.3, -0]) cylinder(d = 0.3, h = thickness + epsilon2);
            translate([0.3, 3.3, 0]) cylinder(d = 0.3, h = thickness + epsilon2);
        }
        // cut setup switch
        translate([12, 6, -epsilon]) cylinder(d = 0.5, h = thickness + epsilon2);
    }
}

module part1_skeleton()
{
    color("SaddleBrown") translate([0.5, depth - 1.5, -0.5])
    {
        cube(size = [width - 1, 0.5, 0.5]);
    }
}

module part1_2d()
{
    projection()
    {
        part1();
        part1_skeleton();
    }
}

part1();
part1_skeleton();