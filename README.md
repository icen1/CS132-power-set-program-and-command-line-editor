# CS132-power-set-program-and-command-line-editor
My second coursework for CS132 (Computer Organistion and Architecture). Where we had to get the power set of any set inputted in a program and make a command line editor in C.
# Documentation 
Question 1:
Insert the elements you want when prompted, and put a , between each element. 
otherwise they will be treated as one element

Question 2:

Commands available:
create - Creates a file and overwrites the file if it exists
copy – Copies the content of a file to another
delete – Deletes a file 
show – Reads the content of a file
append – Adds a line at the end of the file
deleteLine – Deletes a line of the user choice from a file
insertLine – Inserts a line of the user choice to a file in whichever line they want
showLine – Read a single line from a file
list – List all the contents of the current folder including (includes hidden files and directories)
find – Finds the first occurrence of a word in a file
exit – Closes the program

Commands are case sensitive and need to be written exactly as stated. 
They can also be chained together, 
for example you can have ```./a create show list insertLine exit ``` 
This will create a file then read a file then list all the files in the current directory 
then insert line to a file and then exits the program. 

