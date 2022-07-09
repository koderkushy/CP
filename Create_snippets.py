import os
import re

snip_head = """<snippet>
	<content><![CDATA["""

snip_tail = """]]></content>
	<!-- Optional: Set a tabTrigger to define how to trigger the snippet -->
	<tabTrigger>TAB_TRIGGER</tabTrigger>
	<!-- Optional: Set a scope to limit where the snippet will trigger -->
	<!-- <scope>source.python</scope> -->
</snippet>"""

for root, subdirs, files in os.walk(os.getcwd() + "\\Library"):
	for file in files:

		print("At", file)
		
		read_dir = root
		write_dir = root.replace("Library", "Snippets")

		read_file = "\\" + file
		write_file = "\\" + file.replace("hpp", "sublime-snippet")

		code = open (read_dir + read_file, "r").read()
		# print(code)

		tab_trigger = None

		if not os.path.exists(write_dir):
			os.makedirs(write_dir)

		if not os.path.exists(write_dir + write_file):
			tab_trigger = input("Enter tab trigger for " + file + "\n")
			pass
			
		else:
			initial = open (write_dir + write_file, "r").read()
			tab_trigger = re.search(r"<tabTrigger>(.*)</tabTrigger>", initial, flags=re.DOTALL).group(1)
		
		tail = snip_tail.replace("TAB_TRIGGER", tab_trigger)

		code = '\n'.join([snip_head, code, tail])

		open(write_dir + write_file, "w").write(code)

		print("done")
	