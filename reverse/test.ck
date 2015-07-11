SawOsc s => Butlp m => TEnv t => Reverse r => dac;
t.atk(0.001);
t.hold(0.001);
t.rel(0.3);
r.delay(1);
r.mix(0.3);
while(1) 
{
    t.trig();
    Std.rand2f(50, 1000) => s.freq;
    1::second => now;
}
