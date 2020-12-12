
$fn=100;

//Cadran
union(){
difference() {
    translate([-80,-10,-2])cube([160,90,2]);
    difference() {
        translate([0,0,-0.5])cylinder(h=3,r=71);
        translate([0,0,-0.5])cylinder(h=5,r=69);
    }
    for (a=[0:-15:-180]){
        rotate(a=a, v=[0,0,1])translate([-73,-0.5,-0.5])cube([3,1,1]);
    }
    for (a=[0:-45:-180]){
        rotate(a=a, v=[0,0,1])translate([-73,-0.5,-0.5])cube([6,1,1]);
    }    
    translate([0,0,-5])cylinder(h=10,r=2);
}
    translate([-75,-10,-1])cube([10,9.5,1]);
    translate([65,-10,-1])cube([10,9.5,1]);
}

//Eguille longeur 68
*cube([60,4,2]);
