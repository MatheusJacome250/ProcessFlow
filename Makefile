processflow: processflow.c
	gcc -Wall -Wextra -std=c11 processflow.c -o processflow

run: processflow
	./processflow

clean:
	rm -f processflow

.PHONY: run clean