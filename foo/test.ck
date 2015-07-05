SawOsc s => Butlp m => dac;

while(1) 
{
    Std.rand2f(50, 1000) => m.cutoff;
    0.1::second => now;
}
