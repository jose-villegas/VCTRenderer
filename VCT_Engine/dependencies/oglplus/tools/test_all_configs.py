#!/usr/bin/python
# coding=utf-8
#  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# globally enables/disables the "dry-run" mode
dry_run = False

# returns a normalized path to the project root directory
def get_root_dir():
	import os
	import sys

	return os.path.normpath(
		os.path.join(
			os.path.dirname(sys.argv[0]),
			os.path.pardir
		)
	)

# returns the path of the config script
def configure_script(root_dir = get_root_dir()):
	import os

	return os.path.join(root_dir, "configure.py")

# executes a command in a subprocess
def execute_command(cmd_line, root_dir=get_root_dir(), simulate=None):
	import subprocess

	if simulate is None: simulate = dry_run
	if simulate:
		print(cmd_line[0]+" '"+str("' '").join(cmd_line[1:])+"'")
	else:
		proc = subprocess.Popen(cmd_line, cwd=root_dir)
		proc.communicate()

def execute_configure(parameters, simulate=None):
	execute_command(
		[configure_script()]+
		parameters,
		get_root_dir(),
		simulate
	)

# creates the command line argument parser
def get_argument_parser():
	import argparse
	import datetime

	def BoolArgValue(arg):
		if(arg in ("True", "true", "Yes", "yes", "Y", "On", "1")):
			return True
		elif(arg in ("False", "false", "No", "no", "N", "Off", "0")):
			return False
		else:
			msg = "'%s' is not a valid boolean value" % str(arg)
			raise argparse.ArgumentTypeError(msg)

	argparser = argparse.ArgumentParser(
		prog="test_all_configs",
		description="""
			Utility scripts that tried to build all OGLplus
			in all available configurations
		""",
		epilog="""
			Copyright (c) 2008 - %(year)d Matúš Chochlík.
			Permission is granted to copy, distribute and/or modify this document
			under the terms of the Boost Software License, Version 1.0.
			(See a copy at http://www.boost.org/LICENSE_1_0.txt)
		""" % { "year": datetime.datetime.now().year }
	)

	argparser.add_argument(
		"--dry-run",
		default=False,
		action="store_true",
		help="""
			Only print the commands that should be executed
			but don't do anything.
		"""
	)
	argparser.add_argument(
		"--config",
		dest="config_options",
		nargs=argparse.REMAINDER,
		default=list(),
		help="""Everything following this option will be passed to configure verbatim."""
	)

	return argparser

def main():
	import os, sys
	try:
		# parse and process the command-line arguments
		argparser = get_argument_parser()
		options = argparser.parse_args()

		global dry_run
		dry_run = options.dry_run

		# get configuration info
		print("Getting configuration info...")
		execute_configure(options.config_options+[
			"--from-scratch",
			"--info-only"
		])
		# load configuration info
		info_py_path=os.path.join(get_root_dir(), '_build', 'config', 'info.py')
		info_py=open(info_py_path).read()
		exec(info_py) in locals()

		# process the configurations
		for gl_api_lib in oglplus_config_info['GL_API_LIBS']:
			for gl_init_lib in oglplus_config_info['GL_INIT_LIBS']:
				for additional_opts in [
					['--low-profile', 'True'],
					['--low-profile', 'False']
				]:
					conf_msg = "Building with %(api)s+%(init)s %(opts)s\n" % {
						'api': gl_api_lib,
						'init': gl_init_lib,
						'opts': ' '.join(additional_opts)
					}
					sys.stdout.write(conf_msg)
					sys.stderr.write(conf_msg)
					execute_configure(options.config_options+[
						"--from-scratch",
						"--use-gl-api-lib="+gl_api_lib,
						"--use-gl-init-lib="+gl_init_lib,
						"--build"
					]+additional_opts, dry_run)
	except RuntimeError as rte:
		print("Runtime error: " + str(rte))

if __name__ == "__main__": main()

