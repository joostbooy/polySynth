import numpy
import sys
dir = '/Users/joostbooij/Desktop/Code/lib/scripts'
sys.path.insert(0,dir)
import lutCompiler


#table variables
tables = []

ppqn = 24
min_bpm = 30
max_bpm = 300
max_bpm_fractional = 10
update_freq = 8000

num_mux_channels = 8
sample_rate = update_freq / num_mux_channels


'''____________________
	BPM INC
________________________'''

name = 'bpm_inc'

bpm = numpy.arange(min_bpm, max_bpm + 1)
hertz = bpm / 60 * ppqn
values = hertz / update_freq * (1 << 32);

tables.append('uint32_t ' + name)
tables.append(values.astype(int))

'''__________________________
	BPM FRACTIONAL INC
_____________________________'''

name = 'bpm_fractional_inc'

stepsize = 1.0 / max_bpm_fractional
bpmFraction = numpy.arange(0.00, 1.00, stepsize)
hertz = bpmFraction / 60 * ppqn
values = hertz / update_freq * (1 << 32);

tables.append('uint32_t ' + name)
tables.append(values.astype(int))

'''____________________
        PHASE INC
________________________'''
phase_table_size = 256

name = 'phase_length'
freq_min = 0.1
freq_max = 50.0
steps = numpy.linspace(freq_min, freq_max, phase_table_size)

tables.append('float ' + name)
tables.append(steps.astype('float32'))

name = 'phase_inc'
freq_inc_min = freq_min / sample_rate
freq_inc_max = freq_max / sample_rate
steps = numpy.linspace(freq_inc_min, freq_inc_max, phase_table_size)

tables.append('float ' + name)
tables.append(steps.astype('float32'))

'''_____________________________
     BEAT LENGTH
_____________________________'''

name = 'beat_length_inc'
values = 1.0 / (sample_rate * (60 / bpm))
tables.append('float ' + name)
tables.append(values.astype('float32'))

'''____________________
	EXP TABLE
________________________'''

exp_table_size = 1024
stepsize = 1.0 / exp_table_size
x = numpy.arange(0, 1.0, stepsize)

name = 'exp'
values = 1.0 - numpy.exp(-6 * x)
values /= values.max()

tables.append('float ' + name)
tables.append(values.astype('float32'))

name = 'inv_exp'
values = 1.0 - values[::-1]

tables.append('float ' + name)
tables.append(values.astype('float32'))

'''____________________
	DEFINES
________________________'''

defines = [
'SAMPLE_RATE '			+ str(int(sample_rate)),
'MIN_BPM '				+ str(int(min_bpm)),
'MAX_BPM '				+ str(int(max_bpm)),
'MAX_BPM_FRACTIONAL '	+ str(int(max_bpm_fractional)),
'UPDATE_FREQ '			+ str(int(update_freq)),
'EXP_TABLE_SIZE '		+ str(int(exp_table_size)),
'PHASE_TABLE_SIZE '		+ str(int(phase_table_size)),
]

'''____________________
	COMPILING
________________________'''

# set directory to write tables (argument given in makeFile)
lutCompiler.setDir(str(sys.argv[1]))
# compile and store
lutCompiler.compile(tables, defines)
