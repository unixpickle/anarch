dependencies:
	mkdir dependencies
	git clone https://github.com/unixpickle/ansa.git dependencies/ansa
	git clone https://github.com/unixpickle/analloc2.git dependencies/analloc2
	git clone https://github.com/unixpickle/makemaker.git dependencies/makemaker

clean-all:
	rm -rf dependencies
