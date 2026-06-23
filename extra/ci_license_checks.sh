#!/bin/bash

# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0


#
# License check configuration
#

# - ignored extensions (ignore *.ext)
IGNORE_EXTS="md rst txt" # text files
IGNORE_EXTS+=" ino pde" # arduino sketch files
IGNORE_EXTS+=" json yaml yml" # configuration files (also ignored by scancode-action)

# - source file extensions (match *.ext)
SOURCE_EXTS="c cc cpp h hh hpp" # C/C++ source and header files
SOURCE_EXTS+=" cmake conf dts dtsi overlay ld" # build / configuration files
SOURCE_EXTS+=" py sh go" # scripts

# - source file names (match <name>)
SOURCE_FILES="CMakeLists.txt Kconfig Kconfig.default" # build / configuration files

# - Arduino official copyright text
ARDUINO_COPYRIGHT='Copyright (c) Arduino s.r.l. and/or its affiliated companies'


#
# Run the checks
#

SCANCODE_INPUTS=${1}
SCANCODE_RESULTS=${2}
if ! [ -d "$SCANCODE_INPUTS" ] || [ -z "$SCANCODE_RESULTS" ] || ! [ -s "$SCANCODE_RESULTS" ] ; then
    echo "Usage: $0 <input-path> <results.json>"
    exit 1
fi

ignore_ext_re=$(echo $IGNORE_EXTS | sed -e 's/\s\+/|/g' -e 's/.*/\\.(&)$/')

source_ext_re=$(echo $SOURCE_EXTS | sed -e 's/\s\+/|/g' -e 's/.*/\\.(&)$/')
source_file_re=$(echo $SOURCE_FILES | sed -e 's/\s\+/|/g' -e 's/\./\\./g' -e 's/.*/\<(&)$/')
source_match_re="($source_ext_re|$source_file_re)"

RETVAL=0

for NEW_FILE in $(cd "$SCANCODE_INPUTS" && find . -type f | sed -e 's/..//'); do
	if echo $NEW_FILE | grep -qE "${ignore_ext_re}" ; then
		# ignored file
		echo "Ignoring new file: $NEW_FILE"
		continue
	elif echo $NEW_FILE | grep -qE "${source_match_re}" ; then
		# matches the source file criteria
		KIND="source"
	else
		# anything else
		KIND="file"
	fi

	echo "Checking new $KIND: $NEW_FILE"

	FILE_RES=$(jq -cr ".files[] | select(.path == \"$NEW_FILE\")" $SCANCODE_RESULTS)
	FILE_RES=${FILE_RES:-\{\}} # should never happen, results in missing license and copyright info
	STATUS=$(echo "$FILE_RES" | jq -r '.status // empty' 2>/dev/null)
	COMPLIANCE=$(echo "$FILE_RES" | jq -r '.compliance_alert // empty' 2>/dev/null)
	LICENSE=$(echo "$FILE_RES" | jq -r '.detected_license_expression // empty' 2>/dev/null)
	COPYRIGHT=$(echo "$FILE_RES" | jq -r '.copyrights[0].copyright // empty' 2>/dev/null)
	LINE_NUMBER=1

	if [ "$STATUS" != "scanned" ] ; then
		# anything else is a problem with scancode
		MSG="File not scanned: '$STATUS'"
		TYPE="warning"
	elif ! ( [ -z "$COMPLIANCE" ] || [ "$COMPLIANCE" == "ok" ] ) ; then
		# anything other than ok or empty is a hard error
		MSG="License compliance $COMPLIANCE for license(s): $LICENSE"
		TYPE="error"
		RETVAL=1
	elif [ -z "$LICENSE" ] || [ -z "$COPYRIGHT" ] ; then
		# Something is missing
		MSG=""
		[ -z "$LICENSE" ] && MSG="license"
		[ -z "$COPYRIGHT" ] && MSG="${MSG}${MSG:+ and }copyright"
		MSG="Missing $MSG information"
		# missing info in source files is an error, in other files is a warning
		if [ $KIND == "source" ] ; then
			TYPE="error"
			RETVAL=1
		else
			TYPE="warning"
		fi
	elif echo "$COPYRIGHT" | grep -qi "arduino" ; then
		# Arduino copyright found, check it matches exactly
		LINE_NUMBER=$(echo "$FILE_RES" | jq -r '.copyrights[0].start_line')
		FULL_LINE=$(sed -n "${LINE_NUMBER}p" "$SCANCODE_INPUTS/$NEW_FILE")
		if ! echo "$FULL_LINE" | grep -qF "$ARDUINO_COPYRIGHT" ; then
			MSG="Arduino copyright must be: '$ARDUINO_COPYRIGHT'"
			TYPE="error"
			RETVAL=1
		else
			# looks good
			continue
		fi
	else
		# no issues
		continue
	fi

	echo "::$TYPE file=$NEW_FILE,line=$LINE_NUMBER::$MSG"
done

exit $RETVAL
