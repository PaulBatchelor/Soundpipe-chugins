SawOsc s => LPF f => Gain g2 => blackhole;

g2 => TEnv t => Gain g;

g => Rpt r => dac;

1000 => f.freq;
0.01 => t.atk;
0.05 => t.hold;
0.03 => t.rel;

130.0 => float bpm;

bpm => r.bpm;
8 => r.div;
4 => r.rep;
(60/bpm)::second => dur T;

while(1) 
{
    if(Std.rand2(0,2) == 0) r.trig();
    t.trig();   
    T => now;
}
