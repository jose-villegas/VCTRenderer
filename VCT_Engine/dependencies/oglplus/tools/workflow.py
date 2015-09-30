#!/usr/bin/python
# coding=utf-8
#  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
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

# returns the path of the VERSION file
def version_file(root_dir = get_root_dir()):
	import os

	return os.path.join(root_dir, "VERSION")

# returns the current version numbers in a list
def current_version(root_dir = get_root_dir()):
	import os

	return map(int, open(version_file(root_dir)).read().strip().split("."))

# returns the current version string
def version_string(version_numbers):
	return "%d.%d.%d" % tuple(version_numbers)

def bumped_version_number(version_numbers, number):
	version_numbers[number] += 1
	return version_numbers

def next_release(root_dir = get_root_dir()):
	return bumped_version_number(current_version(root_dir), 1)

def next_hotfix(root_dir = get_root_dir()):
	return bumped_version_number(current_version(root_dir), 2)


# writes contents into a file
def write_file(file_path, contents, simulate=None):

	if simulate is None: simulate = dry_run
	if simulate:
		print("echo '"+contents+"' > "+file_path)
	else:
		open(file_path, 'w').write(contents)

# executes a command in a subprocess
def execute_command(cmd_line, root_dir=get_root_dir(), simulate=None):
	import subprocess

	if simulate is None: simulate = dry_run
	if simulate:
		print(cmd_line[0]+" '"+str("' '").join(cmd_line[1:])+"'")
		return str()
	else:
		proc = subprocess.Popen(
			cmd_line,
			cwd=root_dir,
			stdout=subprocess.PIPE
		)
		out, err = proc.communicate()
		return str(out)

# executes a git command
def git_command(parameters, root_dir=get_root_dir(), simulate=None):
	return execute_command(["git"]+parameters, root_dir, simulate)

# returns the name of the current git branch
def git_current_branch(root_dir=get_root_dir()):
	return git_command(
		["rev-parse", "--abbrev-ref", "HEAD"],
		root_dir,
		False
	).strip()

# returns true if the specified branch exists
def git_has_branch(branch_name, root_dir=get_root_dir):
	return bool(
		git_command(
			["branch", "--list", branch_name],
			root_dir,
			False
		).strip()
	)


# returns true if the specified remote branch exists
def git_has_remote_branch(branch_name, root_dir=get_root_dir):
	git_command(["fetch", "origin"], root_dir, False)
	return bool(
		git_command(
			["branch", "--list", branch_name],
			root_dir,
			False
		).strip()
	)


# Mirrors master to master-with-history
def action_mirror_master():
	root_dir = get_root_dir()
	git_command(["checkout", "master-with-history"], root_dir)
	git_command(["pull", "origin", "master-with-history"], root_dir)
	git_command(["merge", "master"], root_dir)
	git_command(["push", "origin", "master-with-history"], root_dir)
	#git_command(["push", "sourceforge", "master-with-history:master"], root_dir)
	git_command(["checkout", "develop"], root_dir)

# Begins a new release
def action_begin_release():
	root_dir = get_root_dir()
	next_version = version_string(next_release(root_dir))
	git_command(["checkout", "-b", "release-"+next_version, "develop"], root_dir)
	write_file(version_file(root_dir), next_version)
	git_command(["add", version_file(root_dir)], root_dir)
	git_command(["commit", "-m", "Started release-"+next_version], root_dir)
	git_command(["push", "origin", "release-"+next_version], root_dir)

# Finishes a release
def action_finish_release():
	root_dir = get_root_dir()
	current_branch = git_current_branch(root_dir)
	release_version = version_string(current_version(root_dir))
	release_branch = "release-"+release_version
	if current_branch != release_branch:
		release_version = version_string(next_release(root_dir))
		release_branch = "release-"+release_version
		if git_has_branch(release_branch, root_dir):
			git_command(["checkout", release_branch], root_dir)
			git_command(["pull", "origin", release_branch], root_dir)
		elif git_has_remote_branch(release_branch, root_dir):
			git_command(["checkout", "-b", release_branch, "origin/"+release_branch], root_dir)
			git_command(["pull", "origin", release_branch], root_dir)
		else: raise RuntimeError(
			"Release branch '"+release_branch+"' does not exist. "
			"Re-run with --begin-release to start a new release."
		)
	git_command(["checkout", "master"], root_dir)
	git_command(["pull", "origin", "master"], root_dir)
	git_command(["merge", "--no-ff", release_branch], root_dir)
	git_command(["tag", "-a", release_version, "-m", "Tagged release "+release_version], root_dir)
	git_command(["push", "origin", "master"], root_dir)
	git_command(["checkout", "develop"], root_dir)
	git_command(["pull", "origin", "develop"], root_dir)
	git_command(["merge", "--no-ff", release_branch], root_dir)
	git_command(["push", "origin", ":"+release_branch], root_dir)
	git_command(["branch", "-D", release_branch], root_dir)
	git_command(["push", "origin", "develop"], root_dir)


