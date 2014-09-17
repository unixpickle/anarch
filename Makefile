dependencies:
	mkdir dependencies
	git clone --branch v0.1.0 https://github.com/unixpickle/ansa.git dependencies/ansa 2>/dev/null
	git clone --branch v0.1.1 https://github.com/unixpickle/analloc2.git dependencies/analloc2 2>/dev/null
	git clone https://github.com/unixpickle/makemaker.git dependencies/makemaker

clean-all:
	rm -rf dependencies
