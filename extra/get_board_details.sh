#!/bin/bash

# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0

set -e

get_boards() {
	cat boards.txt | sed -e 's/#.*//' | grep -E '^.*\.build\.variant=' | sed -e 's/\.build\.variant=.*//'
}

get_board_field() {
	board=$1
	field=$2
	cat boards.txt | sed -e 's/#.*//' | grep -E "^$board\\.$field=" | cut -d '=' -f2- | sed -e 's/"/\"/g'
}

for BOARD in $(get_boards); do
	NAME=$(get_board_field $BOARD "name")
	VARIANT=$(get_board_field $BOARD "build\\.variant")
	TARGET=$(get_board_field $BOARD "build\\.zephyr_target")
	ARGS=$(get_board_field $BOARD "build\\.zephyr_args")
	HALS=$(get_board_field $BOARD "build\\.zephyr_hals")
	ARTIFACT=$(get_board_field $BOARD "build\\.artifact")
	ARTIFACT=${ARTIFACT:-zephyr_contrib}

	ARTIFACT_JSON=extra/artifacts/$ARTIFACT.json
	if ! [ -f "$ARTIFACT_JSON" ] ; then
		echo "error: missing artifact description file $ARTIFACT_JSON" 1>&2
		exit 1
	fi
	SUBARCH=$(jq -r '.architecture' < $ARTIFACT_JSON)

	if [ -z "$TARGET" ] ; then
		echo "error: missing '$BOARD.build.zephyr_target'" 1>&2
		exit 1
	fi
	if [ -z "$HALS" ] ; then
		echo "error: missing '$BOARD.build.zephyr_hals'" 1>&2
		exit 1
	fi

	cat << EOF
	{
	  "name": "$NAME",
	  "board": "$BOARD",
	  "variant": "$VARIANT",
	  "target": "$TARGET",
	  "args": "$ARGS",
	  "hals": "$HALS",
	  "artifact": "$ARTIFACT",
	  "subarch": "$SUBARCH"
	}
EOF
done | jq -crs .
