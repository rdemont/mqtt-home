//resolution
R=30;

//BOX

union(){
    difference (){
        difference (){
            cube(size=[40,30,32],$fn=R);
            translate([2,2,2])cube(size=[36,26,33],$fn=R);
        }
        //hole power
        translate([-1,7.5,2])cube(size=[8,16,10],$fn=R);
    }
    //inside support
    translate([2,2,2])cube(size=[1,26,1],$fn=R);
    translate([32,2,2])cube(size=[6,26,1],$fn=R);
}


//Cover
/*
translate([-2,-2,2])cube(size=[40,30,2],$fn=R);
cube(size=[36,26,2],$fn=R);
*/