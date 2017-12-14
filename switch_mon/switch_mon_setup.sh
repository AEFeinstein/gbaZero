#!/bin/bash

# Make sure this script is run as root
if [ "$EUID" -ne 0 ]
	then echo "$0: Please run as root"
	exit
fi

# Make sure files are where they need to be
file="/opt/switch_mon/switch_mon"
if [ ! -f "$file" ]
then
	echo "$0: '${file}' not found. Please move the switch_mon folder to /opt or modify switch_mon.service accordingly"
	exit
fi

# Copy the service file to where it needs to be
cp switch_mon.service /etc/systemd/system/
if [ $? -ne 0 ]; then
    echo "$0: Copying the service failed"
	exit
fi

# Enable the service, so it gets started on boot
systemctl enable switch_mon.service
if [ $? -ne 0 ]; then
    echo "$0: Enabling the service failed"
	exit
fi

# Start the service for good measure
systemctl start switch_mon.service
if [ $? -ne 0 ]; then
    echo "$0: Starting the service failed"
	exit
fi

echo "$0: switch_mon set up"
