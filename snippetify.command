#! /usr/bin/python3

import os
import re

def show_exception_and_exit(exc_type, exc_value, tb):
    import traceback
    traceback.print_exception(exc_type, exc_value, tb)
    print("Press any key to exit.", flush = True)
    input()
    sys.exit(-1)

import sys
sys.excepthook = show_exception_and_exit

snip_head = """<snippet>
	<content><![CDATA["""

snip_tail = """]]></content>
	<!-- Optional: Set a tabTrigger to define how to trigger the snippet -->
	<tabTrigger>TAB_TRIGGER</tabTrigger>
	<!-- Optional: Set a scope to limit where the snippet will trigger -->
	<!-- <scope>source.python</scope> -->
</snippet>"""

include_guard = re.compile('#.*CP_[A-Z_]*_HPP')

cpp_extension = re.compile('[A-Za-z_]*\.(hpp|h)')

empty_string = re.compile('[\s\n]*')

def dfs (file_path, included):
	code = open(file_path, "r").read()
	included.append(file_path)
	result = []

	for line in code.splitlines():
		if include_guard.match(line):
			continue
		result.append(line)

	while len(result) > 0 and result[0] == '':
		result.pop(0)

	while len(result) > 0 and result[-1] == '':
		result.pop(-1)

	return result

lib_path = "/Users/kushyalt/Library/Application Support/Sublime Text/Packages/User/lib"

for root, subdirs, files in os.walk(lib_path):
	for file in files:

		if not cpp_extension.match(file):
			continue

		print("At", file)
		
		read_dir = root
		write_dir = root.replace("lib", "snippets")

		read_file = "/" + file
		write_file = "/" + file.replace("hpp", "sublime-snippet")

		tab_trigger = None

		if os.path.exists(write_dir):
			if os.path.exists(write_dir + write_file):
				if os.path.getmtime(write_dir + write_file) > os.path.getmtime(read_dir + read_file):
					# print("No changes")
					continue;
				else:
					initial = open (write_dir + write_file, "r").read()
					tab_trigger = re.search(r"<tabTrigger>(.*)</tabTrigger>", initial, flags=re.DOTALL).group(1)
		else:
			os.makedirs(write_dir)

		if tab_trigger is None or len(tab_trigger) == 0:
			tab_trigger = input("Enter tab trigger for " + file + "\n")

		tail = snip_tail.replace("TAB_TRIGGER", tab_trigger)

		code = open (read_dir + read_file, "r").read()
		code = '\n'.join(dfs(read_dir + read_file, []))		
		code = '\n'.join([snip_head, code, tail])

		open(write_dir + write_file, "w").write(code)

		print("updated", write_file)
	