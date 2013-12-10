default: daemon app
	echo "...done with everything!"
	
buildenv:
	echo "Setting up environment..."
	mkdir -p ./obj
	mkdir -p ./bin
	echo "...done!"
	
daemon: buildenv
	echo "Building the damon..."
	g++ src/TimeCop_daemon.cpp src/myLogfile.cpp -lbe -o bin/TimeCop_daemon
	rc src/TimeCop_daemon.rdef -o TimeCop_daemon.rsrc
	xres TimeCop_daemon.rsrc -o bin/TimeCop_daemon
	echo "...done!"
	
app: buildenv
	echo "Building the app..."
	g++ src/BalkenWin.cpp src/ShadowBox.cpp src/StatistikView.cpp src/main.cpp \
	src/LogWin.cpp src/menu.cpp src/myLogfile.cpp -lbe -o bin/TimeCop
	rc src/TimeCop.rdef -o TimeCop.rsrc
	xres TimeCop.rsrc -o bin/TimeCop
	echo "...done!"
	
clean:
	rm ./obj/* ./bin/*
