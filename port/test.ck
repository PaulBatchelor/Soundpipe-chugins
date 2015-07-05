Step f => Port p => blackhole;
SinOsc s => dac;


function void control() 
{
    while(1::samp => now) 
    {
        p.last() => s.freq;
    }
}

spork ~ control();
while(1) {
    Std.rand2f(100, 800) => f.next;
    300::ms => now;
}