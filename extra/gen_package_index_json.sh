#!/bin/bash

# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0

set -e

if [ $# -lt 2 ] || [ $# -gt 3 ] ; then
	echo "Usage: $0 <artifact> <artifact_file> [<json_file>]" 1>&2
	exit 1
fi

ARTIFACT=$1
ARTIFACT_FILE=$2
JSON_FILE=${3:-/dev/stdout}

if [ ! -f "$ARTIFACT_FILE" ] ; then
	echo "Artifact file '$ARTIFACT_FILE' not found" 1>&2
	exit 1
fi

if [ "$ARTIFACT" == "zephyr" ] ; then
	exit 0 # no JSON needed for the merged file
fi

if ! [ -f "extra/artifacts/$ARTIFACT.json" ] ; then
	echo "Artifact '$ARTIFACT' not found" 1>&2
	exit 1
fi

BOARD_NAMES=$(extra/get_board_details.sh | jq -c "map(select(.artifact == \"$ARTIFACT\")) | map({name}) | sort")

jq -s '{ "packages": [ { "platforms": [ .[0]*.[1]*.[2] ] } ] }' \
	extra/artifacts/_common.json \
	extra/artifacts/${ARTIFACT}.json \
	- > "$JSON_FILE" << EOF
{
	"version": "$(extra/get_core_version.sh)",
        "url": "https://downloads.arduino.cc/cores/zephyr/$(basename $ARTIFACT_FILE)",
	"archiveFileName": "$(basename $ARTIFACT_FILE)",
	"checksum": "SHA-256:$(sha256sum $ARTIFACT_FILE | awk '{print $1}')",
	"size": "$(stat -c %s $ARTIFACT_FILE)",
	"boards": $BOARD_NAMES
}
EOF
