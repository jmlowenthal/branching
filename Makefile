build: predictor

predictor: predictor.cpp
	g++ -o predictor predictor.cpp

run: build
	gzip -dc gcc.trace.gz | ./predictor