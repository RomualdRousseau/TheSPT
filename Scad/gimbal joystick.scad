use <hardware.scad>;

module joystick(gaz = false, with_screw = true, thickness = 0.3, angle_x = 0, angle_y = 0)
{
    // top base
    color("DarkSlateGray") translate([0, 0, 0.4]) difference()
    {
        cylinder(h = 0.8, d = 4.9, center = true);
        difference()
        {
            cylinder(h = 1, d = 4.7, center = true);
            difference()
            {
                union()
                {
                    translate([0, 2, 0]) cube(size = [4.7, 1.5, 1], center = true);
                    translate([0, -2, 0]) cube(size = [4.7, 1.5, 1], center = true);
                }
                translate([0, 0, 1]) rotate([0, 90, 0]) cylinder(h = 4, d = 3.8, center = true);
            }
            translate([0, 0, -1.6]) rotate([0, 90, 0]) cylinder(h = 4.7, d = 3.5, center = true);
        }
        translate([0, 0, -1.6]) rotate([0, 90, 0]) cylinder(h = 3.8, d = 3.505, center = true);
    }
    // bottom base
    color("DarkSlateGray") translate([0, 0, -1.7])
    {
        difference()
        {
            union()
            {
                cube(size = [4.9, 4.9, 2], center = true);
                if(!gaz)
                {
                    translate([-0.25, 0, 1.35]) cube(size = [5.4, 5.9, 0.7], center = true);
                    translate([2.95, 0, 0]) cube(size = [1, 1.4, 2.0], center = true);
                }
                else
                {
                    translate([0.25, 0, 1.35]) cube(size = [5.4, 5.9, 0.7], center = true);
                    translate([-2.95, 0, 0]) cube(size = [1, 1.4, 2.0], center = true);
                }
            }
            translate([0, 0, 0.45]) rotate([0, 90, 0]) cylinder(h = 3.8, d = 3.6, center = true);
            translate([0.55-2.45+1.9, -0.1-2.45+2.55, 0]) cube(size = [3.8, 5.1, 2.2], center = true);
            translate([-1.1, -2.65, 1.35]) cylinder(h = 0.9, d = 0.2, center = true);
            translate([1.1, -2.65, 1.35]) cylinder(h = 0.9, d = 0.2, center = true);
            translate([-1.1, 2.65, 1.35]) cylinder(h = 0.9, d = 0.2, center = true);
            translate([1.1, 2.65, 1.35]) cylinder(h = 0.9, d = 0.2, center = true);
        }
    }
    translate([0, 0, -1.1]) rotate([angle_y, 0, 0])
    {
        // gimbal y
        color("DarkSlateGray") difference()
        {
            rotate([0, 90, 0])
            {
                cylinder(h = 3.7, d = 3.3, center = true);
                cylinder(h = 4.9, d = 0.8, center = true);
            }
            rotate([0, 90, 0]) cylinder(h = 3.4, d = 2.9, center = true);
            translate([0, 0, -3.5 / 4]) cube(size = [3.4, 3.7, 3.5 / 2], center = true);
            translate([0, 0, 1.5]) cube(size = [2.9, 0.8, 0.5], center = true);
        }
        // gimbal x    
        translate([0, 0, 0.2]) 
        {
            color("DarkSlateGray") translate([0, -1, -0.7]) cube(size = [1.4, 1, 2.0], center = true);
            color("DarkSlateGray") translate([0, 1.4, -0.7]) cube(size = [1.4, 0.3, 2.0], center = true);
            rotate([0, angle_x, 0])
            {
                color("DarkSlateGray") rotate([90, 0, 0]) 
                {
                    cylinder(h = 0.8, d = 3, center = true);
                    cylinder(h = 3, d = 0.8, center = true);
                }
                translate([0, 0, 1.3]) rotate_extrude() polygon(points = [[0, 0], [0, 3.3], [0.3, 3.5], [0.4, 3.2], [0.4, 2], [0.2, 1], [0.2, 0.6], [0.4, 0.6], [0.4, 0]]);
            }
        }
    }
    if(with_screw)
    {
        translate([-1.1, -2.65, thickness + 0.05])
        {
            screw(thickness + 1, 0.2);
            washer(0.2);
            translate([0, 0, -0.75 - thickness]) nut(0.2, 0.2);
        }
        translate([1.1, -2.65, thickness + 0.05])
        {
            screw(thickness + 1, 0.2);
            washer(0.2);
            translate([0, 0, -0.75 - thickness]) nut(0.2, 0.2);
        }
        translate([-1.1, 2.65, thickness + 0.05])
        {
            screw(thickness + 1, 0.2);
            washer(0.2);
            translate([0, 0, -0.75 - thickness]) nut(0.2, 0.2);
        }
        translate([1.1, 2.65, thickness + 0.05])
        {
            screw(thickness + 1, 0.2);
            washer(0.2);
            translate([0, 0, -0.75 - thickness]) nut(0.2, 0.2);
        }
    }
}

module test()
{
    $fn = 40;
    joystick(angle_x = 30 * cos(360 * $t), angle_y = 30 * sin(360 * $t));
}

test();





