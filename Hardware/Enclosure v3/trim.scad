use <hardware.scad>;
use <electronic.scad>;

module trim(with_pcb = true, with_screw = true, thickness = 0.3, angle = 0, explode_dist = 0, solid_holes = false)
{
    color("DarkSlateGray") translate([-1.45, -0.5, 0.9 + explode_dist]) difference()
    {
        cube(size = [2.9, 1, 0.4]); 
        translate([0.5, 0.1, -0.1]) cube(size = [0.8, 0.8, 0.3]);
        translate([1.6, 0.1, -0.1]) cube(size = [0.8, 0.8, 0.3]);
        translate([0.1, 0.1, 0.1]) cube(size = [2.7, 0.8, 0.4]);
        translate([1.45, 1.1, 0.35]) rotate([90, 0, 0]) cylinder(d = 0.12, h = 1.2);
        translate([0.35, 0.5, -0.1]) cylinder(d = 0.2, h = 0.6);
        translate([2.55, 0.5, -0.1]) cylinder(d = 0.2, h = 0.6);
    }
    if(!solid_holes) color("DarkSlateGray") translate([0, 0.5, 1.25 + explode_dist]) rotate([0, angle, 0])
    {
        translate([0.5, -0.5, -0.25]) cylinder(d = 0.5, h = 0.4);
        translate([-0.5, -0.5, -0.25]) cylinder(d = 0.5, h = 0.4);
        translate([-0.5, -0.75, -0.15]) cube(size = [1, 0.5, 0.6]);
        translate([-0.35, -0.65, 0.45]) cube(size = [0.7, 0.3, 0.8]);
        translate([0, 0, 0]) rotate([90, 0, 0]) cylinder(d = 0.1, h = 1);
    }
    if(with_pcb)
    {
        translate([0.5, 0, 0]) micro_switch(length = 0.7);
        translate([-0.5, 0, 0]) micro_switch(length = 0.7);
        color("green") translate([-1.4, -0.5, -0.2]) cube(size = [2.8, 1.7, 0.2]);
    }
    if(with_screw)
    {
        translate([-1.1, 0, 0])
        {
            translate([0, 0, thickness + 1.85 + explode_dist * 2])
            {
                screw(thickness + 0.5, 0.2);
                washer(0.2);
            }
            translate([0, 0, -0.2 - explode_dist]) rotate([180, 0, 0])
            {
                screw(1.5, 0.2);
                translate([0, 0, -0.2 - explode_dist]) nut(0.9, 0.2);
                translate([0, 0, -1.2 - explode_dist * 2.5]) nut(0.8, 0.2);
            }
        }
        translate([1.1, 0, 0])
        {
            translate([0, 0, thickness + 1.85 + explode_dist * 2])
            {
                screw(thickness + 0.5, 0.2);
                washer(0.2);
            }
            translate([0, 0, -0.2 - explode_dist]) rotate([180, 0, 0])
            {
                screw(1.5, 0.2);
                translate([0, 0, -0.2 - explode_dist]) nut(0.9, 0.2);
                translate([0, 0, -1.2 - explode_dist * 2.5]) nut(0.8, 0.2);
            }
        }
    }
    if(solid_holes)
    {
        translate([0, 0.5, 1.25 + explode_dist]) translate([-0.55, -0.70, 0.45]) cube(size = [1.1, 0.4, 0.8]);
    }
}

module test()
{
    $fn = 20;
    trim(angle = 0, explode_dist = 2);
}

test();