#!/usr/bin/python3
import numpy as np
from psycopg2.extensions import register_adapter, AsIs
from insert_row import *
from create_tables import create_tables


filename = 'hit_monitor_beam.csv'

#create_tables()  # This is the command to create table in database

dy = np.dtype([('runnumber', np.float64), ('eventnum', np.float64), ('hitavg', np.float64)])
#dy = np.dtype([('runnumber', np.float64), ('hotnum', np.float64)])
data = np.loadtxt(filename, delimiter = ' ', dtype = dy, unpack = False)
#data_list = list(data.to_records())

#print('Creating tables...')
for i in range(10):
    print(data[i])
    insert_event_num(data[i])

