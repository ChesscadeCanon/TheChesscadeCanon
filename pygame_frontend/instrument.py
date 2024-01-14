from pygame import midi 

class MIDINote:

    def __init__(self, midi_out, note, volume):
        self.note = note
        self.volume = volume
        self.midi_out = midi_out
        self.time = -1
        self.length = 0
        
    def play(self, milliseconds):
        self.midi_out.note_on(self.note, self.volume) # 74 is middle C, 127 is "how loud" - max is 127
        self.time = 0
        self.length = milliseconds
        
    def pump(self, milliseconds):
        if self.time < 0:
            return
        self.time += milliseconds 
        if self.time >= self.length:
            self.midi_out.note_off(self.note, self.volume)
            self.time = -1
            self.length = 0