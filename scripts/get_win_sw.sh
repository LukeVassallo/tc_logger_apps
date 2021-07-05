#/bin/bash

ver="v0.0.0"
date="2021/07/05 21:36"

printf "\n"
printf "  **** Thermocouple Configuration and Datalogging Software\n"
printf "   *** Automated downloader, validator and extractor\n"
printf "    ** ./get_win_sw %s %s\n" "$ver" "$date"
printf "\n\n"
sleep 1

FILE=tc_config_and_log_sw_0_0_3_win_x86_64
DIR=win

mkdir -p $DIR
cd $DIR
echo "Downloading software package ..."
wget https://www.lukevassallo.com/thermocouple_datalogger/sw/$DIR/$FILE.zip
echo "Downloading md5 checksum ... "
wget https://www.lukevassallo.com/thermocouple_datalogger/sw/$DIR/$FILE.md5

echo "Validating md5 checksum ... "
md5sum -c $FILE.md5
# md5sum returns 0 if the calculated hash matches what's in the file, otherwise 1
return_value=$?		

if [ $return_value -eq 1 ]; then
	echo "The archive is corrupted. Please try re-downloading."
	exit
fi

echo "Extracting archive... "
unzip $FILE.zip

echo ""
echo "All done. Please refer to lnx/Thermocouple\ Configuration\ and\ Logging\ Software/docs/installation_guide.pdf for software installation."
