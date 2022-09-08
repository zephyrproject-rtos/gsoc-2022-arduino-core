#!/bin/bash
set -e;

printf '%s\n' "Before running this script, make sure of the following:"
printf '%s\n' "1. You have zephyr installed properly on your system."
printf '%s\n' "2. Updated west.yml in zephyr and run west update to pull this module in proper location."
printf '%s\n' "If you meet the above criteria, please enter [yY]Yes and if not, then [nN]No."
read answer;

if [ "$answer" != "${answer#[Yy]}" ] ;then
    printf '%s\n' "Yes";
else
    printf '%s\n' "No";
	exit 1;
fi

API_FOLDER=~/.ArduinoCore-API
if [ ! -d "$API_FOLDER" ] ; then
	printf '%s\n' "Cloning ArduinoCore API in $API_FOLDER";
	git clone git@github.com:arduino/ArduinoCore-API ~/.ArduinoCore-API;
else
	printf '%s\n' "API Folder already exists, skipping clone...";
fi

printf '%s\n' "Commenting out WCharacter.h because it fails to build properly";
sed '/WCharacter.h/ s/./\/\/ &/' ~/.ArduinoCore-API/api/ArduinoAPI.h > ~/.ArduinoCore-API/api/tmpArduinoAPI.h ;
mv ~/.ArduinoCore-API/api/tmpArduinoAPI.h ~/.ArduinoCore-API/api/ArduinoAPI.h ;

printf '%s\n' "Linking...";
ln -sf ~/.ArduinoCore-API/api cores/arduino/. ;

printf '%s\n' "Module Successfully setup...";

