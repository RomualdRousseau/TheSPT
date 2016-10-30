include<global.scad>

use <hardware.scad>;
use <electronic.scad>;

module pcb()
{
    w = width - 1.5;
    h = depth / 2;
    color("green") difference()
    {
        cube([w, h, 0.2]);
        translate([0, 0, -epsilon])
        {
            translate([0.5, 0.5, 0]) cylinder(d = 0.3, h = 0.2 + epsilon2);
            translate([w - 5.5, 0.5, 0]) cylinder(d = 0.3, h = 0.2 + epsilon2);
            translate([0.5, h - 0.5, 0]) cylinder(d = 0.3, h = 0.2 + epsilon2);
            translate([w - 1, h - 0.5, 0]) cylinder(d = 0.3, h = 0.2 + epsilon2);
        }
    }
    translate([0, 0, h - thickness - 0.5])
    {
        translate([0.5, 0.5, 0])
        {
            screw(thickness + 1, 0.3);
            washer(0.3);
            translate([0, 0, -thickness - 0.05]) nut(h - 2 * thickness - 0.75, 0.3);
        }
        translate([w - 5.5, 0.5, 0])
        {
            screw(thickness + 2.1, 0.3);
            washer(0.3);
            translate([0, 0, -thickness - 0.1]) nut(h - 2 * thickness - 0.75, 0.3);
        }
        translate([0.5, h - 0.5, 0])
        {
            screw(thickness + 2.1, 0.3);
            washer(0.3);
            translate([0, 0, -thickness - 0.1]) nut(h - 2 * thickness - 0.75, 0.3);
        }
        translate([w - 1, h - 0.5, 0])
        {
            screw(thickness + 2.1, 0.3);
            washer(0.3);
            translate([0, 0, -thickness - 0.1]) nut(h - 2 * thickness - 0.75, 0.3);
        }
    }
    translate([0, 0, 0.2])
    {
        // ft232rl
        translate([w - 1.5, 2.1, 1.1]) rotate([180, 0, 180]) ft232rl();
        translate([w - 3.35, 0.75, 0]) cube([3, 0.3, 0.8]);
        translate([w - 3.35, 3.15, 0]) cube([3, 0.3, 0.8]);
        // yl30sd
        translate([8.8, 5.4, 0]) cube([2.1, 0.3, 0.8]);
        translate([9.85, 7.5, 1.1]) rotate([0, 180, 90]) yl30sd();
        // 7805, 7803
        translate([0.4, 3.2, 0]) cube([1, 3, 2.1]);
        // analog right
        translate([2.3, 3.7, 0]) cube([1, 0.8, 2]);
        // analog left
        translate([w - 2, 3.7, 0]) cube([1, 0.8, 2]);
        // digital
        translate([w - 5.5, 3.8, 0]) cube([1.8, 0.4, 2]);
        // I2C 3.3V 
        translate([w - 4.4, 4.5, 0]) cube([0.5, 1, 2]);
        // I2C 5V
        translate([w - 6.5, 4.5, 0]) cube([0.5, 1, 2]);
        // analog middle
        translate([6.9, 5.1, 0]) cube([1, 1, 2]);
    }
}

pcb();