# Begins a new hotfix
def action_begin_hotfix():
	root_dir = get_root_dir()
	next_version = version_string(next_hotfix(root_dir))
	git_command(["checkout", "-b", "hotfix-"+next_version, "master"], root_dir)
	write_file(version_file(root_dir), next_version)
	git_command(["add", version_file(root_dir)], root_dir)
	git_command(["commit", "-m", "Started hotfix-"+next_version])

# Finishes a hotfix
def action_finish_hotfix():
	root_dir = get_root_dir()
	current_branch = git_current_branch(root_dir)
	hotfix_version = version_string(current_version(root_dir))
	hotfix_branch = "hotfix-"+hotfix_version
	if current_branch != hotfix_branch:
		hotfix_version = version_string(next_hotfix(root_dir))
		hotfix_branch = "hotfix-"+hotfix_version
		if git_has_branch(hotfix_branch, root_dir):
			git_command(["checkout", hotfix_branch], root_dir)
			git_command(["pull", "origin", hotfix_branch], root_dir)
		elif git_has_remote_branch(hotfix_branch, root_dir):
			git_command(["checkout", "-b", hotfix_branch, "origin/"+hotfix_branch], root_dir)
			git_command(["pull", "origin", hotfix_branch], root_dir)
		else: raise RuntimeError(
			"Hotfix branch '"+hotfix_branch+"' does not exist. "
			"Re-run with --begin-hotfix to start a new hotfix."
		)
	git_command(["checkout", "master"], root_dir)
	git_command(["pull", "origin", "master"], root_dir)
	git_command(["merge", "--no-ff", hotfix_branch], root_dir)
	git_command(["tag", "-a", hotfix_version, "-m", "Tagged hotfix "+hotfix_version], root_dir)
	git_command(["push", "origin", "master"], root_dir)
	git_command(["checkout", "develop"], root_dir)
	git_command(["pull", "origin", "develop"], root_dir)
	git_command(["merge", "--no-ff", hotfix_branch], root_dir)
	git_command(["push", "origin", ":"+hotfix_branch], root_dir)
	git_command(["branch", "-D", hotfix_branch], root_dir)
	git_command(["push", "origin", "develop"], root_dir)


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
		prog="workflow",
		description="""
			Git Workflow utility script
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

	argparser_action_group = argparser.add_mutually_exclusive_group()
	argparser_action_group.add_argument(
		"--mirror-master",
		dest="action",
		action="store_const",
		const=action_mirror_master,
		help="""
			Merges the master branch into master-with-history.
			Pushes master-with-history to master @ SF.
		"""
	)
	argparser_action_group.add_argument(
		"--begin-release",
		dest="action",
		action="store_const",
		const=action_begin_release,
		help="""
			Starts a new release.
		"""
	)
	argparser_action_group.add_argument(
		"--finish-release",
		dest="action",
		action="store_const",
		const=action_finish_release,
		help="""
			Finishes the current release.
		"""
	)
	argparser_action_group.add_argument(
		"--begin-hotfix",
		dest="action",
		action="store_const",
		const=action_begin_hotfix,
		help="""
			Starts a new hotfix.
		"""
	)
	argparser_action_group.add_argument(
		"--finish-hotfix",
		dest="action",
		action="store_const",
		const=action_finish_hotfix,
		help="""
			Finishes the current hotfix.
		"""
	)

	return argparser

def main():
	try:
		# parse and process the command-line arguments
		argparser = get_argument_parser()
		options = argparser.parse_args()

		global dry_run
		dry_run = options.dry_run

		if options.action:
			options.action()
		else: print("No action specified")
	except RuntimeError as rte:
		print("Runtime error: " + str(rte))

if __name__ == "__main__": main()

