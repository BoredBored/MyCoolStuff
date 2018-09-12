#!/usr/bin/python

vbox_mac = "08:00:27"
mac_file = '/sys/class/net/eth0/address'
infile = open(mac_file, "r")
line = infile.readline()
if line.startswith(vbox_mac):
    print 'Running inside vbox'
