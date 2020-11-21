
use <lib/objLib.scad>

$fn=60;


group(){
    difference(){
        group(){ //couvercle 
            translate([52,-1,-5]) cube([2,31,50]);
            
            translate([46,1.5,1.5]) cube([4,2,2]);
            translate([50,0,0]) cube([2,3.5,3.5]);
            
            translate([46,1.5,36.5]) cube([4,2,2]);
            translate([50,0,36.5]) cube([2,3.5,3.5]);
            
            translate([46,26.5,36.5]) cube([4,2,2]);
            translate([50,25,36.5]) cube([2,5,8.5]);
            
            translate([46,26.5,1.5]) cube([4,2,2]);
            translate([50,25,-5]) cube([2,5,8.5]);    
            
        }
        translate([55,27.5,-1.5])rotate([0,-90,0])screwHole(6);
        translate([55,27.5,41.5])rotate([0,-90,0])screwHole(6);
    }
}


*group(){ //box
    difference(){
        group(){
            cube ([50,30,40]);
            difference(){
                group(){ // protection prise
                    translate([25,25,-3])cylinder(h=3,r=5);
                    translate([14,28.5,-3]) cube([10,1.5,3]);
                    translate([14,20,-3]) cube([10,1.5,3]);            
                }
                translate([18-((sqrt(2)*10)/2),17,-(sqrt(2)*10)/2])rotate([0,45,0])cube([10,15,10]); 
                translate([8,17,-4]) cube([10,15,4]);            
            }
        }
        translate([2,1.5,1.5]) cube([50,27,37]);
        translate([-1,25,3]) cube([5,1,33]);
        translate([-1,22,3]) cube([5,1,33]);
        translate([-1,19,3]) cube([5,1,33]);
        translate([-1,16,3]) cube([5,1,33]);
        translate([-1,13,3]) cube([5,1,33]);
        translate([-1,10,3]) cube([5,1,33]);
        translate([-1,7,3]) cube([5,1,33]);
        translate([-1,2.8,3]) cube([5,1,33]);
        
        //trou prise
        translate([18,21.5,-5]) cube([7,7,10]);
        translate([18,25,-5])cylinder(h=10,r=3.5);
        translate([25,25,-5.5])cylinder(h=10,r=3.5);


        
    }
    translate([2,5.5,10])rotate([0,90,0])cylinder(h=10,r1=1.6,r2=1);
    translate([2,5.5,20])rotate([0,90,0])cylinder(h=10,r1=1.6,r2=1);

    difference(){ //cal haut Wemos
        translate([10,25,33.5]) cube([27,4,5]); 
        translate([35,28-sqrt(2)*15,2])rotate([0,0,45])cube([15,15,27]); 
    }
    translate([10,20,35.5]) cube([27,2,3]); 


    difference(){ //cal bas Wemos
        translate([1.5,18,2]) cube([8,11,37]); 
        translate([8,22,2]) cube([5,3,37]); 
        translate([0,28-sqrt(2)*15,2])rotate([0,0,45])cube([15,15,37]); 
        translate([8,18,2]) cube([4,2,37]); 
    }
    difference(){ // support prise
        group(){
            translate([45,30,-5])rotate([90,0,0])cylinder(h=5,r=5);
            translate([33,25,-8]) cube([15,5,8]);         
            translate([45,25,-5]) cube([5,5,5]); 
        }
        translate([45,31,-5])rotate([90,0,0])cylinder(h=7,r=2);
        translate([33,23,0])rotate([0,135,0])cube([10,10,20]); 
        translate([51,27.5,-1.5])rotate([0,-90,0])screwingHole(10);
    }

    difference(){ // support oppposé prise
        group(){
            translate([45,30,45])rotate([90,0,0])cylinder(h=5,r=5);
            translate([33,25,40]) cube([15,5,8]);         
            translate([45,25,40]) cube([5,5,5]); 
        }
        translate([45,31,45])rotate([90,0,0])cylinder(h=7,r=2);
        translate([33-sqrt(2)*10,23,40])rotate([0,45,0])cube([10,10,30]); 
        translate([51,27.5,41.5])rotate([0,-90,0])screwingHole(10);
        
        
    }

    
}

