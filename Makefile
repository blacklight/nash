all:
	g++ -o nash main.cpp nash.cpp -g

clean:
	rm tags
	rm nash
