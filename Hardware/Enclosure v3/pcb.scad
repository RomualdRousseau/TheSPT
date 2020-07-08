include<global.scad>

use <hardware.scad>;
use <electronic.scad>;

module pcb(thickness = 0.3)
{
    w = 18.0;
    h = 7.5;
    color("green") difference()
    {
        cube([w, h, 0.2]);
        translate([0, 0, -epsilon])
        {
            translate([0.3, 0.5, 0]) cylinder(d = 0.3, h = 0.2 + epsilon2);
            translate([w - 5.2, 0.5, 0]) cylinder(d = 0.3, h = 0.2 + epsilon2);
            translate([0.3, h - 0.5, 0]) cylinder(d = 0.3, h = 0.2 + epsilon2);
            translate([w - 0.5, h - 0.5, 0]) cylinder(d = 0.3, h = 0.2 + epsilon2);
        }
    }
    translate([0, 0, height - thickness - 0.5])
    {
        translate([0.3, 0.5, -0.05])
        {
            screw(thickness + 1, 0.3);
            washer(0.3);
            translate([0, 0, -thickness - 0.05]) nut(height - 2 * thickness - 0.75, 0.3);
        }
        translate([w - 5.2, 0.5, -0.05])
        {
            screw(thickness + 2.1, 0.3);
            washer(0.3);
            translate([0, 0, -thickness - 0.05]) nut(height - 2 * thickness - 0.75, 0.3);
        }
        translate([0.3, h - 0.5, -0.05])
        {
            screw(thickness + 2.1, 0.3);
            washer(0.3);
            translate([0, 0, -thickness - 0.05]) nut(height - 2 * thickness - 0.75, 0.3);
        }
        translate([w - 0.5, h - 0.5, -0.05])
        {
            screw(thickness + 2.1, 0.3);
            washer(0.3);
            translate([0, 0, -thickness - 0.05]) nut(height - 2 * thickness - 0.75, 0.3);
        }
    }
    translate([0, 0, 0.2])
    {
        // ft232rl
        translate([w - 3.35, 0.75, 0]) cube([3, 0.3, 0.8]);
        translate([w - 3.35, 3.15, 0]) cube([3, 0.3, 0.8]);
        translate([w - 1.5, 2.1, 1.1]) rotate([180, 0, 180]) ft232rl();
        // yl30sd
        //translate([7.7, 5, 0]) cube([2.4, 0.3, 0.8]);
        translate([9, 7.1, 0.3]) rotate([0, 180, 90]) yl30sd();
        // 7805, 7803
        translate([0.4, 3.2, 0]) cube([1, 3, 2.1]);
        // 7 analog
        translate([5.95, h - 2.35, 0]) jumper(7);
        translate([5.95, h - 2.05, 0]) jumper(7);
        translate([5.95, h - 1.75, 0]) jumper(7);
        // 15 digital
        translate([13.05, h - 2.35, 0]) jumper(15);
        translate([13.05, h - 2.05, 0]) jumper(15);
        // I2C 3.3V
        translate([13.1, 1.80, 0]) jumper(2);
        translate([13.1, 2.10, 0]) jumper(2); 
        translate([13.1, 2.40, 0]) jumper(2); 
        // I2C 5V
        translate([11, 1.80, 0]) jumper(2);
        translate([11, 2.10, 0]) jumper(2);
        translate([11, 2.40, 0]) jumper(2);
        // ct8z
        translate([3.6, 2.3, 0]) jumper(3);
        // battery
        translate([1.4, 0.4, 0]) cube([1, 0.7, 1]);
        // switch
        translate([2.9, 0.4, 0]) cube([0.8, 0.4, 0.6]);
        // led
        translate([3.9, 0.4, 0]) cube([0.8, 0.4, 0.6]);
    }
}

pcb();
