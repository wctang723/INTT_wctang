import numpy as np

dt = np.dtype([('name', np.str_, 16), ('grades', np.int64, (2,))])
dy = np.dtype([('runnumber', np.str_, 24), ('hitratio', np.str_, 24)])
sql = '''INSERT INTO event_num_dummy(run_number, event_num)
             VALUES%s;'''

filename = 'dummydata.dat'
dy = np.dtype([('runnumber', np.float64), ('hitratio', np.float64)])
data = np.loadtxt(filename, delimiter = ' ', dtype = dy, unpack = False)
tup = (1,2) 

numbers = '%s' % (data)
print(sql % numbers)

#print(dt['name'])
#print(dt['grades'])

x = np.array([('Sarah', (8, 7)), ('John', (6, 7))], dtype = dt)
y = np.array([(27790, 100000)], dtype = dy)

#print(x[1]['grades'])
#print(y[0])
