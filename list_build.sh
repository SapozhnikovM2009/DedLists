#!/bin/bash
gcc List.c listlib.c -o list
dot -Tsvg graph.dot -o graph.svg