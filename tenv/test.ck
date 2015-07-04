function void beeper(float sec, float freq) {
    SinOsc s => TEnv e => dac;
    0.003 => e.atk;
    0 => e.hold;
    0.3 => e.rel;
    0.2 => s.gain;
    freq => s.freq;
    while(1) 
    {
        e.trig();        
        sec::second => now;
    }
}

spork ~ beeper(0.2, Std.mtof(61));
spork ~ beeper(0.3, Std.mtof(59));
beeper(0.4, Std.mtof(66));