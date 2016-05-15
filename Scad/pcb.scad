include<global.scad>

use <hardware.scad>;
use <electronic.scad>;

module pcb()
{
    color("green") difference()
    {
        cube([width - 1.5, depth / 2, 0.2]);
        translate([0, 0, -epsilon])
        {
            translate([0.5, 0.5, 0]) cylinder(d = 0.3, h = 0.2 + epsilon2);
            translate([width - 7, 0.5, 0]) cylinder(d = 0.3, h = 0.2 + epsilon2);
            translate([0.5, depth / 2 - 0.5, 0]) cylinder(d = 0.3, h = 0.2 + epsilon2);
            translate([width - 2.5, depth / 2 - 0.5, 0]) cylinder(d = 0.3, h = 0.2 + epsilon2);
        }
    }
    translate([0, 0, height - thickness - 0.5])
    {
        translate([0.5, 0.5, 0])
        {
            screw(thickness + 1, 0.3);
            washer(0.3);
            translate([0, 0, -thickness - 0.05]) nut(height - 2 * thickness - 0.75, 0.3);
        }
        translate([width - 7, 0.5, 0])
        {
            screw(thickness + 2.1, 0.3);
            washer(0.3);
            translate([0, 0, -thickness - 0.1]) nut(height - 2 * thickness - 0.75, 0.3);
        }
        translate([0.5, depth / 2 - 0.5, 0])
        {
            screw(thickness + 2.1, 0.3);
            washer(0.3);
            translate([0, 0, -thickness - 0.1]) nut(height - 2 * thickness - 0.75, 0.3);
        }
        translate([width - 2.5, depth / 2 - 0.5, 0])
        {
            screw(thickness + 2.1, 0.3);
            washer(0.3);
            translate([0, 0, -thickness - 0.1]) nut(height - 2 * thickness - 0.75, 0.3);
        }
    }
    translate([width - 3, 2.1, 1.3]) rotate([180, 0, 180]) ft232rl();
}

pcb();
