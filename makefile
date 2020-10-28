all:
	cd bin2txt; make; cd ..
	cd env; make; cd ..
	cd memcheck; make; cd ..
	cd secs2time; make; cd ..
	cd spaces2tabs; make; cd ..
	cd tabs2spaces; make; cd ..

clean:
	cd bin2txt; make clean; cd ..
	cd env; make clean; cd ..
	cd memcheck; make clean; cd ..
	cd secs2time; make clean; cd ..
	cd spaces2tabs; make clean; cd ..
	cd tabs2spaces; make clean; cd .. 

