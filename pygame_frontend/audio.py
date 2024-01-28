from os import environ
from pygame import midi

if "REPL_OWNER" in environ:
    def init_notes():
        pass
    def get_notes():
        return None
    def quit_audio():
        pass
    def stop_notes():
        pass
    def pump_notes():
        pass
else:
    import pygame.midi
    pygame.midi.init()
    from instrument import MIDINote
    port = pygame.midi.get_default_output_id()
    midi_out = pygame.midi.Output(port, 0)
    def prepare_notes(instrument, volume=0, layers=0, low_note=0, outer_step=0, inner_step=0):
        notes = []
        for layer in range(layers):
            notes.append(list(range(low_note + outer_step * layer, low_note + outer_step * (layer+1), inner_step)))
        adjust_volume = lambda v, n: v - (n - low_note) // 2
        return [[MIDINote(midi_out, instrument=instrument, note=n, volume=volume, length=64 * 36) for n in s] for s in notes]
    notes = type('', (), {})()
    def init_notes():
        notes.fall_notes = prepare_notes(instrument=114, volume=64, layers=4, low_note=48, outer_step=16, inner_step=2)
        notes.move_note = MIDINote(midi_out, instrument=116, note=96, volume=32, length=64)
        notes.drop_note = MIDINote(midi_out, instrument=118, note=32, volume=96, length=128)
        notes.land_note = MIDINote(midi_out, instrument=113, note=48, volume=64, length=128)
        notes.capture_note = MIDINote(midi_out, instrument=15, note=64, volume=72, length=256)
        notes.wrap_note = MIDINote(midi_out, instrument=57, note=64, volume=64, length=256)
    def get_notes():
        return notes
    def pump_notes(passed):
        [[n.pump(passed) for n in s] for s in notes.fall_notes]
        notes.move_note.pump(passed)
        notes.drop_note.pump(passed)
        notes.land_note.pump(passed)
        notes.capture_note.pump(passed)
        notes.wrap_note.pump(passed)
    def stop_notes():
        [[note.stop() for note in layer] for layer in notes.fall_notes]
        notes.move_note.stop()
        notes.drop_note.stop()
        notes.land_note.stop()
        notes.capture_note.stop()
        notes.wrap_note.stop()
    def quit_audio():
        stop_notes()
        midi.quit()

