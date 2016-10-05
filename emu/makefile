init:
	rm -rf v1.3.tar.gz
	wget https://github.com/buserror/simavr/archive/v1.3.tar.gz
	tar xvf v1.3.tar.gz
	mv simavr-1.3 simavr
	cd simavr && make

# Install required packages for debian/ubuntu
install-pkgs-debian:
	sudo apt-get install avr-libc libelf-dev freeglut3-dev pkg-config

clean:
	rm -rf simavr simavr-1.3 v1.3.tar.gz


