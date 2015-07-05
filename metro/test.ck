Metro m => TEnv t => blackhole;
SinOsc s => dac;

function void control() {
    while(1::samp => now) {
        t.last() => s.gain;
    }
}

spork ~ control();

0 => t.mode;

while(1) 
{
    1::second => now;
}
