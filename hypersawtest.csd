<CsoundSynthesizer>
<CsOptions>
-odac --opcode-lib=hypersaw2.dylib

</CsOptions>
<CsInstruments>

sr = 44100
ksmps = 64
nchnls = 1
0dbfs = 1

;basic implementation of BlitB3
instr 1

icps cpsmidi 
aenv madsr 0.1, 0.6, 0.8, 0.2
iamp ampmidi 1
iphase = 0.0
iM = 0.5
asig hypersaw icps, iamp, iphase, iM
outs asig*0.1*aenv

endin

;phase modulation implementation
instr 2 
icps cpsmidi 
aenv madsr 0.1, 0.6, 0.8, 0.2
iamp ampmidi 1
aphsr phasor icps
kphsr phasor icps
iM = 0.5
amod hypersaw icps, iamp, 0.0, iM
acarry vco2 iamp, icps*kphsr, 4, 0.4
asig = amod*acarry+aphsr
asig2 = aenv*asig
outs asig2*0.1, asig2*0.1

endin

;Tone organ with BlitB3
instr 3
icps cpsmidi 
aenv madsr 0.1, 0.6, 0.8, 0.2
iamp ampmidi 1
asig blitsaw icps, iamp
iM = 0.5
asig2 hypersaw icps*1.002, iamp, 0.0, iM
asig3 hypersaw icps*1.004, iamp, 0.25, iM
asig4 hypersaw icps*1.006, iamp, 0.1, iM
asig5 = asig+asig2+asig3+asig4
asig5 = asig5*0.1
outs asig5*aenv
endin

</CsInstruments>
<CsScore>

</CsScore>
</CsoundSynthesizer>






















































<bsbPanel>
 <label>Widgets</label>
 <objectName/>
 <x>100</x>
 <y>100</y>
 <width>320</width>
 <height>240</height>
 <visible>true</visible>
 <uuid/>
 <bgcolor mode="background">
  <r>240</r>
  <g>240</g>
  <b>240</b>
 </bgcolor>
</bsbPanel>
<bsbPresets>
</bsbPresets>
