from pygame import midi 

class MIDINote:

    def __init__(self, midi_out, instrument=0, note=0, volume=0, length=0):
        self.note = note
        self.volume = volume
        self.midi_out = midi_out
        self.time = -1
        self.length = length
        self.instrument = instrument
        
    def play(self):
        if self.time >= 0:
            return
        self.midi_out.set_instrument(self.instrument)
        self.midi_out.note_on(self.note, self.volume) # 74 is middle C, 127 is "how loud" - max is 127
        self.time = 0
     
    def stop(self):
        self.midi_out.note_off(self.note, self.volume)
        self.time = -1

    def pump(self, milliseconds):
        if self.time < 0:
            return
        self.time += milliseconds 
        if self.time >= self.length:
            self.stop()