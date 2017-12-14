switch_mon is a small C program which polls the pin connected to power switch.
If the switch is switched off for four seconds, the system will kill any emulators.
If the switch is then switched on for four seconds, it will disconnect the internal enable for the PowerBoost 1000c and safely shutdown.
If the switch is then switched off, the system will power down.

switch_mon.service is a systemd service to run switch_mon in the background
switch_mon_setup.sh will install the service