#!/bin/bash
# Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
sample_exe="${1}"
screenshot="${2:-${1}.png}"
convert="${3:-$(which convert)}"

if [[ "$(dirname ${sample_exe})" == "." ]]
then sample_exe="./${sample_exe}"
fi

function oglplus_screenshot_fallback()
{
	echo "Failed to create screenshot for example '$(basename ${sample_exe})'" 1>&2
	echo "Using fallback image."
	if ! [[ -f "$(dirname ${screenshot}/not_available.png)" ]]
	then cp \
		"$(dirname $0)/../source/textures/not_available.png" \
		"$(dirname ${screenshot})/not_available.png"
	fi
	ln -T -f "$(dirname ${screenshot})/not_available.png" "${screenshot}"
}

if [[ -x "${convert}" ]]
then
	if [[ "${sample_exe}" == "" ]]
	then echo "Example executable not specified." 1>&2 && exit 1
	fi

	if [[ "${screenshot}" == "" ]]
	then echo "Screenshot path not specified." 1>&2 && exit 2
	fi

	if ! [[ -d $(dirname "${screenshot}") ]]
	then mkdir -p $(dirname "${screenshot}")
	fi

	if ! [[ -d $(dirname "${screenshot}") ]]
	then echo "Could not create directory for '${screenshot}'" 1>&2 && exit 3
	fi

	if ! [[ -w $(dirname "${screenshot}") ]]
	then echo "Cannot write to '$(diranme ${screenshot})'" 1>&2 && exit 4
	fi

	if ! [[ -f "${sample_exe}" ]]
	then oglplus_screenshot_fallback && exit 0
	fi

	if ! [[ -x "${sample_exe}" ]]
	then oglplus_screenshot_fallback && exit 0
	fi

	timeout 15s "${sample_exe}" --screenshot "${screenshot%.*}.rgb"

	if [[ $? -ne 0 ]]
	then oglplus_screenshot_fallback
	else
		"${convert}" \
			-depth 8 \
			-size 800x600 \
			"${screenshot%.*}.rgb" \
			-flip \
			-adaptive-resize 400x300 \
			"${screenshot}"
	fi
else oglplus_screenshot_fallback
fi
