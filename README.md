# Multicore

README

Please download the zip file and cd into the proper directory.

Once you cd into the Multicore folder, please run the following command in terminal:

[ujs204@crunchy1 Multicore]$ make

Note that the sudoku solver runs on inputs. 

Some examples of inputs we attached in this file are: input.txt, input9.txt, input16.txt and input_4_1.txt.

Each input has a board of different sizes, and should give you an idea of how to create your own input. If you want a randomly generated Sudoku board of a certain dimension, our program also takes the boards given by Chat GPT (e.g. "generate a 9 by 9 sudoku board" to ChatGPT). 

Once you have the given dimension, you must go to the Makefile and change the BLK_SIZE_SZ variable based on the size of one block within the board (e.g. 16X16 board has block size of 4).


And, to change the number of threads available, you have to go to the solver_cfg_1_seq.cpp file and change the NUM_THREADS value to what you want.

Please don't hesitate to reach out if there are any questions.

Finally to run the code, please type the following command:

[ujs204@crunchy1 Multicore]$ ./seq1 <input.txt

The result will show.

Enjoy!
