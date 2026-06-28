#!/bin/bash

# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0

# This script generates a SemVer-compatible version number based on Git tags.
#
# If the current commit is tagged, it returns that version. If not, it
# generates a version string based on the next patch number and the current
# commit hash.
#
# If the tag is a simple "<maj>.<min>.<patch>", git describe will output:
#
#     <maj>.<min>.<patch>-<number-of-commits-since-tag>-g<commit-hash>
#
# The regexp will remove the commit hash and split the patch number, then awk
# converts the tokens to:
#
#     <maj>.<min>.<next-patch>-0.dev
#
# If the tag refers to a pre-release, like "<maj>.<min>.<patch>-<extra-stuff>",
# sed will output a total of 3 arguments and the format of the final awk output
# will be:
#
#     <maj>.<min>.<patch>-<extra-stuff>-0.dev
#
# These are among the lowest possible SemVer versions greater than the last
# tagged version. If there are no tags at all (for example when run in a fork
# etc), it defaults to "9.9.9-<date>".
#
# Finally, non-exact-tag version have a "+<commit-hash-dirty>" appended. The
# commit hash used is taken from HEAD_REF if set - this is to ensure that in CI
# environments the correct commit is used, even in pull requests where HEAD
# might be a temporary detached commit.

VERSION=$(git describe --tags --exact-match --exclude '*/*' 2>/dev/null)
if [ -z "$VERSION" ]; then
	STEM=$(git describe --tags --exclude '*/*' 2>/dev/null |
	       sed 's/\.\([[:digit:]]\+\)\(-.*\)*-[[:digit:]]\+-g.*/ \1 \2/' |
	       awk '{ if (NF==2) { print $1 "." ($2+1) "-0.dev" } else { print $1 "." $2 $3 "-0.dev" }}')
	if [ -z "$STEM" ]; then
		# no tags: build in a fork or locally, use a maxed out version with date+hash
		STEM="9.9.9-$(date '+%Y%m%d-%H%M%S')"
	fi

	# If HEAD_REF is not set, we're not in CI but in a local clone. Use the
	# implicit HEAD and include --dirty to test for uncommitted changes.
	VERSION="${STEM}+$(git describe --always ${HEAD_REF:---dirty})"
fi
echo $VERSION
