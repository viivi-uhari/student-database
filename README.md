ELEC-A7100 Basic course in C programming
Aalto University
Viivi Uhari

This project is for creating and handling a simple student database.
The database has the student number, firstname, lastname,
points for 6 different rounds and the total points of these 6 rounds for each student.

The database can be used via command line.
When in the directory where the project.c file is located
the program can be compiled, for example with gcc, using the command
'gcc -g -Wall -std=c99 -o test project.c'
After this the program can be run with the command
'./test'

The commands start with a capital letter of the following ones:
'A', 'U', 'L', 'W', 'O' or 'Q'.
After these, numbers or strings can follow and the specifics are explained below.

A: Add a student; student number, lastname and firstname
Example: A 1234 Korhonen Kalle

L: Print points; student number, lastname, firstname,
round points divided by spaces, total points

U: Update a student's points; student number,
number of the round and the number of the new points
Example: U 1234 1 7
(round 1, points 7)

W: Save results. Writes results to the file with the given filename
Example: W testFile.txt

O: Load results. Loads results from the file with the given filename
and replaces the existing results
Example: O testFile.txt

Q: Quits the program

The program reports whether the command was valid or not.
