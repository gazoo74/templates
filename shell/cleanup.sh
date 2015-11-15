#!/bin/sh
#
# The MIT License (MIT)
#
# Copyright (c) 2015 Gaël PORTAY <gael.portay@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

# Set up cleanup on exit!
cleanup() {
	$CLEANUP
}
CLEANUP='eval echo "$$: $0: $cmd returned $?!"'
trap cleanup 0

# Do your tricky work from here...
usage() {
	cat <<EOF
Usage: $0 [CMD [ARGS]]
EOF
}

if [ $# -eq 0 ]; then
	set -- usage
fi

cmd=$1
echo "$$: $0: Running $*..."
$*
# ... downto here!
