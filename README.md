# PBL-1: Graph Coloring using DSATUR Algorithm

This repository contains the source code for Project-Based Learning 1 (PBL1) at Da Nang University of Science and Technology (DUT). The project simulates the Map Coloring Problem by coloring the vertices of a graph using the **DSATUR** algorithm.

## Key Features
- **DSATUR Implementation:** Efficiently colors graph vertices using a 2D array structure.
- **Multiple Input Modes:** Read data from `input.dat` or input manually via an interactive, step-by-step console UI.
- **Robust Input Validation:** Safely handles incorrect user inputs (e.g., typing letters instead of numbers, self-loops) and supports a quick exit using the 'q' key.
- **Visual Output (Graphviz):** Automatically generates a `.dot` file to visually draw and color the graph.

## Graph Visualization
After running the program, a `graph.dot` file is generated. You can visualize the colored graph by:
1. Installing [Graphviz](https://graphviz.org/) on your machine 
2. Or copying the content of `graph.dot` and pasting it into [GraphvizOnline](https://dreampuf.github.io/GraphvizOnline/).

## Input File Format (`input.txt`)
To use the file input mode, create an `input.dat` file in the same directory as the executable. The data must be formatted as follows:
- **Line 1:** Number of vertices (`V`) and Number of edges (`E`), separated by a space.
- **Next `E` lines:** Each line represents an edge connecting vertex `u` and vertex `v`. *Note: Vertices must be 0-indexed (from `0` to `V-1`).*

**Example `input.dat`:**
```text
5 5
0 1
1 2
2 3
3 4
4 0
```
## Authors
- **Mai Xuan Tai** - Class 25T_DT4
- **Nguyen Quoc Thinh** - Class 25T_DT4
